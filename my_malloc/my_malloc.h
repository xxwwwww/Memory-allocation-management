

#ifndef MY_MALLOC_FUNCTION_H
#define MY_MALLOC_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct Node_{
    size_t size;
    struct Node_ *next;
    struct Node_ *prev;
} Node;

void * bf_malloc(size_t size);
void * ff_malloc(size_t size);
void ff_free(void *ptr);
void bf_free(void *ptr);

void *split(Node * curr,size_t size);
void add(Node * freenode);
void my_free(void *ptr);
void *remove_node(Node * curr);
void merge(Node *block);
void check_whether_to_merge(Node * first, Node * sec);

unsigned long get_data_segment_free_space_size();
unsigned long get_data_segment_size();

Node * head = NULL;
Node * tail = NULL;
#endif //MY_MALLOC_FUNCTION_H
