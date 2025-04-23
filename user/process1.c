#include "library/syscalls.h"
#include "library/stdio.h"
#include "library/task.h"

int main(int argc, char **argv) {
    printf("=== Process 1 (PID: %d, Priority: %d) is running for 6 seconds ===\n", syscall_process_self(), syscall_process_pri());
    runForSeconds(6);
    return 0;
}