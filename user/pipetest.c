#include "library/syscalls.h"
#include "library/string.h"

int main(void) {
    const char *pipe_name = "test_pipe";
    const char *sender_exe = "/bin/sender.exe",
               *receiver_exe = "/bin/receiver.exe";
    printf("Testing pipe. pipe name: %s\n", pipe_name);
    const int fd_sender1 = syscall_open_file(KNO_STDDIR, sender_exe, 0, 0),
              fd_sender2 = syscall_open_file(KNO_STDDIR, sender_exe, 0, 0),
              fd_receiver = syscall_open_file(KNO_STDDIR, receiver_exe, 0, 0);
    if (fd_sender1 < 0 || fd_sender2 < 0 || fd_receiver < 0) {
        printf("Failed to open file: %d %d %d\n", fd_sender1, fd_sender2, fd_receiver);
        syscall_object_close(fd_sender1);
        syscall_object_close(fd_sender2);
        syscall_object_close(fd_receiver);
        return 1;
    }
    const char *content = "I love CityU";
    const char *argv_receiver[] = {receiver_exe, pipe_name, content};
    const char *argv_sender[] = {sender_exe, pipe_name, content};
    syscall_process_run_with_priority(fd_receiver, 3, argv_receiver, 0);
    syscall_process_run_with_priority(fd_sender1, 3, argv_sender, 0);
    syscall_process_run_with_priority(fd_sender2, 3, argv_sender, 0);
    syscall_object_close(fd_receiver);
    syscall_object_close(fd_sender1);
    syscall_object_close(fd_sender2);
    return 0;
}
// #include "library/syscalls.h"
// #include "library/string.h"

// // Simple itoa implementation
// void itoa(int value, char *str) {
//     char temp[10];
//     int i = 0, j;
//     if (value == 0) {
//         str[0] = '0';
//         str[1] = '\0';
//         return;
//     }
//     while (value > 0) {
//         temp[i++] = (value % 10) + '0';
//         value /= 10;
//     }
//     for (j = 0; j < i; j++) {
//         str[j] = temp[i - 1 - j];
//     }
//     str[i] = '\0';
// }

// int main() {
//     char buffer[256];
//     strcpy(buffer, "Starting Named PIPE test\n");
//     printf("%s", buffer);

//     // Prepare arguments for receiver
//     const char *receiver_exec = "/bin/receiver.exe";
//     const char *receiver_argv[1] = { receiver_exec };
//     int receiver_pfd = syscall_open_file(KNO_STDDIR, receiver_exec, 0, 0);
//     int pid_receiver = -1;
//     if (receiver_pfd >= 0) {
//         pid_receiver = syscall_process_run(receiver_pfd, 1, receiver_argv);
//         if (pid_receiver > 0) {
//             strcpy(buffer, "Started receiver, PID: ");
//             char pid_str[10];
//             itoa(pid_receiver, pid_str);
//             strcat(buffer, pid_str);
//             strcat(buffer, "\n");
//             printf("%s", buffer);
//         }
//         syscall_object_close(receiver_pfd);
//     } else {
//         strcpy(buffer, "Failed to find receiver.exe\n");
//         printf("%s", buffer);
//         return 1;
//     }

//     // Prepare arguments for sender1
//     const char *sender1_exec = "/bin/sender1.exe";
//     const char *sender1_argv[1] = { sender1_exec };
//     int sender1_pfd = syscall_open_file(KNO_STDDIR, sender1_exec, 0, 0);
//     int pid_sender1 = -1;
//     if (sender1_pfd >= 0) {
//         pid_sender1 = syscall_process_run(sender1_pfd, 1, sender1_argv);
//         if (pid_sender1 > 0) {
//             strcpy(buffer, "Started sender1, PID: ");
//             char pid_str[10];
//             itoa(pid_sender1, pid_str);
//             strcat(buffer, pid_str);
//             strcat(buffer, "\n");
//             printf("%s", buffer);
//         }
//         syscall_object_close(sender1_pfd);
//     } else {
//         strcpy(buffer, "Failed to find sender1.exe\n");
//         printf("%s", buffer);
//         return 1;
//     }

//     // Prepare arguments for sender2
//     const char *sender2_exec = "/bin/sender2.exe";
//     const char *sender2_argv[1] = { sender2_exec };
//     int sender2_pfd = syscall_open_file(KNO_STDDIR, sender2_exec, 0, 0);
//     int pid_sender2 = -1;
//     if (sender2_pfd >= 0) {
//         pid_sender2 = syscall_process_run(sender2_pfd, 1, sender2_argv);
//         if (pid_sender2 > 0) {
//             strcpy(buffer, "Started sender2, PID: ");
//             char pid_str[10];
//             itoa(pid_sender2, pid_str);
//             strcat(buffer, pid_str);
//             strcat(buffer, "\n");
//             printf("%s", buffer);
//         }
//         syscall_object_close(sender2_pfd);
//     } else {
//         strcpy(buffer, "Failed to find sender2.exe\n");
//         printf("%s", buffer);
//         return 1;
//     }

//     // Wait for all processes to complete
//     struct process_info info;
//     int timeout = -1;
//     info.pid = pid_receiver;
//     syscall_process_wait(&info, timeout);
//     info.pid = pid_sender1;
//     syscall_process_wait(&info, timeout);
//     info.pid = pid_sender2;
//     syscall_process_wait(&info, timeout);

//     strcpy(buffer, "Named PIPE test completed\n");
//     printf("%s", buffer);

//     return 0;
// }