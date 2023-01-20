#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

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
  string os, kernel, version;
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
  string line;
  string key;
  float mem_total;
  float mem_free;
  float tmp_value;
  float mem_util;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> tmp_value;
      if (key == "MemTotal:") {
        mem_total = tmp_value;
      }
      if (key == "MemFree:") {
        mem_free = tmp_value;
      }
    }
  }
  mem_util = ((mem_total - mem_free) / mem_total);
  return mem_util;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  float val1;
  float val2;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> val1 >> val2;

  }
  return (long int)val1;
}

// TODO: Read and return the number of jiffies for the system
// Algo Source: https://knowledge.udacity.com/questions/218355 and https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
long LinuxParser::Jiffies() { 
  vector<string> jiffieMetrics = LinuxParser::CpuUtilization();
  long totalJiffies = 0;
  for (int i = 0; i < 10; i++) {
    totalJiffies += stoi(jiffieMetrics[i]);
  }
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string idx0, idx1, idx2, idx3, idx4, idx5, idx6, idx7, idx8, idx9, idx10, idx11, idx12, idx13, idx14, idx15, idx16;
  string processID = to_string(pid);
  std::ifstream filestream(kProcDirectory + processID + "/" + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> idx0 >> idx1 >> idx2 >> idx3 >> idx4 >> idx5 >> idx6 >> idx7 >> idx8 >> idx9 >> idx10 >> idx11 >> idx12 >> idx13 >> idx14 >> idx15 >> idx16;
  }
  return stol(idx13) + stol(idx14) + stol(idx15) + stol(idx16); 
}

// TODO: Read and return the number of active jiffies for the system
// Algo Source: https://knowledge.udacity.com/questions/218355 and https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
long LinuxParser::ActiveJiffies() { 
  return Jiffies() - IdleJiffies(); 
}

// TODO: Read and return the number of idle jiffies for the system
// Algo Source: https://knowledge.udacity.com/questions/218355 and https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
long LinuxParser::IdleJiffies() { 
  vector<string> jiffieMetrics = LinuxParser::CpuUtilization();
  return stoi(jiffieMetrics[CPUStates::kIdle_]) + stoi(jiffieMetrics[CPUStates::kIOwait_]);   
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  string field1, field2, field3, field4, field5, field6, field7, field8, field9, field10;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> field1 >> field2 >> field3 >> field4 >> field5 >> field6 >> field7 >> field8 >> field9 >> field10;
  }
  vector<string> cpu_jiffies = {field1, field2, field3, field4, field5, field6, field7, field8, field9, field10};
  return cpu_jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int tmp_value;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> tmp_value;
      if (key == "processes") {
        value = tmp_value;
      }
    }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int tmp_value;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> tmp_value;
      if (key == "procs_running") {
        value = tmp_value;
      }
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string val;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> val;
  }
  return val;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  int tmp_value;
  int value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> tmp_value;
      if (key == "VmSize:") {
        value = tmp_value;
      }
    }
  }
  return to_string(value / 1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string lineUID;
  string keyUID;
  string tmp_valueUID;
  string valueUID;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, lineUID)) {
      std::istringstream linestream(lineUID);
      linestream >> keyUID >> tmp_valueUID;
      if (keyUID == "Uid:") {
        valueUID = tmp_valueUID;
      }
    }
  }
  return valueUID;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  // Call function to get Uid based on pid
  string valueUID = LinuxParser::Uid(pid);
  // Parse out Username
  string lineUSER;
  string val_USER_tmp;
  string val_X;
  string val_UID_tmp;
  string USER;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, lineUSER)) {
      std::replace(lineUSER.begin(), lineUSER.end(), ':', ' ');
      std::istringstream linestream(lineUSER);
      while (linestream >> val_USER_tmp >> val_X >> val_UID_tmp) {
        if (val_UID_tmp == valueUID) {
          USER = val_USER_tmp;
        }
      }
    }
  }
  return USER;
}

// TODO: Read and return the uptime of a process since process was started in seconds
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  // Retrieve startime of process
  string line;
  string idx0, idx1, idx2, idx3, idx4, idx5, idx6, idx7, idx8, idx9, idx10, idx11, idx12, idx13, idx14, idx15, idx16, idx17, idx18, idx19, idx20, idx21;
  string processID = to_string(pid);
  std::ifstream filestream(kProcDirectory + processID + "/" + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> idx0 >> idx1 >> idx2 >> idx3 >> idx4 >> idx5 >> idx6 >> idx7 >> idx8 >> idx9 >> idx10 >> idx11 >> idx12 >> idx13 >> idx14 >> idx15 >> idx16 >> idx17 >> idx18 >> idx19 >> idx20 >> idx21;
  }
  // extract the process time tics
  float proc_tics = stof(idx21);
  // Call system uptime in seconds
  long int system_uptime = UpTime();
  // Calculate and return uptime of process in seconds
  float hertz = 100;
  return system_uptime - (long int)(proc_tics / hertz); 
}
