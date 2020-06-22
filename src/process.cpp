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
    return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long total_time, seconds;

    vector<int> cpu_data = LinuxParser::CpuUtilizationPid(pid_);

    //total_time = utime + stime
    total_time = cpu_data[LinuxParser::kUtime_] + cpu_data[LinuxParser::kStime_] +
        cpu_data[LinuxParser::kCutime_] + cpu_data[LinuxParser::kCstime_];

    //system uptime:
    long uptime = LinuxParser::UpTime();
    //seconds = uptime - (starttime / Hertz)
    seconds = uptime - (cpu_data[LinuxParser::kStarttime_] / sysconf(_SC_CLK_TCK));

    //cpu_usage = ((total_time / Hertz) / seconds
    float test = total_time / sysconf(_SC_CLK_TCK);
    cpu_utilization_ = test / seconds;
    return cpu_utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    string step = LinuxParser::Command(pid_);
    return step;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    string step = LinuxParser::Ram(pid_);
    ram_ = std::stoi(step);
    return step;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string step = LinuxParser::User(pid_);
    return step;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long int step = LinuxParser::UpTime(pid_);
    return step; 
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return ram_ < a.ram_; 
}