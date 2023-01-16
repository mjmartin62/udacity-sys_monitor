#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
    return processID_;
}

// TODO: Return this process's CPU utilization
// This is main function to store parameter for use in ncurses
// Return % in decimal format
float Process::CpuUtilization() const { 
    // Call total time of process in jiffies
    long active_tics = LinuxParser::ActiveJiffies(processID_);
    // Call elapsed time of process in seconds
    long process_time = LinuxParser::UpTime(processID_);
    // Calculate and return cpu utulization
    float hertz = 100;
    
    
    return active_tics;
    //return 22;
    //return (((float)active_tics) / hertz) ;
    //return (((float)active_tics) / hertz) / ((float)process_time);
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(processID_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {    
    return LinuxParser::Ram(processID_);
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(processID_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(processID_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    bool test;
    test = a.CpuUtilization() < this->CpuUtilization();  
    return test; 
}