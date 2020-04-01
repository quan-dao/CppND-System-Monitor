#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor() : prevIdle{0}, prevNonIdle{0} {}
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long prevIdle, prevNonIdle;  // for calculating CPU usage
};

#endif