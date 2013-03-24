/* File: bsort.c

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

#include "bsort.h"

struct node {
  void *data;

  struct node *left;
  struct node *right;
};

struct bsort {
  int  (*compare)(const void *, const void *);
  void (*destroy)(void *);

  struct node *root;
};

bsort_t bsort_create(int  (*compare)(const void *, const void *),
                 void (*destroy)(void *))
{
  struct bsort *bsort = malloc(sizeof(struct bsort));

  if(!bsort)
    return NULL;

  bsort->compare = compare;
  bsort->destroy = destroy;
  bsort->root    = 0;

  return bsort;
}

void * bsort_insert(bsort_t bsort, void *data)
{
  struct node **ref = &bsort->root;
  struct node *node = bsort->root;

  while(node) {
    int comparison = bsort->compare(data, node->data);

    if(comparison < 0) {
      ref  = &node->left;
      node = node->left;
    }
    else if(comparison > 0) {
      ref  = &node->right;
      node = node->right;
    }
    else {
      bsort->destroy(node->data);
      node->data = data;
      return data;
    }
  }

  node = malloc(sizeof(struct node));

  if(!node)
    return NULL;

  node->left  = NULL;
  node->right = NULL;
  node->data  = data;

  *ref = node;

  return data;
}

void * bsort_search(bsort_t bsort, void *data)
{
  struct node *node = bsort->root;

  while(node) {
    int comparison = bsort->compare(data, node->data);

    if(comparison < 0)
      node = node->left;
    else if(comparison > 0)
      node = node->right;
    else
      return node->data;
  }

  return NULL;
}

void bsort_delete(bsort_t bsort, void *data)
{
  struct node **ref = &bsort->root;
  struct node *node = bsort->root;

  while(node) {
    int comparison = bsort->compare(data, node->data);

    if(comparison < 0) {
      ref  = &node->left;
      node = node->left;
    }
    else if(comparison > 0) {
      ref  = &node->right;
      node = node->right;
    }
    else
      goto found;
  }

  return;

found:
  bsort->destroy(node->data);

  if(node->left && node->right) {
    struct node *root = node;
    node = node->right;
    while(node->left) {
      ref  = &node->left;
      node = node->left;
    }

    root->data = node->data;
    *ref       = node->right;
    free(node);
  }
  else if(node->left) {
    *ref = node->left;
    free(node);
  }
  else {
    *ref = node->right;
    free(node);
  }
}

static void inorder(struct node *node, void (*action)(void *data))
{
  if(!node)
    return;

  inorder(node->left, action);
  action(node->data);
  inorder(node->right, action);
}

void bsort_walk(bsort_t bsort, void (*action)(void *data))
{
  inorder(bsort->root, action);
}

static void destroy(bsort_t bsort, struct node *node)
{
  if(!node)
    return;

  destroy(bsort, node->left);
  destroy(bsort, node->right);

  bsort->destroy(node->data);
  free(node);
}

void bsort_destroy(bsort_t bsort)
{
  destroy(bsort, bsort->root);
  free(bsort);
}
