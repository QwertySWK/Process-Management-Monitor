#include "../include/tui_display.hpp"
#include <ncurses.h>

void init_tui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // ไม่หยุดรอรับ Input
    curs_set(0);           // ซ่อน Cursor
}

void close_tui() {
    endwin();
}

void render_tui(const std::vector<ProcessInfo>& procs, int selected_idx) {
    clear();
    
    // Header
    attron(A_BOLD);
    mvprintw(0, 0, "=== Linux Process Management Monitor ===");
    attroff(A_BOLD);
    mvprintw(1, 0, "Keys: [Up/Down] Navigate | [t] Terminate | [k] Kill | [s] Suspend | [c] Continue | [q] Quit");
    mvprintw(3, 0, "%-10s %-30s %-10s %-12s %-10s", "PID", "NAME", "STATE", "CPU(%)", "MEM(KB)");
    mvprintw(4, 0, "--------------------------------------------------------------------------------");

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int start_row = 5;
    int display_count = max_y - start_row - 1;

    // คำนวณ Scrolling Offset
    int offset = 0;
    if (selected_idx >= display_count) {
        offset = selected_idx - display_count + 1;
    }

    // ลิสต์รายการโปรเซส
    for (int i = 0; i < display_count && (i + offset) < (int)procs.size(); ++i) {
        int actual_idx = i + offset;
        
        if (actual_idx == selected_idx) {
            attron(A_REVERSE); // Highlight แถวที่เลือก
        }
        
        mvprintw(start_row + i, 0, "%-10d %-30s %-10c %-12.2f %-10ld",
                 procs[actual_idx].pid, 
                 procs[actual_idx].name.c_str(), 
                 procs[actual_idx].state,
             procs[actual_idx].cpu_percent,
                 procs[actual_idx].memory_kb);
                 
        if (actual_idx == selected_idx) {
            attroff(A_REVERSE);
        }
    }
    refresh();
}