#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hour, min, sec;

    hour = seconds/3600;
	seconds = seconds%3600;
	min = seconds/60;
	seconds = seconds%60;
	sec = seconds;
    
    return  std::to_string(hour) + ":" + std::to_string(min) + ":" + std::to_string(sec); 
}