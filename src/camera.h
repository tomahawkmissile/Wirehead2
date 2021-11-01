#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#include "util/strings.h"
#include "util/universal_structs.h"

enum io_method {
	IO_METHOD_READ,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR,
};

struct buffer {
	void   *start;
	size_t  length;
	int screen_width,screen_height;
	bool ready;
};

#define CLEAR(x) memset(&(x), 0, sizeof(x))

bool camera_buf_rdy();
struct buffer camera_buf_get_last();
struct buffer camera_buf_get_previous();

void* run_camera(void* input);

#endif