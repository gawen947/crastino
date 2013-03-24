/* File: task.h

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

#ifndef _TASK_H_
#define _TASK_H_

#include <stdbool.h>
#include <time.h>

/* This structure represents a task in its simpliest form. */
struct task {
  const char *title;   /* short description of the task */
  const char *message; /* full description of the task */

  time_t ctime;        /* date of creation */
  time_t mtime;        /* date of last modification */
  time_t dtime;        /* last date this task was marked as done */

  bool recursive;      /* marked as a recursive task */
};

/* Create a task structure with propely time. The structure has to be freed
   manually. The passed should be freed manually too to avoid useless string
   duplication. */
struct task * creat_task(const char *title, const char *message,
                         bool recursive);

/* Modify a task and setup the time accordingly. */
void modify_task(struct task *task, const char *title, const char *message);

/* Mark the task as done and setup the time accordingly.
   The task should be a recursive task. */
void mark_as_done(struct task *task);

#endif
