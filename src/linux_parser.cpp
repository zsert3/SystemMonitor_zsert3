#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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
    linestream >> os >> kernel >> version;
  }
  return kernel + " " + version;
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

  string line, key, value;
  float sum, total, free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::stringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "MemTotal:") total = std::stoi(value);
        if (key == "MemFree:") free = std::stoi(value);
      }
    }
  }
  // Memory Utilized = ( ( Total - Free ) / Total)
  sum = (total - free) / total;
  return sum;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string uptime, idletime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return std::atol(uptime.c_str()); 
}

// According to https://knowledge.udacity.com/questions/199377 the jiffies functions are not to be used
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
  string line, data;
  vector<string> cpu_data;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    filestream.ignore(5, ' '); // Skip the 'cpu' prefix.
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> data){
      cpu_data.push_back(data);
    }
  }
  return cpu_data;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value, processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") processes = value;
    }
  }
  return std::stoi(processes);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value, procs_running;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") procs_running = value;
    }
  }
  return std::stoi(procs_running);
}

vector<int> LinuxParser::CpuUtilizationPid(int pid){
  string line, data;
  vector<int> pid_cpu_data;
  int word_counter = 1;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> data){
      if ((word_counter >= 14 && word_counter <= 17) || word_counter == 22){
        pid_cpu_data.push_back(std::stoi(data));
      }
      word_counter++;
    }
  }
  return pid_cpu_data;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, command);
  }
  return command;
}

// returns a string with a RAM value in Kb
string LinuxParser::Ram(int pid) {
  string line, key, value, Ram;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::stringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "VmSize:") {
          Ram = to_string(stoi(value) / 1000); 
        }
      }
    }
  }
  return Ram;
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::stringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "Uid:") return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, user, found_Uid, useles;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> useles >> found_Uid){
        if (found_Uid == Uid(pid)){
          return user;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  // 22th position
  string line, clk_tc;
  // the start_time of the pid is at the 22th position in the line
  int pos = 22;
  long start_time;
  long up_time;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    // TODO CHECK
    for (auto i = 1; i <= pos; i++){
      linestream >> clk_tc;
    }
  }
  start_time = std::atol(clk_tc.c_str());
  start_time /= sysconf(_SC_CLK_TCK);
  // position 22 contains the pid starting time and the function UpTime returns the time the system is running
  up_time = UpTime() - start_time;
  return up_time;
  //return start_time; // returns seconds
}