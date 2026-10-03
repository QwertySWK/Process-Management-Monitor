#ifndef TUI_DISPLAY_HPP
#define TUI_DISPLAY_HPP

#include "proc_parser.hpp"
#include <vector>

void init_tui();
void close_tui();
void render_tui(const std::vector<ProcessInfo>& procs, int selected_idx);

#endif