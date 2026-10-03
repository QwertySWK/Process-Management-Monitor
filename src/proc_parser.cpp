#include "../include/proc_parser.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <unordered_map>
#include <unistd.h>

namespace fs = std::filesystem;

std::vector<ProcessInfo> get_all_processes() {
    std::vector<ProcessInfo> procs;
    std::unordered_map<int, unsigned long long> current_ticks;
    
    // วนลูปอ่านโฟลเดอร์ใน /proc
    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (entry.is_directory()) {
            std::string dirname = entry.path().filename().string();
            
            // กรองเอาเฉพาะโฟลเดอร์ที่เป็นตัวเลข PID
            if (std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
                ProcessInfo p;
                p.pid = std::stoi(dirname);
                p.memory_kb = 0;
                p.cpu_percent = 0.0;

                // Read state and CPU time from /proc/[pid]/stat.
                std::ifstream stat_file("/proc/" + dirname + "/stat");
                if (stat_file.is_open()) {
                    std::string stat_line;
                    if (std::getline(stat_file, stat_line)) {
                        const std::size_t name_start = stat_line.find('(');
                        const std::size_t name_end = stat_line.rfind(')');
                        if (name_start != std::string::npos && name_end > name_start) {
                            p.name = stat_line.substr(name_start + 1, name_end - name_start - 1);

                            std::istringstream stat_stream(stat_line.substr(name_end + 2));
                            stat_stream >> p.state;
                            long long ignored;
                            for (int field = 4; field <= 13; ++field) {
                                stat_stream >> ignored;
                            }

                            unsigned long long user_ticks = 0;
                            unsigned long long system_ticks = 0;
                            stat_stream >> user_ticks >> system_ticks;
                            current_ticks[p.pid] = user_ticks + system_ticks;
                        }
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

    static std::unordered_map<int, unsigned long long> previous_ticks;
    static auto previous_time = std::chrono::steady_clock::now();
    const auto current_time = std::chrono::steady_clock::now();
    const double elapsed_seconds = std::chrono::duration<double>(current_time - previous_time).count();
    const long clock_ticks_per_second = sysconf(_SC_CLK_TCK);

    if (elapsed_seconds > 0.0 && clock_ticks_per_second > 0) {
        for (auto& process : procs) {
            const auto current = current_ticks.find(process.pid);
            const auto previous = previous_ticks.find(process.pid);
            if (current != current_ticks.end() && previous != previous_ticks.end() && current->second >= previous->second) {
                process.cpu_percent = (current->second - previous->second) * 100.0 /
                    (elapsed_seconds * clock_ticks_per_second);
            }
        }
    }

    previous_ticks = std::move(current_ticks);
    previous_time = current_time;
    return procs;
}