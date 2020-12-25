#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
{
    _pid = pid;
    getCpuUsage();
    getRam();
    // _user = User();
    // std::cout<<_pid << " ID " << _user<< " UserName " << std::endl;
    // CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { 
    return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization(){ return _cpu; }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return _ram; }

// TODO: Return the user (name) that generated this process
string Process::User() 
{return LinuxParser::User(Pid());}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid());}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }

void Process::getCpuUsage() {
    long uptime = LinuxParser::UpTime();
    vector<float> val = LinuxParser::CpuUtilization(Pid());
    if (val.size() == 5) {
        float totaltime = val[kUtime_] + val[kStime_] + val[kCutime_] + val[kCstime_];
        float seconds = uptime - val[kStarttime_];
        _cpu = totaltime / seconds;
    } else {
        _cpu = 0;
    }
}

void Process::getRam() {
    string val = LinuxParser::Ram(Pid());
    try {
        long conv = std::stol(val) / 1000;
        _ram = std::to_string(conv);
    } catch (const std::invalid_argument& arg) {
        _ram = "0";
    }
}