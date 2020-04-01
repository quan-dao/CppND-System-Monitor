#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  // get system uptime
  long uptime = LinuxParser::UpTime();
  
  // parse /proc/[pid]/stat for time related info
  vector<string> timeInfo = LinuxParser::ProcCpuUtil(pid_);
  long utime = std::stol(timeInfo[0]);
  long stime = std::stol(timeInfo[1]);
  long cutime = std::stol(timeInfo[2]);
  long cstime = std::stol(timeInfo[3]);
  long starttime = std::stol(timeInfo[4]);

  // get number of clock ticks per sec
  long hz = sysconf(_SC_CLK_TCK);

  long total_time = utime + stime + cutime + cstime;
  float seconds = uptime*1.0f - (starttime*1.0f/hz);
  
  return (total_time*1.0f / hz) / seconds; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() const { 
  int ramMB = std::stol(LinuxParser::Ram(pid_)) / 1000;
  return std::to_string(ramMB); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  // compare Ram usage
  return std::stol(Ram()) < std::stol(a.Ram());
}

bool Process::operator>(Process const& a) const { 
  // compare Ram usage
  return std::stol(Ram()) > std::stol(a.Ram());
}