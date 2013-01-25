/* File: htable.h
   Time-stamp: <2013-01-25 03:24:44 gawen>

   Copyright (C) 2011-2013 David Hauweele <david@hauweele.net>

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

#ifndef _HTABLE_H_
#define _HTABLE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct htable * htable_t;

/* Create a new hash table. The number of buckets should
   be a power of two. The hash function take the key
   and return a 32 bit hash. Comparison is done on key and
   return true if they are equals, false otherwise. And the
   last function destroy data when necessary. It should be
   used to destroy the key too as long as it is stored
   with the data (for example inside a structure). Not useful
   though when the key is only an integer. */
htable_t ht_create(unsigned int nbuckets,
                   uint32_t (*hash)(const void *),
                   bool (*compare)(const void *, const void *),
                   void (*destroy)(void *));

/* Search, insert or replace an entry inside the hash table.
   If data is NULL then this function will return the data
   associated to the entry if found in the hash table or NULL
   otherwise. If data is not NULL then this function will
   replace the entry with the new data and key if found inside
   the hash table. Otherwise it will simply insert the data
   inside the hash table. With a non NULL data argument this
   function will always return the specified data. */
void * ht_search(htable_t htable, const void *key, void *data);

/* Lookup inside the hash table for an entry specified by key.
   If this entry is found, this function simply return the
   associated data. Otherwise it will use the retrieve function
   to insert a new entry with its return value as data and return
   this data. This retrieve function will be passed the key as its
   first argument and the optarg as its second argument. The key
   point with this function is that is that it will not call the
   retrieve function as long as the requested entry is already
   inside the hash table. */
void * ht_lookup(htable_t htable, const void *key,
                 void * (*retrieve)(const void *, void *),
                 void *optarg);

/* Walk through the hash table and apply the action function
   on each entry, passing data to this function. */
void ht_walk(htable_t htable, void (*action)(void *));

/* Delete the entry specified by key from the hash table.
   The destroy function used at creation is used to destroy
   the data and the key if necessary. */
void ht_delete(htable_t htable, const void *key);

/* Destroy each entry from the hash table and then destroy
   the hash table itself. */
void ht_destroy(htable_t htable);

#endif /* _HTABLE_H_ */
