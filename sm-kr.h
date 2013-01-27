/* File: sm-kr.h
   Time-stamp: <2013-01-27 16:08:09 gawen>

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

typedef struct kr * kr_t;

/* Create the string matching context. The pattern argument will be matched
   against the proposed strings. This pattern is not duplicated and should not
   be freed until the string matching context is destroyed. The string matching
   is done using the Karp-Rabin algorithm (1987). */
kr_t kr_create(const char *pattern);

/* Search for one occurence of the pattern in a text. The search is abandoned as
   soon as one occurence is found. The size of the text must be passed in
   argument. */
bool kr_match(kr_t kr, const char *text, size_t size);

/* Search for all occurences of the pattern in a text. The function will return
   the index of the match in the text or less than zero if the pattern was not
   found. Subsequent calls for next occurences on the same text must be called
   will NULL as the text argument. The size of the text must be passed in
   argument. */
int kr_matchall(kr_t kr, const char *text, size_t size);

/* Destroy the string matching context. */
void kr_destroy(kr_t kr);
