/*
Copyright (C) 2016-2019 The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "kernel/types.h"
#include "kernel/stats.h"

void syscall_debug(const char *str);

/* Syscalls that manipulate this process and its children. */

void syscall_process_exit(int status);
int syscall_process_yield();
int syscall_process_run(int fd, int argc, const char **argv);
int syscall_process_run_with_priority(int fd, int argc, const char **argv, int pri);
int syscall_process_wrun(int fd, int argc, const char **argv,  int * fds, int fd_len);
int syscall_process_run_with_priority(int fd, int argc, const char **argv, int pri); // new syscall: schedule a process with priority
int syscall_process_fork();
int syscall_process_exec(int fd, int argc, const char **argv);
int syscall_process_self();
int syscall_process_pri(); // new syscall: get the current process's priority
int syscall_process_parent();
int syscall_process_kill(unsigned int pid);
int syscall_process_reap(unsigned int pid);
int syscall_process_wait(struct process_info *info, int timeout);
int syscall_process_sleep(unsigned int ms);
int syscall_process_stats(struct process_stats *s, unsigned int pid);
extern void *syscall_process_heap(int a);
int syscall_process_run_blocked(); // new syscall: run the highest priority blocked process
int syscall_process_run_blocked_all(); // new syscall: run all blocked processes

/* Syscalls that open or create new kernel objects for this process. */

int syscall_make_named_pipe(const char *path); // new syscall: create a named pipe
int syscall_destroy_named_pipe(const char *path); // new syscall: destroy a named pipe
int syscall_open_named_pipe(const char *path); // new syscall: open a named pipe
int syscall_open_file(int fd, const char *path, int mode, kernel_flags_t flags);
int syscall_open_dir( int fd, const char *path, kernel_flags_t flags );
int syscall_open_window(int fd, int x, int y, int w, int h);
int syscall_open_console(int fd);
int syscall_open_pipe();

/* Syscalls that manipulate kernel objects for this process. */

int syscall_object_type(int fd);
int syscall_object_copy( int src, int dst );
int syscall_object_read(int fd, void *data, int length, kernel_io_flags_t flags );
int syscall_object_list( int fd, char *buffer, int buffer_len);
int syscall_object_write(int fd, const void *data, int length, kernel_io_flags_t flags );
int syscall_object_seek(int fd, int offset, int whence);
int syscall_object_size(int fd, int * dims, int n);
int syscall_object_remove( int fd, const char *name );
int syscall_object_close(int fd);
int syscall_object_set_tag(int fd, char *tag);
int syscall_object_get_tag(int fd, char *buffer, int buffer_size);
int syscall_object_max();

/* Syscalls that query or affect the whole system state. */

int syscall_system_stats(struct system_stats *s);
int syscall_bcache_stats(struct bcache_stats *s);

int syscall_bcache_flush();

int syscall_system_time( uint32_t *t );
int syscall_system_rtc( struct rtc_time *t );

int syscall_device_driver_stats(char * name, struct device_driver_stats * stats);

#endif
