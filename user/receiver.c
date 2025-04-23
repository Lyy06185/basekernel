#include "library/string.h"
#include "library/syscalls.h"

int main(const int argc, const char** argv) {
    int fd = syscall_make_named_pipe(argv[1]);
    if (fd < 0) {
        printf("Failed to create named pipe: %d\n", fd);
        return 1;
    }
    int failing_fd = syscall_make_named_pipe(argv[1]);
    if (failing_fd >= 0) {
        printf("Named pipe creation should have failed\n");
        syscall_object_close(fd);
        syscall_object_close(failing_fd);
        return 1;
    }
    const int content_size = (strlen(argv[2]) + 1) * 2;
    char buffer[content_size];
    syscall_object_read(fd, buffer, content_size, 0);
    printf("read: %s\n", buffer);
    syscall_object_close(fd);
    return 0;
}

// #include "library/syscalls.h"
// #include "library/string.h"

// int main() {
//     char buffer[256];
//     strcpy(buffer, "Receiver starting\n");
//     printf("%s", buffer);

//     // Create Named PIPE
//     char *fname = "/pipe/test";
//     int res = syscall_make_named_pipe(fname);
//     if (res < 0) {
//         strcpy(buffer, "Failed to create Named PIPE\n");
//         printf("%s", buffer);
//         return 1;
//     }

//     // Open Named PIPE
//     int fd = syscall_open_named_pipe(fname);
//     if (fd < 0) {
//         strcpy(buffer, "Failed to open Named PIPE\n");
//         printf("%s", buffer);
//         return 1;
//     }

//     // Read from Named PIPE (expecting two messages)
//     char read_buffer[50];
//     int bytes_read = syscall_object_read(fd, read_buffer, 50);
//     if (bytes_read > 0) {
//         read_buffer[bytes_read] = '\0';
//         strcpy(buffer, "Received: ");
//         strcat(buffer, read_buffer);
//         printf("%s", buffer);
//     }

//     bytes_read = syscall_object_read(fd, read_buffer, 50);
//     if (bytes_read > 0) {
//         read_buffer[bytes_read] = '\0';
//         strcpy(buffer, "Received: ");
//         strcat(buffer, read_buffer);
//         printf("%s", buffer);
//     }

//     // Close Named PIPE
//     syscall_object_close(fd);

//     strcpy(buffer, "Receiver completed\n");
//     printf("%s", buffer);

//     return 0;
// }