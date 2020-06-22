#include "processor.h"

using std::vector;
using std::string;
using std::stof;
using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float Idle, NonIdle, Total, totald, idled;

    vector<string> cpu = CpuUtilization();

    Idle = stof(cpu[kIdle_]) + stof(cpu[kIOwait_]);

    NonIdle = stof(cpu[kUser_]) + stof(cpu[kNice_]) + stof(cpu[kSystem_]) + 
        stof(cpu[kIRQ_]) + stof(cpu[kSoftIRQ_]) + stof(cpu[kSteal_]);
    
    PrevTotal = PrevIdle + PrevNonIdle;
    Total = Idle + NonIdle;

    //differentiate: actual value minus the previous one
    totald = Total - PrevTotal;
    idled = Idle - PrevIdle;

    PrevIdle = Idle;
    PrevNonIdle = NonIdle;
    PrevTotal = Total;
    
    // CPU percentage:
    return (totald - idled) / totald; 
}