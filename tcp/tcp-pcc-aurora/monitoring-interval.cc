#include "monitoring-interval.h"

#include <iostream>

namespace
{

// Number of probing MonitorIntervals necessary for Probing.
//const size_t kRoundsPerProbing = 4;
// Tolerance of loss rate by utility function.
const double kLossTolerance = 0.05f;
// Coefficeint of the loss rate term in utility function.
const double kLossCoefficient = -1000.0f;
// Coefficient of RTT term in utility function.
const double kRTTCoefficient = -200.0f;
// Coefficienty of the latency term in the utility function.
const double kLatencyCoefficient = 1;
// Alpha factor in the utility function.
const double kAlpha = 1;
// An exponent in the utility function.
const double kExponent = 0.9;
// An exponent in the utility function.
const size_t kMegabit = 1024 * 1024;

}  // namespace

namespace ns3
{

PacketRttSample::PacketRttSample()
    : packet_number(0), rtt(0)
{
}

PacketRttSample::PacketRttSample(SequenceNumber32 packet_number, Time rtt)
    : packet_number(packet_number), rtt(rtt)
{
}

PccMonitorIntervalQueue::PccMonitorIntervalQueue() {}

PccMonitorIntervalQueue::~PccMonitorIntervalQueue() {}

void PccMonitorIntervalQueue::Push(MonitorInterval mi) {
    m_monitor_intervals.emplace_back(mi); //emplace_back 可以在容器（如动态数组、向量等）的末尾直接构造一个新的对象
}

MonitorInterval PccMonitorIntervalQueue::Pop() {
    MonitorInterval mi = m_monitor_intervals.front();  // 获取队列的第一个 MonitorInterval 对象
    m_monitor_intervals.pop_front(); // 从队列中移除该对象
    return mi;
}

//检查 PccMonitorIntervalQueue 中的第一个 MonitorInterval 是否已经完成所有的处理
bool PccMonitorIntervalQueue::HasFinishedInterval(Time cur_time) {
    if (m_monitor_intervals.empty()) {
        return false; // 如果队列为空，则返回 false，表示没有完成的监控间隔
    }
    return m_monitor_intervals.front().AllPacketsAccountedFor(cur_time); // 检查第一个 MonitorInterval 是否已经处理了所有发送的包; 
                                                            //AllPacketsAccountedFor 方法会检查是否所有发送的包都已经被确认（即被 ACKed）或被认为是丢失的
}

void PccMonitorIntervalQueue::OnPacketSent(Time sent_time,
                                           SequenceNumber32 packet_number,
                                           uint32_t bytes) {
  if (m_monitor_intervals.empty()) {
    return;
  }

  MonitorInterval& interval = m_monitor_intervals.back(); // 获取队列中最新的监控间隔
  interval.OnPacketSent(sent_time, packet_number, bytes); // 调用最新的监控间隔的 OnPacketSent 方法，更新其状态
}

// void PccMonitorIntervalQueue::OnCongestionEvent(
//     const AckedPacketVector& acked_packets,
//     const LostPacketVector& lost_packets,
//     int64_t rtt_us,
//     Time event_time) {

//   if (m_monitor_intervals.empty()) {
//     // Skip all the received packets if we have no monitor intervals.
//     return;
//   }

//   for (MonitorInterval& interval : m_monitor_intervals) {
//     if (interval.AllPacketsAccountedFor(event_time)) {
//       // Skips intervals that have available utilities.
//       continue;
//     }

//     for (const AckedPacket& acked_packet : acked_packets) {
//       interval.OnPacketAcked(event_time, acked_packet.packet_number, acked_packet.bytes_acked, rtt_us);
//     }

//     for (const LostPacket& lost_packet : lost_packets) {
//       interval.OnPacketLost(event_time, lost_packet.packet_number, lost_packet.bytes_lost);
//     }
//   }
// }

void PccMonitorIntervalQueue::OnPacketAcked(
    Time event_time,
    SequenceNumber32 packet_number,
    uint32_t bytes_acked,
    Time rtt) 
{

    if (m_monitor_intervals.empty()) 
    {
        // Skip all the received packets if we have no monitor intervals.
        return;
    }

    for (MonitorInterval& interval : m_monitor_intervals)
    {
        if (interval.AllPacketsAccountedFor(event_time))
        {
            // Skips intervals that have available utilities.
            continue;
        }

        interval.OnPacketAcked(event_time, packet_number, bytes_acked, rtt);
    }
}

void PccMonitorIntervalQueue::OnPacketLost(
    Time event_time,
    SequenceNumber32 packet_number,
    uint32_t bytes_lost) 
{

    if (m_monitor_intervals.empty()) 
    {
        // Skip all the received packets if we have no monitor intervals.
        return;
    }

    for (MonitorInterval& interval : m_monitor_intervals)
    {
        if (interval.AllPacketsAccountedFor(event_time))
        {
            // Skips intervals that have available utilities.
            continue;
        }

        interval.OnPacketLost(event_time, packet_number, bytes_lost);
    }
}
// 返回队列中最近的一个MonitorInterval对象的引用
const MonitorInterval& PccMonitorIntervalQueue::Current() const {
  return m_monitor_intervals.back();
}

bool PccMonitorIntervalQueue::Empty() const {
  return m_monitor_intervals.empty();
}
// 返回队列中元素的数量，通过返回m_monitor_intervals向量的大小
size_t PccMonitorIntervalQueue::Size() const {
  return m_monitor_intervals.size();
}

MonitorInterval::MonitorInterval(DataRate sending_rate, Time end_time) {
    this->target_sending_rate = sending_rate;
    this->end_time = end_time;
    bytes_sent = 0;
    bytes_acked = 0;
    bytes_lost = 0;
    n_packets_sent = 0;
    n_packets_accounted_for = 0;
    first_packet_ack_time = Time();
    last_packet_ack_time = Time();
    id = next_id;
    ++next_id;
}

void MonitorInterval::OnPacketSent(Time cur_time, SequenceNumber32 packet_number, uint32_t packet_size) {
    if (n_packets_sent == 0) {
        first_packet_sent_time = cur_time;
        first_packet_number = packet_number;
        last_packet_number_accounted_for = first_packet_number - 1;
        //std::cerr << "MI " << id << " started with " << packet_number << ", dur " << (end_time - cur_time) << std::endl; 
    }
    //std::cerr << "MI " << id << " got packet " << packet_number << std::endl;
    //std::cerr << "\tSent: " << packet_number << ", dur " << (end_time - cur_time) << std::endl; 
    //std::cerr << "\tTime: " << cur_time << std::endl; 
    last_packet_sent_time = cur_time;
    last_packet_number = packet_number;
    ++n_packets_sent;
    bytes_sent += packet_size;
    // std::cout << "Mi::OnPacketSent " << "SeqNum: " << packet_number <<  " ";
    // std::cout << "PacketSize: " << packet_size <<  std::endl;
}

void MonitorInterval::OnPacketAcked(Time cur_time, SequenceNumber32 packet_number, uint32_t packet_size, Time rtt) {
    // Below code is disabled, since the TCP's SeqNum mechanism is different to QUIC's PNs.
    // if (ContainsPacket(packet_number) && packet_number > last_packet_number_accounted_for) {
    if (ContainsPacket(packet_number)) {
        int skipped = (packet_number - last_packet_number_accounted_for) - 1;
        bytes_acked += packet_size;
        n_packets_accounted_for += skipped + 1;
        // std::cout << "rtt sample size: " << packet_rtt_samples.size() << std::endl; 
        packet_rtt_samples.push_back(PacketRttSample(packet_number, rtt)); // 将RTT样本加入到样本列表中
        last_packet_number_accounted_for = packet_number;
        last_packet_ack_time = cur_time;
        //std::cerr << "MI " << id << " got ack " << packet_number << std::endl;
        //std::cerr << "\tAck time: " << cur_time << std::endl; 
    } else if (packet_number > last_packet_number) {
        n_packets_accounted_for = n_packets_sent;
        last_packet_number_accounted_for = last_packet_number;
    }
    if (packet_number >= first_packet_number && first_packet_ack_time.IsZero()) {
        first_packet_ack_time = cur_time;
        //std::cerr << "MI " << id << " first ack " << packet_number << std::endl;
        //std::cerr << "\tAck time: " << cur_time << std::endl; 
    }
    if (packet_number >= last_packet_number && last_packet_ack_time.IsZero()) {
        last_packet_ack_time = cur_time;
        //std::cerr << "MI " << id << " last ack " << packet_number << std::endl;
        //std::cerr << "\tAck time: " << cur_time << std::endl; 
    }
    if (AllPacketsAccountedFor(cur_time)) {
        //std::cout << "MI " << id << " [" << first_packet_number << ", " << last_packet_number << "] finished at packet " << packet_number << std::endl; 
    }
    static int count = 1;
    count++;
    // std::cout << "Mi::OnPacketAcked " << "SeqNum: " << packet_number <<  " ";
    // std::cout << "PacketSize: " << packet_size <<  std::endl;
}

void MonitorInterval::OnPacketLost(Time cur_time, SequenceNumber32 packet_number, uint32_t packet_size) {
    // Below code is disabled, since the TCP's SeqNum mechanism is different to QUIC's PNs.
    // if (ContainsPacket(packet_number) && packet_number > last_packet_number_accounted_for) {

    // 如果丢失的数据包在当前监控间隔内    
    if (ContainsPacket(packet_number)) {  
        int skipped = (packet_number - last_packet_number_accounted_for) - 1; // 计算跳过的包数;last_packet_number_accounted_for是上一个已知包号
        bytes_lost += packet_size; // 增加丢失字节数
        n_packets_accounted_for += skipped + 1; // 增加已知包数
        last_packet_number_accounted_for = packet_number; // 更新最后一个已知包号
    } else if (packet_number > last_packet_number) { // 如果丢失的数据包号大于当前监控间隔的最后一个包号
        n_packets_accounted_for = n_packets_sent;
        last_packet_number_accounted_for = last_packet_number;
    }
    if (packet_number >= first_packet_number && first_packet_ack_time.IsZero()) { // 如果丢失的数据包号在第一个包号之后并且第一个确认时间未设置
        first_packet_ack_time = cur_time;
    }
    if (packet_number >= last_packet_number && last_packet_ack_time.IsZero()) { // 如果丢失的数据包号在最后一个包号之后并且最后一个确认时间未设置
        last_packet_ack_time = cur_time;
    }
    if (AllPacketsAccountedFor(cur_time)) {
        //std::cout << "MI [" << first_packet_number << ", " << last_packet_number << "] finished at packet " << packet_number << std::endl; 
    }
}

bool MonitorInterval::AllPacketsSent(Time cur_time) const {
    //std::cout << "Checking if all packets sent: " << cur_time << " >= " << end_time << std::endl;
    return (cur_time >= end_time);
}

bool MonitorInterval::AllPacketsAccountedFor(Time cur_time) {
    return AllPacketsSent(cur_time) && (n_packets_accounted_for == n_packets_sent);
}

Time MonitorInterval::GetStartTime() const {
    return first_packet_sent_time;
}

DataRate MonitorInterval::GetTargetSendingRate() const {
    return target_sending_rate;
}

double MonitorInterval::GetObsThroughput() const {
    double dur = GetObsRecvDur();
    //std::cout << "Num packets: " << n_packets_sent << std::endl;
    //std::cout << "Dur: " << dur << " Acked: " << bytes_acked << std::endl;
    if (dur == 0) {
        return 0;
    }
    return 8 * bytes_acked / (dur / 1000000.0);
}

double MonitorInterval::GetObsSendingRate() const {
    double dur = GetObsSendDur();
    if (dur == 0) {
        return 0;
    }
    return 8 * bytes_sent / (dur / 1000000.0);
}

double MonitorInterval::GetObsSendDur() const {
    return (last_packet_sent_time.GetMicroSeconds() - first_packet_sent_time.GetMicroSeconds());
}

double MonitorInterval::GetObsRecvDur() const {
    //std::cerr << "MI " << id << "\n";
    //std::cerr << "\tfirst ack " << first_packet_ack_time << "\n\tlast ack " << last_packet_ack_time << "\n";
    return (last_packet_ack_time.GetMicroSeconds() - first_packet_ack_time.GetMicroSeconds());
}

double MonitorInterval::GetObsRtt() const { //计算观察时间段内的往返时延（RTT）的平均值
    if (packet_rtt_samples.empty()) {
        return 0;
    }
    double rtt_sum = 0.0;
    for (const PacketRttSample& sample : packet_rtt_samples) {
        rtt_sum += sample.rtt.GetSeconds();
    }
    return rtt_sum / packet_rtt_samples.size();
}

double MonitorInterval::GetObsRttInflation() const {
    if (packet_rtt_samples.size() < 2) {
        return 0;
    }
    double send_dur = GetObsSendDur();
    if (send_dur == 0) {
        return 0;
    }
    double first_half_rtt_sum = 0;
    double second_half_rtt_sum = 0;
    int half_count = packet_rtt_samples.size() / 2;
    for (int i = 0; i < 2 * half_count; ++i) {
        if (i < half_count) {
            first_half_rtt_sum += packet_rtt_samples[i].rtt.GetMicroSeconds();
        } else {
            second_half_rtt_sum += packet_rtt_samples[i].rtt.GetMicroSeconds();
        }
    }
    double rtt_inflation = (second_half_rtt_sum - first_half_rtt_sum) / (half_count * send_dur);
    return rtt_inflation;
}

double MonitorInterval::GetObsLossRate() const {
    return 1.0 - (bytes_acked / (double)bytes_sent);
}

void MonitorInterval::SetUtility(double new_utility) {
    utility = new_utility;
}

double MonitorInterval::GetObsUtility() const {
    return utility;
}

double MonitorInterval::GetFirstAckLatency() const {
    if (packet_rtt_samples.size() > 0) {
        return packet_rtt_samples.front().rtt.GetSeconds();
    }
    return 0;
}

double MonitorInterval::GetLastAckLatency() const {
    if (packet_rtt_samples.size() > 0) {
        return packet_rtt_samples.back().rtt.GetSeconds();
    }
    return 0;
}

bool MonitorInterval::ContainsPacket(SequenceNumber32 packet_number) {
    return (packet_number >= first_packet_number && packet_number <= last_packet_number);
}


double CalculateUtility(MonitorInterval const &cur_mi) {

  double throughput = cur_mi.GetObsThroughput();
  double sending_rate_mbps = cur_mi.GetObsSendingRate() * 1e-6;
  double rtt_inflation = cur_mi.GetObsRttInflation(); 
  double avg_rtt = cur_mi.GetObsRtt();
  double loss_rate = cur_mi.GetObsLossRate();
  
  double rtt_contribution = 900 * rtt_inflation;
  double loss_contribution = 11.35 * loss_rate;
  double sending_factor = kAlpha * pow(sending_rate_mbps, kExponent);
  loss_contribution *= -1.0 * sending_rate_mbps;
  rtt_contribution *= -1.0 * sending_rate_mbps;
  
  double utility = sending_factor + loss_contribution + rtt_contribution;
    
  return utility;
}

} // namespace ns3