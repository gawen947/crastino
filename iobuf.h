/* File: iobuf.h

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

#define IOBUF_SIZE 65536

/* TODO: Define OFF_T_MIN and OFF_T_MAX. */

#define MIN_LSEEK_OFFSET OFF_T_MIN + IOBUF_SIZE
#define MAX_LSEEK_OFFSET OFF_T_MAX - IOBUF_SIZE

#ifndef __FreeBSD__
# define MIN_LSEEK64_OFFSET OFF64_T_MIN + IOBUF_SIZE
# define MAX_LSEEK64_OFFSET OFF64_T_MAX - IOBUF_SIZE
#endif /* __FreeBSD__ */

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
   associated with the file argument to the argument offset according to the
   directive whence. For details see lseek(). There are however two differences
   from the original lseek call. First when using SEEK_CUR directive whence the
   function may return zero instead of the absolute location offset to indicate
   that one lseek syscall has been spared. Second the offset when using the
   SEEK_CUR directive whence must be comprised between two constants
   MIN_LSEEK_OFFSET and MAX_LSEEK_OFFSET. These values are large enough so the
   user may ensure that every seek will be comprised in this interval. */
off_t iobuf_lseek(iofile_t file, off_t offset, int whence);

#ifndef __FreeBSD__
/* The iobuf_lseek64() function repositions the offset of the open stream
   associated with the file argument to the argument offset according to the
   directive whence. For details see lseek64(). There are however two
   differences from the original lseek call. First when using SEEK_CUR directive
   whence the function may return zero instead of the absolute location offset
   to indicate that one lseek syscall has been spared. Second the offset when
   using the SEEK_CUR directive whence must be comprised between two constants
   MIN_LSEEK64_OFFSET and MAX_LSEEK64_OFFSET. These values are large enough so
   the user may ensure that every seek will be comprised in this interval. */
off64_t iobuf_lseek64(iofile_t file, off64_t offset, int whence);
#endif /* __FreeBSD__ */

#endif /* _IOBUF_H_ */
