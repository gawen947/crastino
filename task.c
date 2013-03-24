/* File: task.c

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

#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "common.h"
#include "task.h"

struct task * creat_task(const char *title, const char *message, bool recursive)
{
  struct task *task = xmalloc(sizeof(struct task));
  time_t now = time(NULL);
  task->ctime = task->mtime = task->dtime = now;
  task->title     = title;
  task->message   = message;
  task->recursive = recursive;

  return task;
}

void modify_task(struct task *task, const char *title, const char *message)
{
  task->title   = title;
  task->message = message;
  task->mtime   = time(NULL);
}

void mark_as_done(struct task *task)
{
  /* The task should be recursive. Non-recursive task are
     deleted when marked as done. */
  assert(task->recursive);

  task->dtime = time(NULL);
}
