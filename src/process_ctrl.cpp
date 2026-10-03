#include "../include/process_ctrl.hpp"
#include <signal.h>

bool send_signal(int pid, int sig) {
    return kill(pid, sig) == 0;
}