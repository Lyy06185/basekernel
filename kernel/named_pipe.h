#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include "pipe.h"
#include "fs.h"

struct named_pipe
{
	struct pipe *p;
	int refcount;
};

int named_pipe_create(const char *fname);
int named_pipe_destroy(const char *fname);
struct named_pipe *named_pipe_open(struct fs_dirent *f);
int named_pipe_size(struct named_pipe *np);
void named_pipe_flush(struct named_pipe *np);
void named_pipe_close(struct named_pipe *np);
void named_pipe_delete(struct named_pipe *np);
struct named_pipe *named_pipe_addref(struct named_pipe *np);
int named_pipe_read(struct named_pipe *np, char *buffer, int size);
int named_pipe_read_nonblock(struct named_pipe *np, char *buffer, int size);
int named_pipe_write(struct named_pipe *np, char *buffer, int size);
int named_pipe_write_nonblock(struct named_pipe *np, char *buffer, int size);

#endif