#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

// using std::set;
// using std::size_t;
// using std::string;
// using std::vector;
using namespace std;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_;}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    std::vector<Process> processes{};
    for (int i : pids)
    {
        // Process process{i};
        processes.emplace_back(i);//since process's constructor has been defined to take 'i' as arg, so in emplace_back it gets called
        //push_back constructs a temporary object which then will need to get moved into the vector v whereas emplace_back just forwards the argument 
        //and construct it directly in place with no copies or moves needed.In short, it is a good habit if you are using emplace_back instead of push_back
    }
    sort(processes.begin(), processes.end(), [](Process& processA, Process& processB) {return (processB.CpuUtilization() < processA.CpuUtilization());});
    return processes_ = processes; }

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
long System::UpTime() { return LinuxParser::UpTime();}