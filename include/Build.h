#pragma once
#include <string>

struct Build {
    int id = 0;
    int projectId = 0;             
    std::string buildDate;          
    std::string status;             
    double durationSeconds = 0.0;   
};