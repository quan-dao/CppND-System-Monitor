#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;
using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  // get current raw utilization
  vector<string> cpuUtil = CpuUtilization();
  // unpack cpuUtil & convert string to long int
  long user = std::stol(cpuUtil[0]);
  long nice = std::stol(cpuUtil[1]);
  long system = std::stol(cpuUtil[2]);
  long idle = std::stol(cpuUtil[3]);
  long iowait = std::stol(cpuUtil[4]);
  long irq = std::stol(cpuUtil[5]);
  long softirq = std::stol(cpuUtil[6]);
  long steal = std::stol(cpuUtil[7]);
  // no need guest & guest_nice

  // compute current Idle & NonIdle
  long currIdle = idle + iowait;
  long currNonIdle = user + nice + system + irq + softirq + steal;

  // compute current Total & prev Total
  long currTotal = currIdle + currNonIdle;
  long prevTotal = prevIdle + prevNonIdle;

  // compute different between current & prev
  long totalDiff = currTotal - prevTotal;
  long idleDiff = currIdle - prevIdle;

  // update prevIdle & prevNonIdle
  prevIdle = currIdle;
  prevNonIdle = currNonIdle;

  return 1.0f * (totalDiff - idleDiff) / totalDiff; 
}