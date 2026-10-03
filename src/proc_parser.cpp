#include "../include/proc_parser.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

std::vector<ProcessInfo> get_all_processes() {
    std::vector<ProcessInfo> procs;
    
    // วนลูปอ่านโฟลเดอร์ใน /proc
    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (entry.is_directory()) {
            std::string dirname = entry.path().filename().string();
            
            // กรองเอาเฉพาะโฟลเดอร์ที่เป็นตัวเลข PID
            if (std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
                ProcessInfo p;
                p.pid = std::stoi(dirname);
                p.memory_kb = 0;

                // อ่านข้อมูล State
                std::ifstream stat_file("/proc/" + dirname + "/stat");
                if (stat_file.is_open()) {
                    std::string ignore_pid;
                    stat_file >> ignore_pid >> p.name >> p.state;
                    if (p.name.length() >= 2) {
                        p.name = p.name.substr(1, p.name.length() - 2); // ตัดวงเล็บ ( )
                    }
                }

                // อ่านข้อมูล Memory (VmRSS)
                std::ifstream status_file("/proc/" + dirname + "/status");
                std::string line;
                while (std::getline(status_file, line)) {
                    if (line.rfind("VmRSS:", 0) == 0) {
                        std::string mem_str;
                        for (char c : line) {
                            if (std::isdigit(c)) mem_str += c;
                        }
                        if (!mem_str.empty()) p.memory_kb = std::stol(mem_str);
                        break;
                    }
                }
                procs.push_back(p);
            }
        }
    }
    return procs;
}