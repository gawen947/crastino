/* File: bsort.h

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

#ifndef _BSORT_H_
#define _BSORT_H_

typedef struct bsort * bsort_t;

/* Create a new binary sort tree. The compare function compares two data and
   return an integer less than, equal to, or greater than zero if the first
   argument is found, respectively to be less than, to match, or be greater than
   the second argument. And the last function destroy the data when
   necessary. */
bsort_t bsort_create(int  (*compare)(const void *, const void *),
                 void (*destroy)(void *));

/* Insert the specifid data into the tree. This function return the specified
   data or NULL if it cannot be inuserted. The only reason why it can go wrong
   is an out of memory error on the heap. */
void * bsort_insert(bsort_t bsort, void *data);

/* Search for data inside the tree. This function will return the specified data
   if it was found in the tree. Otherwise it will return NULL. */
void * bsort_search(bsort_t bsort, void *data);

/* In-order walk through the binary sort tree and apply action function on
   each entry, passing data to this function. Beware that this function may
   provoke a stack overflow. */
void bsort_walk(bsort_t bsort, void (*action)(void *));

/* Delete the entry specified from the binary sort tree. The destroy function
   passed at creation is used to destroy the data if necessary. */
void bsort_delete(bsort_t bsort, void *data);

/* Destroy each entry from the binary search sort and then destroy the tree
   itself. Beware that this function may provoke a stack overflow with a very
   deep tree. */
void bsort_destroy(bsort_t bsort);

#endif /* _BSORT_H_ */
