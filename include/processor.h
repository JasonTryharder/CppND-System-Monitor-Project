#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 std::string SkUser_,
    SkNice_,
    SkSystem_,
    SkIdle_,
    SkIOwait_,
    SkIRQ_,
    SkSoftIRQ_,
    SkSteal_,
    SkGuest_,
    SkGuestNice_;
    int kUser_,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_,
    kIdlealltime,
    kSystemalltime,
    kVirtualalltime,
    kTotaltime,
    prevIdle,
    prevTotal;    
    float CPU_Percentage,
    D_Total,
    D_Idle;
};

#endif