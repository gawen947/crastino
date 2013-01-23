/* File: path.h
   Time-stamp: <2013-01-23 03:28:50 gawen>

   Copyright (C) 2013 David Hauweele <david@hauweele.net>

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

#ifndef _PATH_H_
#define _PATH_H_

#include <stdbool.h>

/* Initialize the different paths.
   This does not check if those path exist but
   merely allocate memory for them. */
void init_path(void);

/* Return the configuration path.
   The returned string should not be modified.
   This function is non-reentrant. */
const char * configuration_path(void);

/* Append the specified file to the configuration path.
   The returned string should not be modified.
   This function is non-reentrant. */
const char * configuration_file(const char * file);

/* Free the memory that was allocated for the paths. */
void free_path(void);

/* Check if a directory is exist and create it otherwise.
   If it isn't accessible for some other reason it will
   report an error an abort execution. */
void creat_directory(const char *dir);

/* Check if a file exists. If it isn't accessible for some
   other reason it will report an error an abort execution. */
bool file_exist(const char *path);

#endif /* _PATH_H_ */
