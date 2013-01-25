/* File: iobuf.h
   Time-stamp: <2013-01-23 18:52:08 gawen>

   Copyright (C) 2012-2013 David Hauweele <david@hauweele.net>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#ifndef _IOBUF_H_
#define _IOBUF_H_

#ifndef __FreeBSD__
# define _LARGEFILE64_SOURCE
#endif /* __FreeBSD__ */

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct iofile * iofile_t;

/* This creates an opened stream from an already opened file descriptor. */
iofile_t iobuf_dopen(int fd);

/* This opens the file whose name is the string pointed to by pathname
   and associates a stream with it. The arguments flags and mode are
   subject to the same semantic that the ones used in open. */
iofile_t iobuf_open(const char *pathname, int flags, mode_t mode);

/* Write up to count bytes from the buffer pointer buf to the stream
   referred to by file. This is done through an user-space buffer in
   order to avoid useless syscall switch to kernel mode. */
ssize_t iobuf_write(iofile_t file, const void *buf, size_t count);

/* Attemps to read up to count bytes from the stream referred to by
   file. This is done through an user-space buffer in order to avoid
   useless syscall switch to kernel mode. */
ssize_t iobuf_read(iofile_t file, void *buf, size_t count);

/* For output streams, iobuf_flush forces a write of all user-space
   buffered data for the given output. As the standard fflush function
   the kernel buffers are not flushed so you may need to sync manually.
   Unlike the standard fflush function this function does not discards
   the read buffer and only affects the write buffer. */
ssize_t iobuf_flush(iofile_t file);

/* Close a stream. This function also take care of flushing the buffers
   when needed. */
int iobuf_close(iofile_t file);

/* Write a single character to the specified file. */
int iobuf_putc(char c, iofile_t file);

/* The iobuf_lseek() function repositions the offset of the open stream
   associated with the file argument to the argument offset according
   to the directive whence. For details see lseek(). */
off_t iobuf_lseek(iofile_t file, off_t offset, int whence);

#ifndef __FreeBSD__
/* The iobuf_lseek64() function repositions the offset of the open stream
   associated with the file argument to the argument offset according
   to the directive whence. For details see lseek64() */
off64_t iobuf_lseek64(iofile_t file, off64_t offset, int whence);
#endif /* __FreeBSD__ */

#endif /* _IOBUF_H_ */