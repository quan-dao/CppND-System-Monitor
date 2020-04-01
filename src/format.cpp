#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  int mins = (seconds % 3600) / 60;
  int secs = seconds - hours * 3600 - mins * 60; 
  
  // construct output
  string out{""};
  out += hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours); 
  out += ":";
  out += mins < 10 ? "0" + std::to_string(mins) : std::to_string(mins); 
  out += ":";
  out += secs < 10 ? "0" + std::to_string(secs) : std::to_string(secs); 
  
  return out; 
}