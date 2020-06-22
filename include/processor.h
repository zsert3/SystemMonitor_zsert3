#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float PrevIdle {0}, PrevNonIdle {0}, PrevTotal {0};
};
 
#endif