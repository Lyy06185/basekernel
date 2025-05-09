#include "named_pipe.h"
#include "diskfs.h"
#include "fs.h"
#include "fs_internal.h"
#include "pipe.h"

int named_pipe_create(const char *fname)
{
	// create a file which will point to named_pipe
	struct fs_dirent *f = fs_mkfile(fname);

	if (!f)
	{
		return 0;
	}
	struct named_pipe *np = kmalloc(sizeof(*np));
	if (!np)
	{
		fs_dirent_close(f);
		return 0;
	}
	// create a pipe for named_pipe
	np->p = pipe_create();
	np->refcount = 0;

	// connect the inode of file f with named_pipe
	int res = diskfs_inode_bind_named_pipe(f, np);
	fs_dirent_close(f);

	return res;
}

int named_pipe_destroy(const char *fname)
{
	// delete the file pointing to named_pipe
	int res = fs_remove(fname);
	return res;
}

struct named_pipe *named_pipe_open(struct fs_dirent *f)
{
	// enables processes to access a named pipe via its file system entry
	struct named_pipe *np = diskfs_inode_get_named_pipe(f);
	if (!np)
	{
		return 0;
	}
	np->refcount++;
	return np;
}

int named_pipe_size(struct named_pipe *np)
{
	return pipe_size(np->p);
}

void named_pipe_flush(struct named_pipe *np)
{
	pipe_flush(np->p);
}

void named_pipe_close(struct named_pipe *np)
{
	// manages the closure of a named pipe reference
	if (!np)
		return;
	np->refcount--;
	if (np->refcount == 0)
	{
		pipe_delete(np->p);
		kfree(np);
	}
}

struct named_pipe *named_pipe_addref(struct named_pipe *np)
{
	np->refcount++;
	return np;
}

int named_pipe_read(struct named_pipe *np, char *buffer, int size)
{
	return pipe_read(np->p, buffer, size);
}

int named_pipe_read_nonblock(struct named_pipe *np, char *buffer, int size)
{
	return pipe_read_nonblock(np->p, buffer, size);
}

int named_pipe_write(struct named_pipe *np, char *buffer, int size)
{
	return pipe_write(np->p, buffer, size);
}

int named_pipe_write_nonblock(struct named_pipe *np, char *buffer, int size)
{
	return pipe_write_nonblock(np->p, buffer, size);
}
