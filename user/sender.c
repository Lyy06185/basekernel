#include "library/syscalls.h"
#include "library/string.h"

int main(const int argc, const char** argv) {
    int fd = syscall_open_named_pipe(argv[1]);
    if (fd < 0) {
        printf("Failed to open named pipe: %d\n", fd);
        return 1;
    }
    const char *content = argv[2];
    const int content_size = strlen(content) + 1;
    syscall_object_write(fd, content, content_size, 0);
    printf("wrote: %s\n", content);
    syscall_object_close(fd);
    return 0;
}
