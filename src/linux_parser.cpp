#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cassert>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  int memTotal{0}, memFree{0};
  int cnt{0};  // count how many lines have been parsed 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && cnt < 2) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream{line};
      if (linestream >> key >> value) {
        if (key == "MemTotal") memTotal = std::stoi(value);
        if (key == "MemFree") memFree = std::stoi(value);
      }
      cnt++;
    }
  }
  // calculate memory usage
  return 100.0f * (memTotal-memFree) / memTotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string upTime, timeInIdle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream{line};
    linestream >> upTime >> timeInIdle;
  }
  return std::stol(upTime); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string value;
  vector<string> out;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  // get first line only
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream{line};
      bool skipped_cpu{false};
      while (linestream >> value) {
        // skip first value (cpuN) 
        if (!skipped_cpu) {
          skipped_cpu = true;
          continue;
        }
        // store value to output array
        out.push_back(value);
      }
    }
  }
  // check size of out
  assert(out.size() == 10);
  return out;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key, value;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream{line};
      if (linestream >> key >> value) {  // extract first 2 elements of each line
        if (key == "processes") return std::stoi(value);
      }
    }
  }
  return -1;  // fail to look for total processes 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key, value;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream{line};
      if (linestream >> key >> value) {  // extract first 2 elements of each line
        if (key == "procs_running") return std::stoi(value);
      }
    }
  }
  return -1;  // fail to look for RunningProcesses
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream stream{kProcDirectory + std::to_string(pid) + kCmdlineFilename};
  if (stream.is_open()) std::getline(stream, cmd);
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key, value;
  std::ifstream filestream{kProcDirectory + std::to_string(pid) + kStatusFilename};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream{line};
      if (linestream >> key >> value) {  // get the first 2 elements
        if (key == "VmSize") return value;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key, value;
  std::ifstream filestream{kProcDirectory + std::to_string(pid) + kStatusFilename};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream{line};
      if (linestream >> key >> value) {  // get the first 2 elements
        if (key == "Uid") return value;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  // get Uid
  const string uid{LinuxParser::Uid(pid)}; 
  
  string line;
  string user, x, value;
  std::ifstream filestream{kPasswordPath};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream{line};
      if (linestream >> user >> x >> value) {
        if (value == uid) return user;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string value;
  string line;
  std::ifstream stream{kProcDirectory + std::to_string(pid) + kStatFilename};
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream{line};
    int cnt = 1;
    while (linestream >> value) {
      if (cnt == 22) return std::stol(value);
      cnt++;
    }
  }
  return 0; 
}