#include <string>
#include <iostream>
#include <sstream>
#include "system.h"
#include "format.h"

using std::string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    // string time;
    std::stringstream time;
    int h, m, s;
    h = seconds/60/60;
    m = (seconds-h*60*60)/60;
    s = (seconds-h*60*60-m*60);
    // std::cout<< h << "  " << m << "  "<< s << "  \n"<< seconds << "\n"; 
    time <<h << ":"<<m<<":"<<s<<" ";
    return time.str(); }