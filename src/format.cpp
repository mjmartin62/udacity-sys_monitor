#include <string>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) {     return string(); }

string Format::ElapsedTime(long seconds) { 
    
    int hh_calc = seconds / 3600;
    int mm_calc = seconds % 3600 / 60;
    int ss_calc = seconds % 3600 % 60;

    char hh[256], ss[256], mm[256]; 
    sprintf(hh, "%02d", hh_calc);
    sprintf(mm, "%02d", mm_calc);
    sprintf(ss, "%02d", ss_calc);
 
    return string(hh) + ":" + string(mm) + ":" + string(ss); 
}