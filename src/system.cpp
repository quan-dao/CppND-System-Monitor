#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  // get all procs' pid
  vector<int> pids = LinuxParser::Pids();

  // check if this pid represents a non existed proc, if yes add it proceses_
  for (int pid : pids) {
    // create a proc of this pid
    Process proc(pid);
    // check if this process has already existed
    if (std::find(processes_.begin(), processes_.end(), proc) == processes_.end()) {
      // not found -> not exist
      // add proc to processes_
      processes_.push_back(proc);
    }
  } 

  // sort processes, descending order
  std::sort(processes_.begin(), processes_.end(), [](Process const &a, Process const &b) {return a > b; });

  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }