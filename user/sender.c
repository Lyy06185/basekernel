#include "library/syscalls.h"

int main()
{
  char *fname = "/data/test_named_pipe";
  int fd = syscall_open_named_pipe(fname);
  if (fd < 0)
  {
      printf("[ERROR] Failed to open named pipe (Error code: %s)\n", strerror(fd));
      return 1;
  }
  printf("=== Successfully opened named pipe at %s ===\n", fname);
  char *msg = "Hello, world!";
  syscall_object_write(fd, msg, 30, 0);
  printf("=== Successfully sent message: %s ===\n", msg);
  return 0;
}