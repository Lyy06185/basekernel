#include "library/syscalls.h"
#include "library/stdio.h"
#include "library/task.h"   //include the runforSeconds func

//run for 6 secs and print the info when finishing running
int main(int argc, char **argv) {
    printf("=== Process 5 (PID: %d, Priority: %d) is running for 6 seconds ===\n", syscall_process_self(), syscall_process_pri());
    runForSeconds(6);
    return 0;
}