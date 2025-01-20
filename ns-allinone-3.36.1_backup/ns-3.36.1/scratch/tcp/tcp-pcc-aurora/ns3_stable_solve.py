# Copyright 2023
# Original Author for PCC Aurora: Nathan Jay and Noga Rotman
# Original Author for NS3Gym: Piotr Gawlowicz
# Modified by : Haidlir Naqvi
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import faulthandler
import signal
faulthandler.enable()
import argparse
import datetime
import inspect
import os
import sys
import time

import gym
import tensorflow as tf
from stable_baselines.common.policies import MlpPolicy
from stable_baselines.common.policies import FeedForwardPolicy
from stable_baselines import PPO1

from ns3gym import ns3env
if tf.test.is_built_with_cuda():
    print("TensorFlow is built with GPU support.")
else:
    print("TensorFlow is not built with GPU support.")


tf.debugging.set_log_device_placement(True)

# 检查是否有 GPU 可用
if tf.test.is_gpu_available():
    print("----------------GPU is available")
else:
    print("--------------------GPU is not available")

# 打印系统中所有的可用设备
# from tensorflow.python.client import device_lib
# print(device_lib.list_local_devices())

# faulthandler.register(signal.SIGSEGV)
if __name__ == "__main__":
    currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
    parentdir = os.path.dirname(currentdir)
    sys.path.insert(0,parentdir) 

    parser = argparse.ArgumentParser(description='Start congestion control solver and simulation script')
    parser.add_argument('--start',
                        type=int,
                        default=1,
                        help='Start ns-3 simulation script 0/1, Default: 1')
    parser.add_argument('--iterations',
                        type=int,
                        default=1,
                        help='Number of learning iterations, Default: 0')
    parser.add_argument('--arch',
                        type=str,
                        default="32,16",
                        help='Configuration of Hidden Layers')
    parser.add_argument('--gamma',
                        type=float,
                        default="0.99",
                        help='Dicount Rate or Gammam Default: 0.99')
    parser.add_argument('--tf-model-name',
                        type=str,
                        default="",
                        help='Model in Tensorflow format')
    parser.add_argument('--sb-model-name',
                        type=str,
                        # default=f"train-saved-30Mbps-30ms-lam_1000-31ms",
                        # default=f"train-saved-30Mbps-30ms-ori-reward_k_3",
                        default= f"train-saved-100-12-30-128packets-without<0-ori-dur=0-han",
                        help='Model in Stable-Baselines format')
    parser.add_argument('--test',
                        default=1,
                        action="store_true",
                        help='Test the model, Default: 1')

    args = parser.parse_args()
    startSim = bool(args.start)
    iterationNum = int(args.iterations)
    arch_str = str(args.arch)
    gamma = float(args.gamma)
    model_name = str(args.tf_model_name)
    sb_model_name = str(args.sb_model_name)
    conductTest = bool(args.test)
    if arch_str == "":
        arch = []
    else:
        arch = [int(layer_width) for layer_width in arch_str.split(",")]
    print("Architecture is: %s" % str(arch))

    # Preparing TF
    training_sess = None
    class MyMlpPolicy(FeedForwardPolicy):
        def __init__(self, sess, ob_space, ac_space, n_env, n_steps, n_batch, reuse=False,act_fun=tf.tanh, **_kwargs):
            super(MyMlpPolicy, self).__init__(sess, ob_space, ac_space, n_env, n_steps, n_batch, reuse, net_arch=[{"pi":arch, "vf":arch}],
                                            feature_extraction="mlp",act_fun=act_fun, **_kwargs)
            training_sess = sess

    # Initiating Env
    port = 5555
    simTime = 0 # seconds, unused but required
    simArgs = {"--duration": simTime,}
    if conductTest: simArgs["--test"] = 1
    stepTime = 0  # seconds, unused but required
    seed = 12
    debug = False
    env = ns3env.Ns3Env(port=port, stepTime=stepTime, startSim=startSim, simSeed=seed, simArgs=simArgs, debug=debug)
    ob_space = env.observation_space
    ac_space = env.action_space
    print("Observation space: ", ob_space,  ob_space.dtype)
    print("Action space: ", ac_space, ac_space.dtype)
    env.reset()

    # Preparing Agent Solver
    print("gamma = %f" % gamma)
    # policy_kwargs = dict(activation_fn='tanh')
    model = PPO1(MyMlpPolicy,
            env,
            verbose=1,
            schedule='constant',
            # timesteps_per_actorbatch=8192, #每个actor完成一整套流程为一个timestep。
            # timesteps_per_actorbatch=8192,
            # optim_batchsize=2048,
            timesteps_per_actorbatch=8192,
            optim_batchsize=2048,
            gamma=gamma,
            optim_stepsize = 0.001,
            optim_epochs = 10,
            # policy_kwargs = policy_kwargs,
            tensorboard_log=f"./tensorboard/{sb_model_name}")

    # Load existing model if exist
    # sb_model_name = f"./sb_saved_models/{sb_model_name}"
    sb_model_name = f"/home/lunet/cozj/ns3.36/ns-allinone-3.36.1_han/ns-3.36.1/scratch/tcp/tcp-pcc-aurora/sb_saved_models/{sb_model_name}"
    if os.path.exists(f"{sb_model_name}.pkl"):
        print(f">> Load SB Model... {sb_model_name}")
        # PPO1.load(sb_model_name, env)
        model = PPO1.load(sb_model_name, env)

    # Train the model
    # for i in range(0, iterationNum):
    #     model.learn(total_timesteps=(8192*20))
    #     # model.learn(total_timesteps=(8192)) #直接决定episode. episode = total_timesteps/EpLenMean
    #     print(f">> Save SB Model... {sb_model_name}")
    #     model.save(sb_model_name)

    # Test the trained model
    if conductTest:
        num_tests = 1
        total_rewards = []
        rewards_list = []
        for _ in range(num_tests):
            start = time.time()
            obs = env.reset()
            # print("---obs: ", obs)
            acc_reward = 0
            reward = 0
            done = False
            info = None
            while True:
                action, _states = model.predict(obs)
                # print("---action: ", action)
                obs, reward, done, info = env.step(action)
                # print("---obs, reward, done, info: ", obs, reward, done, info)
                acc_reward += reward

                rewards_list.append(reward)
                if done:
                    break

            total_rewards.append(acc_reward)
            # print(" Reward: ", len(rewards_list))
            # print(" Reward: ", rewards_list)
            print("Total reward: ", acc_reward)
            end = time.time()
            print(f"Testing real duration: {end - start}")

        for i, reward in enumerate(total_rewards, 1):
            print(f"Test {i} reward: {reward}")

        average_reward = sum(total_rewards) / num_tests
        print(f"Average Total Reward over {num_tests} tests: {average_reward}")
    ##
    #   Save the model to the location specified below.
    ##
    if model_name:
        model_id = f"{datetime.datetime.now().strftime('%Y-%m-%d-%H:%M')}"
        export_dir = f"./tf_saved_models/{model_name}-{model_id}/"
        print(f">> Save TF Model... {export_dir}")
        with model.graph.as_default():
            pol = model.policy_pi #act_model

            obs_ph = pol.obs_ph
            act = pol.deterministic_action
            sampled_act = pol.action

            obs_input = tf.saved_model.utils.build_tensor_info(obs_ph)
            outputs_tensor_info = tf.saved_model.utils.build_tensor_info(act)
            stochastic_act_tensor_info = tf.saved_model.utils.build_tensor_info(sampled_act)
            signature = tf.saved_model.signature_def_utils.build_signature_def(
                inputs={"ob":obs_input},
                outputs={"act":outputs_tensor_info, "stochastic_act":stochastic_act_tensor_info},
                method_name=tf.saved_model.signature_constants.PREDICT_METHOD_NAME)

            signature_map = {tf.saved_model.signature_constants.DEFAULT_SERVING_SIGNATURE_DEF_KEY:
                             signature}

            model_builder = tf.saved_model.builder.SavedModelBuilder(export_dir)
            model_builder.add_meta_graph_and_variables(model.sess,
                tags=[tf.saved_model.tag_constants.SERVING],
                signature_def_map=signature_map,
                clear_devices=True)
            model_builder.save(as_text=True)
    
    env.close()
