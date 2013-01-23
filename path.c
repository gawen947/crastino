/* File: path.c
   Time-stamp: <2013-01-23 03:15:07 gawen>

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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <assert.h>
#include <errno.h>
#include <err.h>

#include "common.h"

/* This additional padding is added to the path to statically append files. This
   is intentionally short to force us to keep using short filenames. */
#define FILE_PADDING 16

/* Relative path to the configuration directory. */
#define CONFPATH "/.config/crastino"

/* Absolute path to the configuration directory. */
static char *confpath;
static char *confpath_separator;

static const char * gethome(void)
{
  const char *home;

  home = getenv("HOME");

  if(!home) {
    struct passwd *pw = getpwuid(getuid());
    home  = pw->pw_dir;
  }

  return home;
}

void init_path(void)
{
  assert(confpath == NULL);

  /* We only initialize the configuration path for now. */
  int size, hsize;
  const char *home = gethome();

  hsize = strlen(home);
  size  = hsize + sizeof(CONFPATH) + FILE_PADDING;
  confpath = xmalloc(size);

  strcpy(confpath, home);
  strcpy(confpath + hsize, CONFPATH);
}

const char * configuration_path(void)
{
  assert(confpath != NULL);

  *confpath_separator = '\0';
  return confpath;
}

const char * configuration_file(const char * file)
{
  assert(confpath != NULL && strlen(file) < FILE_PADDING);

  *confpath_separator = '/';
  return strcpy(confpath_separator + 1, file);
}

void free_path(void)
{
  assert(confpath != NULL);

  free(confpath);
}

void creat_directory(const char *dir)
{
  int r = access(dir, R_OK | W_OK | X_OK);

  if(r < 0) {
    if(errno == ENOENT) {
      r = mkdir(dir, 770);
      if(r < 0)
        err(EXIT_FAILURE, "cannot create \"%s\" directory", dir);
    } else
      err(EXIT_FAILURE, "cannot access \"%s\" directory", dir);
  }
}

bool file_exist(const char *path)
{
  int r = access(path, R_OK | W_OK);

  if(r < 0) {
    if(errno == ENOENT)
      return false;
    else
      err(EXIT_FAILURE, "cannot access \"%s\"", path);
  }

  return true;
}
