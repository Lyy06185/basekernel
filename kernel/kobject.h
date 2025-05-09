/*
 * Copyright (C) 2016-2019 The University of Notre Dame This software is
 * distributed under the GNU General Public License. See the file LICENSE
 * for details.
 */

#ifndef KOBJECT_H
#define KOBJECT_H

#include "kernel/types.h"

#include "fs.h"
#include "device.h"
#include "window.h"
#include "console.h"
#include "pipe.h"
#include "named_pipe.h" // we include named_pipe header file in kobject
#include "event.h"

struct kobject {
	union {
		struct device *device;
		struct fs_dirent *file;
		struct fs_dirent *dir;
		struct window *window;
		struct console *console;
		struct pipe *pipe;
		struct named_pipe *named_pipe; // add named_pipe
	} data;
	kobject_type_t type;
	int refcount;
	int offset;
	char *tag;
};

struct kobject *kobject_create_file(struct fs_dirent *f);
struct kobject *kobject_create_dir(struct fs_dirent *d);
struct kobject *kobject_create_device(struct device *d);
struct kobject *kobject_create_window(struct window *g);
struct kobject *kobject_create_console(struct console *c);
struct kobject *kobject_create_pipe(struct pipe *p);
struct kobject *kobject_create_named_pipe(struct named_pipe *np); // added named_pipe operation accordingly
struct kobject *kobject_create_event();

struct kobject *kobject_create_window_from_window( struct kobject *k, int x, int y, int w, int h );
struct kobject *kobject_create_console_from_window( struct kobject *k );
struct kobject *kobject_create_dir_from_dir( struct kobject *kobject, const char *name );
struct kobject *kobject_create_file_from_dir( struct kobject *kobject, const char *name );

struct kobject *kobject_addref(struct kobject *k);

struct kobject * kobject_copy( struct kobject *ksrc );

int kobject_read(struct kobject *kobject, void *buffer, int size, kernel_io_flags_t flags );
int kobject_lookup( struct kobject *kobject, const char *name, struct kobject **newobj );
int kobject_write(struct kobject *kobject, void *buffer, int size, kernel_io_flags_t flags );
int kobject_list( struct kobject *kobject, void *buffer, int size );
int kobject_size(struct kobject *kobject, int *dimensions, int n);
int kobject_remove( struct kobject *kobject, const char *name );
int kobject_close(struct kobject *kobject);

int kobject_get_type(struct kobject *kobject);
int kobject_set_tag(struct kobject *kobject, char *new_tag);
int kobject_get_tag(struct kobject *kobject, char *buffer, int buffer_size);

#endif
