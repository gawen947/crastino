/* File: htable.c

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

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "htable.h"

#define IDX(hash, size) hash & (size - 1)

struct entry {
  const void *key;
  void *data;

  struct entry *next;
};

struct htable {
  uint32_t (*hash)(const void *);
  bool (*compare)(const void *, const void *);
  void (*destroy)(void *);

  unsigned int nbuckets;

  struct entry *buckets[];
};

htable_t ht_create(unsigned int nbuckets,
                   uint32_t (*hash)(const void *),
                   bool (*compare)(const void *, const void *),
                   void (*destroy)(void *))
{
  struct htable *ht = malloc(sizeof(struct htable) +            \
                             sizeof(struct entry) * nbuckets);

  if(!ht)
    return NULL;
  memset(ht, 0, sizeof(struct htable) + sizeof(struct entry *) * nbuckets);

  ht->nbuckets = nbuckets;

  ht->hash    = hash;
  ht->compare = compare;
  ht->destroy = destroy;

  return ht;
}

void * ht_search(htable_t ht, const void *key, void *data)
{
  struct entry *entry;
  uint32_t index    = IDX(ht->hash(key), ht->nbuckets);

  for(entry = ht->buckets[index] ; entry ; entry = entry->next) {
    if(ht->compare(entry->key, key)) {
      if(data) {
        ht->destroy(entry->data);
        entry->key  = key;
        entry->data = data;
      }

      return entry->data;
    }
  }

  if(data) {
    struct entry *new = malloc(sizeof(struct entry));

    new->key  = key;
    new->data = data;
    new->next = ht->buckets[index];

    ht->buckets[index] = new;

    return data;
  }

  return NULL;
}

void * ht_lookup(htable_t ht, const void *key,
                 void * (*retrieve)(const void *, void *),
                 void *optarg)
{
  struct entry *entry;
  uint32_t index    = IDX(ht->hash(key), ht->nbuckets);

  for(entry = ht->buckets[index] ; entry ; entry = entry->next)
    if(ht->compare(entry->key, key))
      return entry->data;

  entry = malloc(sizeof(struct entry));
  entry->key  = key;
  entry->data = retrieve(key, optarg);
  entry->next = ht->buckets[index];

  ht->buckets[index] = entry;

  return entry->data;
}

void ht_walk(htable_t ht, void (*action)(void *))
{
  unsigned int i;

  for(i = 0 ; i < ht->nbuckets ; i++) {
    struct entry *entry;

    for(entry = ht->buckets[i] ; entry ; entry = entry->next)
      action(entry->data);
  }
}

void ht_delete(htable_t ht, const void *key)
{
  struct entry *entry;
  struct entry *prev = NULL;
  uint32_t index     = IDX(ht->hash(key), ht->nbuckets);

  for(entry = ht->buckets[index], prev = NULL ;
      entry ;
      prev = entry, entry = entry->next)
    if(ht->compare(entry->key, key))
      break;

  if(!entry)
    return;

  if(!prev)
    ht->buckets[index] = entry->next;
  else
    prev->next = entry->next;

  ht->destroy(entry->data);
  free(entry);
}

void ht_destroy(htable_t ht)
{
  unsigned int i;

  for(i = 0 ; i < ht->nbuckets ; i++) {
    struct entry *entry = ht->buckets[i];

    while(entry) {
      struct entry *f = entry;

      entry = entry->next;
      ht->destroy(f->data);
      free(f);
    }
  }

  free(ht);
}

