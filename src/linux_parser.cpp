#include <dirent.h>
#include <unistd.h>
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
      std::replace(line.begin(), line.end(), ' ', '_');// to perserve the entire string "Ubuntu 16.04.7 LTS"
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' '); // operate on the string "Ubuntu 16.04.7 LTS"
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, kernel_version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel>>kernel_version;
    kernel.append(" ");
    kernel.append(kernel_version);
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
  // std::cout<< "I am here \n";
  string Title, Num, KB;
  float MemTotal, MemFree, MemUsed;  //MemAval, Buffers,
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream>>Title>>Num>>KB){
        if (Title == "MemTotal"){MemTotal = std::stof(Num);}
        else if (Title == "MemFree"){MemFree = std::stof(Num);}
        // else if (Title == "MemAvailable"){MemAval = std::stof(Num);}
        // else if (Title == "Buffers"){Buffers = std::stof(Num);}

      }
    }
  }
  MemUsed = ((MemTotal-MemFree)/MemTotal);
  // std::cout<< MemTotal << "  " << MemFree<<"  "<< MemAval << "  "<< Buffers << "\n";
  return MemUsed; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string Uptime, Idletime, line;
  long UT;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
   if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream>>Uptime>>Idletime){
        UT = std::stol(Uptime);
      }
    }
  }
  // std::cout<< UT << "  \n";
  return UT; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<float> LinuxParser::CpuUtilization(int pid) 
{ 
  vector<float> buffer{};
  std::string line, value;
  float duration = 0;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 1; i <= kStarttime_; i++) {
        linestream >> value;
        if (i == kUtime_ || i == kStime_ || i == kCutime_ || i == kCstime_ || i == kStarttime_) {
          duration = std::stof(value) / sysconf(_SC_CLK_TCK);
          buffer.emplace_back(duration);
        }
      }
    }
  }
  return buffer;
} 

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int Total_process ;
  string line, T_P;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream>>line>>T_P){
        if (line == "processes"){Total_process = std::stoi(T_P);}
      }
    }
  }
  // std::cout << T_P <<"  " << Total_process<< "\n";
  return Total_process;}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int Running_process;
  string line, R_P;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      // std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream>>line>>R_P){
        if (line == "procs_running"){Running_process = std::stoi(R_P);}
      }
    }
  }
  // std::cout << T_P <<"  " << Total_process<< "\n";
  return Running_process;}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string command = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  std::string line, key;
  std::string ramValue = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> ramValue) {
        if (key == "VmData") {
          return ramValue; // using KeyWork VmData over VmSize since Former reflect virtual memory, later reflect physical memory
        }
      }
    }
  }
  return ramValue; 
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  string UID,U_name,line,buffer,X;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream>>line>>buffer)
      {
        if (line == "Uid")
        {UID = buffer;
        // std::cout<< UID << " UID " << line << " line " ;
        return UID;}
      }
    }
  }
  return UID;
  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  std::ifstream stream1(kPasswordPath);
  string buffer,line,X,U_name;
  string UID = Uid(pid);
  if(stream1.is_open())
  {
    // buffer = "";
    while (std::getline(stream1, line))
    {
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream>>line>>X>>buffer)
      {
        if (buffer == UID){return U_name = line;}
        
      }
  
    }
  }
  return U_name; 
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { std::string line, value;
  long time = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 1; i <= kStarttime_; i++) {
        linestream >> value;
        if (i==kStarttime_) {
          try {
            time = UpTime() - std::stol(value) / sysconf(_SC_CLK_TCK);
            //The time the process started after system boot.  In kernels before Linux 2.6, this value was expressed
            //in jiffies.  Since Linux 2.6, the value is expressed in clock ticks (divide by sysconf(_SC_CLK_TCK)).
            //The format for this field was %lu before Linux 2.6.That means in order to get the unit of time it has been running since start 
            //you need to subtract it from the UpTime() of the system
            return time;
          } catch (const std::invalid_argument& arg) {
            return 0;
          }
        }
      }
    }
  }
  return time;
 }