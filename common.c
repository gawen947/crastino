/* File: common.c

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
#include <err.h>

#include "common.h"

#define SAFE_CALL0(name, erron, msg, ret)       \
  ret x ## name () {                            \
    register ret t = name ();                   \
    if(t erron)                                 \
      err(EXIT_FAILURE, msg);                   \
    return t; }

#define SAFE_CALL1(name, erron, msg, ret, type) \
  ret x ## name (type arg) {                    \
    register ret t = name (arg);                \
    if(t erron)                                 \
      err(EXIT_FAILURE, msg);                   \
    return t; }

#define SAFE_CALL2(name, erron, msg, ret, type1, type2) \
  ret x ## name (type1 arg1, type2 arg2) {              \
    register ret t = name (arg1, arg2);                 \
    if(t erron)                                         \
      err(EXIT_FAILURE, msg);                           \
    return t; }

#define SAFE_CALL3(name, erron, msg, ret, type1, type2, type3) \
  ret x ## name (type1 arg1, type2 arg2, type3 arg3) {         \
    register ret t = name (arg1, arg2, arg3);                  \
    if(t erron)                                                \
      err(EXIT_FAILURE, msg);                                  \
    return t; }

SAFE_CALL1(malloc, == NULL, "out of memory", void *, size_t)

