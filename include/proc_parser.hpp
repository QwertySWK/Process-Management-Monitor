#ifndef PROC_PARSER_HPP
#define PROC_PARSER_HPP

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    char state;
    long memory_kb;
    double cpu_percent;
};

std::vector<ProcessInfo> get_all_processes();

#endif