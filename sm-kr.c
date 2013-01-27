/* File: sm-kr.c
   Time-stamp: <2013-01-27 04:09:31 gawen>

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
#include <stdint.h>

#include "sm-kr.h"

struct kr {
  const char *pattern;
  const char *text;
  uint32_t hash_pattern;
  uint32_t hash_text;

  unsigned int len;
  unsigned int index;
};

kr_t kr_create(const char *pattern)
{
  const char *s;
  uint32_t hash = 0;
  uint32_t len  = 0;

  struct kr *kr = malloc(sizeof(struct kr));

  if(!kr)
    return NULL;

  for(s = pattern ; *s ; s++) {
    hash = (hash << 1) + *s;
    len++;
  }

  kr->pattern      = pattern;
  kr->hash_pattern = hash;
  kr->len          = len - 1;

  return kr;
}

static bool match(const char *pattern, const char *text)
{
  const char *p, *t;

  for(p = pattern, t = text ; *p == *t && *p != '\0' ; p++, t++);

  if(*p != *t && *p != '\0')
    return false;

  return true;
}

bool kr_match(kr_t kr, const char *text, size_t size)
{
  unsigned int i;
  unsigned int len = kr->len;
  uint32_t hash    = 0;

  for(i = 0 ; kr->pattern[i] != '\0' && text[i] != '\0' ; i++)
    hash = (hash << 1) + text[i];

  if(hash == kr->hash_pattern && match(kr->pattern, text))
    return true;

  for(i = 1 ; i < size - len ; i++) {
    hash -= (text[i-1] << len);
    hash  = (hash << 1) + text[i+len];

    if(hash == kr->hash_pattern && match(kr->pattern, text + i))
      return true;
  }

  return false;
}

int kr_matchall(kr_t kr, const char *text, size_t size)
{
  unsigned int i;
  unsigned int len;
  uint32_t     hash;

  if(text) {
    hash = 0;

    for(i = 0 ; kr->pattern[i] != '\0' && text[i] != '\0' ; i++)
      hash = (hash << 1) + text[i];

    kr->text      = text;
    kr->hash_text = hash;
    kr->index     = 1;

    if(hash == kr->hash_pattern && match(kr->pattern, text))
      return 0;
  }

  text = kr->text;
  len  = kr->len;
  hash = kr->hash_text;

  for(i = kr->index ; i < size - len ; i++) {
    hash -= (text[i-1] << len);
    hash  = (hash << 1) + text[i+len];

    if(hash == kr->hash_pattern && match(kr->pattern, text + i)) {
      kr->hash_text = hash;
      kr->index     = i+1;
      return i;
    }
  }

  return -1;
}

void kr_destroy(kr_t kr)
{
  free(kr);
}
