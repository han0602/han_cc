/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

 /* Topology:
 *
 *   Right Leafs (Clients)                      Left Leafs (Sinks)
 *           |            \                    /        |
 *           |             \    bottleneck    /         |
 *           |              R0--------------R1          |
 *           |             /                  \         |
 *           |   access   /                    \ access |
 *           N -----------                      --------N
 */

// #include "ns3/config-store.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/opengym-module.h"

#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <random>

#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/ipv4-header.h"
#include "ns3/address.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TcpPccAuroraSim");

static void
CwndTracer(uint32_t oldval, uint32_t newval)
{
    std::cout << "cwnd " << std::fixed << std::setprecision(6) << Simulator::Now().GetSeconds() //小数点后的位数为6位
               << std::setw(12) << oldval << " " << newval << std::endl; // 宽度为12个字符，以便对齐。然后，将 oldval 和 newval 分别输出
}

static void
PacingRateTracer(DataRate oldval, DataRate newval)
{
    std::cout << "pacing rate " << std::fixed << std::setprecision(6) << Simulator::Now().GetSeconds()
                     << std::setw(12) << newval.GetBitRate() / 1e6 << std::endl;
}

static void
SsThreshTracer(uint32_t oldval, uint32_t newval)
{
    std::cout << "ss threshold " << std::fixed << std::setprecision(6) << Simulator::Now().GetSeconds()
                   << std::setw(12) << newval << std::endl;
}

void
ConnectSocketTraces()
{
    Config::ConnectWithoutContext("/NodeList/4/$ns3::TcpL4ProtocolCustom/SocketList/0/CongestionWindow", //指定了一个TCP协议栈中的特定套接字的拥塞窗口参数
                                  MakeCallback(&CwndTracer));//回调函数 创建一个可被连接到参数变化事件的回调函数
    Config::ConnectWithoutContext("/NodeList/4/$ns3::TcpL4ProtocolCustom/SocketList/0/PacingRate",
                                  MakeCallback(&PacingRateTracer));
    Config::ConnectWithoutContext("/NodeList/4/$ns3::TcpL4ProtocolCustom/SocketList/0/SlowStartThreshold",
                                  MakeCallback(&SsThreshTracer));
}

// Ptr<NormalRandomVariable> data_rate_distribution = CreateObject<NormalRandomVariable>();
Ptr<LogNormalRandomVariable> data_rate_distribution; //这个指针可以用来访问和操作一个LogNormal随机变量对象

// static void
// UpdateDataRate(PointToPointNetDevice *dev)
// {
//     float sample =  data_rate_distribution->GetValue();
//     if (sample < 2.0)
//     {
//         sample = 2.0;
//     }
//     // std::cout << sample << std::endl;
//     auto sample_bw = (uint64_t)(sample * 1e6);
//     dev->SetDataRate(DataRate(sample_bw));

// }

// static void
// UpdateDataRate_L(PointToPointNetDevice *dev)
// {
// std::random_device rd;
//     std::mt19937 gen(rd());

//     // 定义均值为12.0f，标准差为10.0f的正态分布 (float 类型)
//     std::normal_distribution<float> dist(12.0f, 2.0f);

//     // 生成一个随机数 (float 类型)
//     float sample = dist(gen);
//     if (sample < 2.0)
//     {
//         sample = 2.0;
//     }
//     // std::cout << sample << std::endl;
//     auto sample_bw = (uint64_t)(sample * 1e6);
//     dev->SetDataRate(DataRate(sample_bw));
// }

// static void
// UpdateDataRate_R(PointToPointNetDevice *dev)
// {


//     // 生成一个随机数 (float 类型)
//     float sample = 15;

//     // std::cout << sample << std::endl;
//     auto sample_bw = (uint64_t)(sample * 1e6);
//     dev->SetDataRate(DataRate(sample_bw));
// }



// static void
// UpdateDataRate_L(PointToPointNetDevice *dev)
// {


//      // 生成一个随机数 (float 类型)
//     static float sample = 20;
//     // sample  = sample - 10;
//     // std::cout <<"samples :  "<< sample << std::endl;
//     auto sample_bw = (uint64_t)(sample * 1e6);
//     dev->SetDataRate(DataRate(sample_bw));
// }

// static void
// UpdateDataRate_R(PointToPointNetDevice *dev)
// {


//      // 生成一个随机数 (float 类型)
//     static float sample = 20;
//     // sample  = sample - 10;
//     // std::cout <<"samples :  "<< sample << std::endl;
//     auto sample_bw = (uint64_t)(sample * 1e6);
//     dev->SetDataRate(DataRate(sample_bw));
// }


//------------------------------------------
static void
UpdateDataRate_L(PointToPointNetDevice *dev, bool increase, int call_count)
{
    static float sample = 10;

    // 前四次增加10，后四次减少10
    if (increase) {
        sample += 10; // 增加10
    } else {
        sample -= 10; // 减少10
    }

    // if (sample<10){
    //     sample=10;
    // }

    std::cout << "Call count: " << call_count << " | sample: " << sample << std::endl;

    auto sample_bw = (uint64_t)(sample * 1e6);
    dev->SetDataRate(DataRate(sample_bw));
}

static void
UpdateDataRate_R(PointToPointNetDevice *dev, bool increase, int call_count)
{
    static float sample = 30;

    // 前四次增加10，后四次减少10
    // if (increase) {
    //     sample += 10; // 增加10
    // } else {
    //     sample -= 10; // 减少10
    // }

    // if (sample<10){
    //     sample=10;
    // }

    // std::cout << "Call count: " << call_count << " | sample: " << sample << std::endl;

    auto sample_bw = (uint64_t)(sample * 1e6);
    dev->SetDataRate(DataRate(sample_bw));
}





//每秒throughput

//----------------------
uint64_t totalBytesReceived = 0;
Ptr<NetDevice> monitoredDevice;

// ---计算吞吐量的定时器回调函数----
void CalculateThroughput ()
{
    double throughput = (totalBytesReceived * 8.0) / (1e6); // 转换为 Mbps
    // std::cout << Simulator::Now ().GetSeconds () << "s: Throughput = " << throughput << " Mbps" << std::endl;
    std::cout << throughput  << std::endl;
    totalBytesReceived = 0;  // 清零计数器
    Simulator::Schedule (Seconds (1), &CalculateThroughput); // 每秒调度一次
}

// 回调函数：监测设备接收的数据包
void RxCallback (Ptr<const Packet> packet)
{
    totalBytesReceived += packet->GetSize();  // 累加接收的数据包大小
}



// 每秒的延迟
std::vector<double> delayList;
uint32_t packetsReceived = 0; // 记录收到的数据包数量
Time lastPrintTime = Seconds(0); // 上一次打印的时间
Time lastSendTime; // 上一次发送的时间

void TxCallback(Ptr<const Packet> packet)
{
    lastSendTime = Simulator::Now(); // 记录发送时间
}

void RxDelayCallback(Ptr<const Packet> packet)
{
    totalBytesReceived += packet->GetSize();  // 累加接收的数据包大小
    Time receiveTime = Simulator::Now();
    
    // 计算延迟
    Time delay = receiveTime - lastSendTime;
    // std::cout << "receiveTime  : " << receiveTime  << std::endl;
    // std::cout << "lastSendTime  : " << lastSendTime  << std::endl;
    // 记录延迟
    delayList.push_back(delay.GetMicroSeconds());
    packetsReceived++;
}

void PrintDelay()
{
    double totalDelay = 0.0;
    for (double d : delayList) {
        totalDelay += d;
    }
    
    if (packetsReceived > 0) {
        double averageDelay = totalDelay*(1e-6) / packetsReceived;
        // std::cout << "totalDelay  : " << totalDelay << " microseconds" << std::endl;
        // std::cout << "packetsReceived : " << packetsReceived << " microseconds" << std::endl;
        std::cout << "Average Delay over last second: " << averageDelay << " microseconds" << std::endl;
    }

    // 清空延迟列表和计数器
    delayList.clear();
    packetsReceived = 0;

    // 每秒调度一次
    Simulator::Schedule(Seconds(1), &PrintDelay);
}
//-------

// 动态变化队列大小
uint32_t initialQueueSize = 15;  // 初始队列大小
double timeInterval = 10.0;        // 时间间隔，单位为秒

void UpdateQueueSize(Ptr<PointToPointNetDevice> device, uint32_t &currentQueueSize)
{
    // 更新队列大小
    currentQueueSize *= 10;
    std::cout << "Updating queue size to: " << currentQueueSize << " packets." << std::endl;
    
    // 修改队列大小
    device->GetQueue()->SetAttribute("MaxSize", StringValue(std::to_string(currentQueueSize) + "p"));
    
    // 再次调度10秒后执行
    Simulator::Schedule(Seconds(10.0), &UpdateQueueSize, device, std::ref(currentQueueSize));
}



void HandleReceivedPacket(Ptr<TcpSocketState> state, const Ptr<const Packet>& packet, const Address& from ) {
    Ipv4Header ipv4Header;
    packet->PeekHeader(ipv4Header);

    Ipv4Address destIp = ipv4Header.GetDestination();
    uint16_t destPort = 1339;

    state->UpdateFlowIdentifier(destIp, destPort);
    NS_LOG_INFO("Flow Identifier updated: " << state->GetFlowIdentifier());
}




int
main(int argc, char* argv[]) //argc 是命令行参数的数量，argv 是指向这些参数的指针数组。
{
    srand(time(NULL)); //用当前时间作为种子，以确保每次运行程序时都会得到不同的种子值
    RngSeedManager::SetSeed(rand());  // Changes seed from default of 1 to 3 设置随机数生成器的种子和运行编号。这些设置可能会影响到模拟中使用的随机数生成行为
    RngSeedManager::SetRun(rand());   // Changes run number from default of 1 to 7
    data_rate_distribution = CreateObject<LogNormalRandomVariable>(); //创建了一个对数正态分布的随机变量对象 模拟真实网络中的数据传输速率的随机性
    double data_rate_mean = 12.0;
    double data_rate_variance = 10;
    double data_rate_mu = std::log(data_rate_mean) - 0.5 * std::log(data_rate_variance/data_rate_mean);
    double data_rate_sigma = std::sqrt(std::log(1 + data_rate_variance/(data_rate_mean*data_rate_mean)));
    data_rate_distribution->SetAttribute("Mu", DoubleValue(data_rate_mu));
    data_rate_distribution->SetAttribute("Sigma", DoubleValue(data_rate_sigma));
    // OpenGym Env --- has to be created before any other thing

    uint32_t openGymPort = 5555;
    Ptr<OpenGymInterface> openGymInterface;
    openGymInterface = OpenGymInterface::Get(openGymPort);
    
    uint32_t run = 0;
    double tcpEnvTimeStep = 0.1;// TCP环境的时间步长，即模拟中每个步长的持续时间
    double duration = 120; //模拟的总持续时间
    bool tracing = false; //是否启用跟踪功能
    uint32_t maxBytes = 0; //发送的最大字节数
    // uint32_t maxBytes = 30 * 1500000;
    uint32_t isTest = 1;

    //
    // Allow the user to override any of the defaults at
    // run-time, via command-line arguments
    //
    CommandLine cmd(__FILE__);
    cmd.AddValue("tracing", "Flag to enable/disable tracing", tracing);
    cmd.AddValue("maxBytes", "Total number of bytes for application to send", maxBytes);
    cmd.AddValue("openGymPort", "Port number for OpenGym env. Default: 5555", openGymPort);
    cmd.AddValue("simSeed", "Seed for random generator. Default: 1", run);
    cmd.AddValue("envTimeStep", "Time step interval for time-based TCP env [s]. Default: 0.1s", tcpEnvTimeStep);
    cmd.AddValue("duration", "Time to allow flows to run in seconds", duration);
    cmd.AddValue("test", "Print Flowstats", isTest);
    cmd.Parse(argc, argv);

    Time::SetResolution (Time::NS); //时间的分辨率为纳秒
    LogComponentEnableAll (LOG_PREFIX_TIME); //启用所有与时间相关的日志记录组件
    LogComponentEnableAll (LOG_PREFIX_FUNC); // 启用所有与function相关的日志记录组件
    LogComponentEnableAll (LOG_PREFIX_NODE);// 启用所有与node相关的日志记录组件

    // LogComponentEnable("BulkSendApplication", LOG_LEVEL_INFO);
    // LogComponentEnable("TcpPccAurora", LOG_LEVEL_INFO);
    // LogComponentEnable("TcpSocketBaseCustom", LOG_LEVEL_INFO);

    // Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpNewReno"));
    // ConfigStore config;
    // config.ConfigureDefaults ();

    //
    // Explicitly create the nodes required by the topology (shown above).
    //
    NS_LOG_INFO("Create nodes.");
    NodeContainer nodes;
    // nodes.Create(2);




    NS_LOG_INFO("Create channels.");

    //
    // Explicitly create the point-to-point link required by the topology (shown above).
    //
    PointToPointHelper accessLink;
    accessLink.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
    accessLink.SetChannelAttribute("Delay", StringValue("0.1ms"));
    // accessLink.SetQueue("ns3::DropTailQueue<Packet>", "MaxSize", StringValue("1p"));

    PointToPointHelper bottleneckLink;
    bottleneckLink.SetDeviceAttribute("DataRate", StringValue("12Mbps"));
    bottleneckLink.SetChannelAttribute("Delay", StringValue("30ms"));
    bottleneckLink.SetQueue("ns3::DropTailQueue<Packet>", "MaxSize", StringValue("150p"));
    /*
    // Arrange Dumbell
    PointToPointDumbbellHelper dumbell (1, accessLink,
                                    1, accessLink,
                                    bottleneckLink);

    auto leftRouter = dumbell.GetLeft();   

    ns3::PointToPointNetDevice *leftRouterBottlenecNetDevice = dynamic_cast<ns3::PointToPointNetDevice*>(&(*(leftRouter->GetDevice(0))));                             
    */


   /*查看有几个节点
   Ptr<Node> node = NodeList::GetNode(100); // 获取索引为4的节点
if (node != nullptr) {
    std::cout << "Node index: " << node->GetId() << std::endl; // 打印节点索引
    std::cout << "Node name: " << Names::FindName(node) << std::endl; // 打印节点名称
    // 在这里打印其他节点属性或配置信息
} else {
    std::cout << "Node with index 4 not found." << std::endl;
}
*/
    // Arrange new Dumbell
    PointToPointDumbbellHelper dumbell (2, accessLink, //gai!!!!!!!!!
                                    2, accessLink,
                                    bottleneckLink);


    auto leftRouter = dumbell.GetLeft();
    auto rightRouter = dumbell.GetRight();
    
/*
    if (leftRouter != nullptr && leftRouter->GetDevice(0) != nullptr&&leftRouterBottlenecNetDevice !=nullptr&&dumbell.GetLeft()!=nullptr) {
    // 这里是指针不为空的情况下的代码
    std::cout << "不是空指针" << std::endl;;
    // 现在你可以安全地使用 leftRouterBottlenecNetDevice 指针
} else {
    // 这里是指针为空的情况下的处理逻辑
    std::cout << "leftRouter 或 GetDevice(0) 返回了空指针" << std::endl;
}

*/
    //
    // Install the internet stack on the nodes
    //
    InternetStackHelper internet;
    // dumbell.InstallStack(internet);
    internet.Install(dumbell.GetRight());
    internet.Install(dumbell.GetLeft());
    internet.Install(dumbell.GetRight(0));
    internet.Install(dumbell.GetRight(1));//gai!!!!!!!!!!!!!!!!!!1
    InternetStackHelper internetCustomL4;
    internetCustomL4.SetTcp("ns3::TcpL4ProtocolCustom");
    internetCustomL4.Install(dumbell.GetLeft(0));
    internetCustomL4.Install(dumbell.GetLeft(1));//gai!!!!!!!!!!!!!!!!!!!!!!




    //
    // We've got the "hardware" in place.  Now we need to add IP addresses.
    //
    NS_LOG_INFO("Assign IP Addresses.");
    dumbell.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                                 Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),
                                 Ipv4AddressHelper ("10.3.1.0", "255.255.255.0")
                            
                                 );
    NS_LOG_INFO ("Initialize Global Routing.");
    Ipv4GlobalRoutingHelper::PopulateRoutingTables (); //根据拓扑结构和路由算法，为每个节点计算并填充路由表，以便节点能够正确地进行数据包转发。

    NS_LOG_INFO("Create Applications.");

    // Ptr<Socket> socket = Socket::CreateSocket(nodes.Get(5), TcpSocketFactory::GetTypeId());
    // Ptr<TcpSocketState> state = CreateObject<TcpSocketState>();
    // socket->TraceConnectWithoutContext(
    //     "Rx", 
    //     MakeBoundCallback(&HandleReceivedPacket, state)
    // );



    //
    // Create a BulkSendApplication and install it on node left
    // Nodelist: 左侧router：0，右侧router：1，左侧第0个sender：2，左侧第1个sender：3
    uint16_t port = 1339; // well-known echo port number
     // 创建一个 RateErrorModel（丢包模型）
    // Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
    // em->SetAttribute ("ErrorRate", DoubleValue (0.0005));  // 设置5%的随机丢包率

    BulkSendHelper source("ns3::TcpSocketFactory", InetSocketAddress(dumbell.GetRightIpv4Address(0), port)); //目的地址和端口
    // BulkSendHelper source("ns3::TcpSocketFactory", InetSocketAddress(dumbell.GetLeftIpv4Address(0), port)); // 发给左侧另一个node
    Config::Set("/NodeList/3/$ns3::TcpL4ProtocolCustom/SocketType", StringValue("ns3::TcpPccAurora")); 
    //Config::Set("/NodeList/4/$ns3::TcpL4ProtocolCustom/SocketType", StringValue("ns3::TcpL4ProtocolCustom"));
    Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(2500000));
    Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(2500000));
    Config::SetDefault("ns3::TcpSocket::DelAckCount", UintegerValue(2));
    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(1448));
    // Set the amount of data to send in bytes.  Zero is unlimited.
    source.SetAttribute("MaxBytes", UintegerValue(maxBytes));


    ApplicationContainer sourceApps = source.Install(dumbell.GetLeft(1));                                                  
    sourceApps.Start(Seconds(0.0));
    sourceApps.Stop(Seconds(2.0));


    
//     //create a new flow
//     //
     uint16_t port1 = 1338;

    BulkSendHelper source_new("ns3::TcpSocketFactory", InetSocketAddress(dumbell.GetRightIpv4Address(1), port1));
    // Config::Set("/NodeList/1/$ns3::TcpL4ProtocolCustom/SocketType", StringValue("ns3::TcpCubic"));
    Config::Set("/NodeList/3/$ns3::TcpL4ProtocolCustom/SocketType", StringValue("ns3::TcpPccAurora")); 
    Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(2500000));//250000 至少1000
    Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(2500000));//500000
    Config::SetDefault("ns3::TcpSocket::DelAckCount", UintegerValue(2));
    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(1448));
    // Set the amount of data to send in bytes.  Zero is unlimited.
    source_new.SetAttribute("MaxBytes", UintegerValue(maxBytes));
    ApplicationContainer source_newApps = source_new.Install(dumbell.GetLeft(1));    // gai!!!!!!!!!!!!!!!!!                                              
    source_newApps.Start(Seconds(30.0));
    source_newApps.Stop(Seconds(50.0));


    //
    // Create a PacketSinkApplication and install it on node right
    //
    
    PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
    ApplicationContainer sinkApps = sink.Install(dumbell.GetRight(0));//右侧node接收
    // ApplicationContainer sinkApps = sink.Install(dumbell.GetLeft(0));//左侧另一个node接收
    sinkApps.Start(Seconds(0.0));
    sinkApps.Stop(Seconds(60.0));

    PacketSinkHelper sink_new("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port1));
    ApplicationContainer sink_newApps = sink_new.Install(dumbell.GetRight(1));
    sink_newApps.Start(Seconds(0.0));
    sink_newApps.Stop(Seconds(50.0));


          // 设置 Trace，监听数据包接收事件
    monitoredDevice = dumbell.GetRightDevices().Get(0); // 监听节点1的 NetDevice
    monitoredDevice->TraceConnectWithoutContext ("PhyRxEnd", MakeCallback (&RxCallback));  // 连接回调函数

      // 每秒调用一次吞吐量计算函数
    Simulator::Schedule (Seconds (0.1), &CalculateThroughput);
    // // Simulator::Schedule(Seconds(1.0), &CalculatePacketLoss);
    // // Simulator::Schedule(Seconds(0.0), &CalculateAverageDelay);
    // Simulator::Schedule(Seconds(1), &PrintDelay);
   


    // 安装设备，例如
    // NetDeviceContainer devices = bottleneckLink.Install(nodes);
    // // 获取指向 bottleneckLink 的设备指针
    // Ptr<PointToPointNetDevice> device = DynamicCast<PointToPointNetDevice>(devices.Get(0));
    // // 初始队列大小
    // uint32_t currentQueueSize = initialQueueSize;

    // 调度在10秒后第一次更新队列大小
    // Simulator::Schedule(Seconds(10.0), &UpdateQueueSize, device, std::ref(currentQueueSize));


    //
    // Set up tracing if enabled
    //
    if (tracing)
    {
        AsciiTraceHelper ascii;
        bottleneckLink.EnableAsciiAll(ascii.CreateFileStream("tcp-bulk-send.tr"));
        bottleneckLink.EnablePcapAll("tcp-bulk-send", false);
    }


    ns3::PointToPointNetDevice *leftRouterBottlenecNetDevice = dynamic_cast<ns3::PointToPointNetDevice*>(&(*(leftRouter->GetDevice(0))));
    ns3::PointToPointNetDevice *rightRouterBottlenecNetDevice = dynamic_cast<ns3::PointToPointNetDevice*>(&(*(rightRouter->GetDevice(0))));
    uint32_t simulation_duration = 60;
 
    // for (uint32_t i = 1; i < simulation_duration; i+=1)
    // {
    //     Simulator::Schedule(Seconds(i), &UpdateDataRate_L, leftRouterBottlenecNetDevice);
    //     Simulator::Schedule(Seconds(i), &UpdateDataRate_R, rightRouterBottlenecNetDevice);
    // }


 //-----------------------
    for (uint32_t i = 1; i < simulation_duration; i += 15)
    {
    int call_count = (i / 15); // 计算当前是第几次调用

    // 前四次 increase 为 true，后四次 increase 为 false
    bool increase = call_count <= 1; 

    Simulator::Schedule(Seconds(i), &UpdateDataRate_L, leftRouterBottlenecNetDevice, increase, call_count);
    Simulator::Schedule(Seconds(i), &UpdateDataRate_R, rightRouterBottlenecNetDevice, increase, call_count);
    }

/* 
   for (uint32_t i = 1; i < simulation_duration; i++) {
    if (leftRouterBottlenecNetDevice != nullptr) {
        Simulator::Schedule(Seconds(i), &UpdateDataRate, leftRouterBottlenecNetDevice);
    } else {
        std::cout << "leftRouterBottlenecNetDevice 指针为空，无法安排函数调用" << std::endl;
        // 在这里处理空指针的情况，可以选择跳过本次迭代或采取其他适当的措施
    }
}
*/

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll ();  

    


    //
    // Now, do the actual simulation.
    //
    NS_LOG_INFO("Run Simulation.");
    Simulator::Stop(Seconds(simulation_duration));
    // Simulator::Stop(Seconds(2.0));
    Simulator::Run();

    if (isTest)
    {
    monitor->CheckForLostPackets ();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();



    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    
    {
        // std::cout << "here here\n";
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
        // if (t.sourceAddress != dumbell.GetLeftIpv4Address(1))
        if (t.sourceAddress != dumbell.GetLeftIpv4Address(1)&&t.sourceAddress != dumbell.GetLeftIpv4Address(0))
            {
            continue;
            }

            auto flow_duration_tx = i->second.timeLastTxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds();
            auto flow_duration_rx = i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstRxPacket.GetSeconds();
            std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << " "<<t.destinationPort <<")\n";
            std::cout << "  Flow Duration: " << flow_duration_tx << "s | " << flow_duration_rx << "s\n";
            std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
            std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
            std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / flow_duration_tx / 1000 / 1000  << "Mbps\n";
            std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
            std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
            std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / flow_duration_rx / 1000 / 1000  << "Mbps\n";
            std::cout << "  Avg Delay:   " << i->second.delaySum.GetMilliSeconds() / i->second.rxPackets << "ms\n";
            std::cout << "  Loss:   " << 1.0 - (static_cast<float>(i->second.rxBytes) / i->second.txBytes) << "\n";
            
    }
    }
    
    monitor->SerializeToXmlFile("han_proposed.xml", true, true);

    openGymInterface->NotifySimulationEnd();
    Simulator::Destroy();
    NS_LOG_INFO("Done.");

    // Ptr<PacketSink> sink1 = DynamicCast<PacketSink>(sinkApps.Get(0));
    // std::cout << "Total Bytes Received: " << sink1->GetTotalRx() << std::endl;
    // std::cout << "Estimated Throughput: " << sink1->GetTotalRx() * 8 / 10 / 1000 << std::endl;

    return 0;
}




