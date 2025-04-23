#include "named_pipe.h"
#include "kmalloc.h"
#include "process.h"
#include "page.h"
#include "list.h"

#define PIPE_SIZE PAGE_SIZE
#define MAX_NAMED_PIPES 256

// we use list for most of the operations like search
// below is the initialization of the list
static struct named_pipe *named_pipes[MAX_NAMED_PIPES];
static int last_named_pipe_index = 0;
static int named_pipe_count = 0;

// struct named_pipe {
//     const char* fname; // the pipe need a name for search
//     char *buffer;
//     int read_pos;
//     int write_pos;
//     int flushed;
//     int refcount;
//     struct list queue;
// };

struct named_pipe *named_pipe_create(const char* fname) {
    if (named_pipe_count >= MAX_NAMED_PIPES) 
        return 0;
    struct named_pipe *existing = named_pipe_search(fname); 
    if (existing) {
        named_pipe_delete(existing);
        return 0;
    }
    struct named_pipe *np = kmalloc(sizeof(struct named_pipe));
    np->fname = fname;
    np->buffer = page_alloc(1);
    if (!np->buffer) {
        kfree(np);
        return 0;
    } // similar to pipe
    named_pipes[last_named_pipe_index++] = np;
    last_named_pipe_index %= MAX_NAMED_PIPES;
    named_pipe_count++; // add new named pipe to list
    np->read_pos = 0;
    np->write_pos = 0;
    np->flushed = 0;
    np->queue.head = 0;
    np->queue.tail = 0;
    np->refcount = 1;
    return np;
}

struct named_pipe *named_pipe_addref(struct named_pipe *np) {
    np->refcount++;
    return np;
}

void named_pipe_flush(struct named_pipe *np) {
    if (np) {
        np->flushed = 1;
        process_wakeup_all(&np->queue); // wake up all processes waiting on this named pipe to find out it's flushed.
    }
}


void named_pipe_delete(struct named_pipe *np) {
    if (!np) 
        return;
    np->refcount--;
    if (np->refcount == 0) {
        for (int i = 0; i < MAX_NAMED_PIPES; i++) // same, use list for operation with O(n) complexity
            if (named_pipes[i] == np) {
                named_pipes[i] = 0;
                break;
            }
        if (np->buffer) page_free(np->buffer);
        kfree(np);
        named_pipe_count--;
    }
}

struct named_pipe *named_pipe_search(const char* fname) {
    for (int i = 0; i < MAX_NAMED_PIPES; i++) 
        // if (named_pipes[i] && named_pipes[i]->fname == fname)
        if (named_pipes[i] && strcmp(named_pipes[i]->fname, fname) == 0)
            return named_pipe_addref(named_pipes[i]);
    return 0;
} // use a list tracersal to find the named pipe through its name, O(n)

static int named_pipe_write_internal(struct named_pipe *np, char *buffer, int size, int blocking) {
    if (!np || !buffer) 
        return -1;
    if (np->flushed) 
        return 0;
    int written = 0;
    while (written < size) {
        if (np->read_pos == (np->write_pos + 1) % PIPE_SIZE) {
            if (!blocking) return written;
            if (np->flushed) {
                np->flushed = 0;
                return written;
            }
            process_wait(&np->queue);
            continue;
        }
        np->buffer[np->write_pos++] = buffer[written++];
        np->write_pos %= PIPE_SIZE;
    }
    if (blocking) process_wakeup_all(&np->queue);
    np->flushed = 0;
    return written;
}

static int named_pipe_read_internal(struct named_pipe *np, char *buffer, int size, int blocking) {
    if (!np || !buffer) 
        return -1;
    if (np->flushed) 
        return 0;
    int read = 0;
    while (read < size) {
        np->read_pos %= PIPE_SIZE;
        if (np->read_pos == np->write_pos) {
            if (!blocking) return read;
            if (np->flushed) {
                np->flushed = 0;
                return read;
            }
            process_wait(&np->queue);
            continue;
        }
        buffer[read++] = np->buffer[np->read_pos++];
    }
    if (blocking) process_wakeup_all(&np->queue);
    np->flushed = 0;
    return read;
}

int named_pipe_read(struct named_pipe *np, char *buffer, int size) {
    return named_pipe_read_internal(np, buffer, size, 1);
}

int named_pipe_read_nonblock(struct named_pipe *np, char *buffer, int size) {
    return named_pipe_read_internal(np, buffer, size, 0);
}

int named_pipe_write(struct named_pipe *np, char *buffer, int size) {
    return named_pipe_write_internal(np, buffer, size, 1);
}

int named_pipe_write_nonblock(struct named_pipe *np, char *buffer, int size) {
    return named_pipe_write_internal(np, buffer, size, 0);
} // the above code basicly the same logic with pipe

int named_pipe_size(struct named_pipe *np) {
    if (!np) 
        return -1;
    return (np->write_pos - np->read_pos) % PIPE_SIZE;
}

