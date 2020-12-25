#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <iostream>
#include <sstream>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    int Total_process ;
  std::string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream >>line >>SkUser_>>SkNice_>>SkSystem_>>SkIdle_>>SkIOwait_>>SkIRQ_>>SkSoftIRQ_>>SkSteal_>>SkGuest_>>SkGuestNice_)
      {
        if (line == "cpu"){
            kUser_ = std::stoi(SkUser_);
            kNice_ = std::stoi(SkNice_);
            kSystem_ = std::stoi(SkSystem_);
            kIdle_ = std::stoi(SkIdle_);
            kIOwait_ = std::stoi(SkIOwait_);
            kIRQ_ = std::stoi(SkIRQ_);
            kSoftIRQ_ = std::stoi(SkSoftIRQ_);
            kSteal_ = std::stoi(SkSteal_);
            kGuest_ = std::stoi(SkGuest_);
            kGuestNice_ = std::stoi(SkGuestNice_);
            kUser_ = kUser_ - kGuest_;
            kNice_ = kNice_ - kGuestNice_;
            kIdlealltime = kIdle_ + kIOwait_;
            kSystemalltime = kSystem_ + kIRQ_ + kSoftIRQ_;
            kVirtualalltime = kGuest_ + kGuestNice_;
            kTotaltime = kUser_+kNice_+kSystemalltime+kIdlealltime+kSteal_+kVirtualalltime;
            D_Total = kTotaltime - prevTotal;
            D_Idle = kIdlealltime - prevIdle;
            CPU_Percentage = (D_Total-D_Idle)/D_Total;
            // std::cout << D_Total << "  " << D_Idle <<"  " <<CPU_Percentage << "  " <<"  \n";
            prevTotal = kTotaltime;
            prevIdle = kIdlealltime;
            // CPU_Percentage = 0.8;
            
        }
      }
    }
  }
  return CPU_Percentage; }