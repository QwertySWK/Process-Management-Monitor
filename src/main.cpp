#include "../include/proc_parser.hpp"
#include "../include/process_ctrl.hpp"
#include "../include/tui_display.hpp"
#include <ncurses.h>
#include <unistd.h>
#include <csignal>

int main() {
    init_tui();

    int selected_idx = 0;
    bool running = true;

    while (running) {
        std::vector<ProcessInfo> procs = get_all_processes();
        if (procs.empty()) break;

        // ล็อกค่า Index ไม่ให้ทะลุขอบเขต
        if (selected_idx >= (int)procs.size()) selected_idx = procs.size() - 1;
        if (selected_idx < 0) selected_idx = 0;

        render_tui(procs, selected_idx);

        // จัดการ Input จากคีย์บอร์ด
        int ch = getch();
        switch (ch) {
            case 'q': running = false; break;
            case KEY_UP: if (selected_idx > 0) selected_idx--; break;
            case KEY_DOWN: if (selected_idx < (int)procs.size() - 1) selected_idx++; break;
            
            // ส่ง Signals ไปยัง PID ที่ถูกเลือก
            case 't': send_signal(procs[selected_idx].pid, SIGTERM); break;
            case 'k': send_signal(procs[selected_idx].pid, SIGKILL); break;
            case 's': send_signal(procs[selected_idx].pid, SIGSTOP); break;
            case 'c': send_signal(procs[selected_idx].pid, SIGCONT); break;
        }

        usleep(100000); // หน่วงเวลา 100ms ลดภาระ CPU
    }

    close_tui();
    return 0;
}