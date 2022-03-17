//
// Created by Yitong Wang on 1/19/22.
//

#include "my_malloc.h"
#include "assert.h"

unsigned long data_segment_size=0;

size_t node_size = sizeof(Node);


void *ff_malloc(size_t size) {
    Node * curr = head;
    while(curr){
         if(curr->size == size || (curr->size >size && curr->size < size + node_size)){
            return remove_node(curr);
        }
         if(curr->size > size + node_size){
             return split(curr,size);
         }
        curr = curr->next;
    }
    void *ptr = sbrk( size + node_size);
    Node *block = (Node *)ptr;
    block->prev = NULL;
    block->next = NULL;
    block->size = size;
    data_segment_size += size + node_size;
    return (void *)block + node_size;
}

void *remove_node(Node * curr){
    if (curr->prev != NULL && curr->next !=NULL) {           //remove node in the middle
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }
    else if(curr->prev == NULL && curr->next !=NULL){           //remove head
        head = curr->next;
        curr->next->prev = NULL;

    }
    else if(curr->prev != NULL && curr->next ==NULL){           //remove tail
        tail =curr->prev;
        curr->prev->next = NULL;
    }
    else{
        head = NULL;
        tail = NULL;
    }
    curr->next = NULL;
    curr->prev = NULL;
    return (void *) curr + node_size;
}
void *split(Node * curr, size_t size){
    assert(curr->size > size + node_size);
    Node * redundant = (Node *) ((void*)curr + node_size +size);
    redundant->size = curr->size - size - node_size;
    redundant->prev = curr->prev;
    redundant->next = curr->next;
    curr->size =size;
    curr->next =redundant;
    if(redundant->next) {
        redundant->next->prev = redundant;
    }
    return remove_node(curr);
}

void ff_free(void *ptr){my_free(ptr);}
void bf_free(void *ptr){my_free(ptr);}


void my_free(void * ptr){
    if (ptr == NULL)
        return;
    Node * freenode = (Node*)(ptr - node_size);
    add(freenode);
  //  printList();
    merge(freenode);
}

void add(Node * freenode){
    if(head == NULL && tail == NULL){
        head = freenode;
        tail = freenode;
    }
    else{
        Node * curr =head;
        while(curr != NULL){
            if(curr == head && curr > freenode){     //add to the front
                freenode->next = curr;
                freenode->prev = NULL;
                curr->prev =freenode;
                head = freenode;
                break;
            }
            else if(curr > freenode ){            //add in the middle
                Node * pre = curr->prev;
                pre->next = freenode;
                curr->prev = freenode;
                freenode->next = curr;
                freenode->prev = pre;
                break;
            }
            else if (curr->next == NULL){         //add at the end
                freenode->prev = curr;
                freenode->next = NULL;
                curr->next = freenode;
                tail = freenode;
                break;
            }
            curr = curr->next;
        }
    }
}

//void merge(Node * freenode) {
//    Node *pre = freenode->prev;
//    Node *net = freenode->next;
//    if (net != NULL) {
//        if (((void *) freenode) + freenode->size + node_size == (void *) net) {
//            freenode->size = net->size + node_size + freenode->size;
//            freenode->next = net->next;
//            if (net->next) {
//                net->next->prev = freenode;
//            } else {
//                tail = freenode;
//            }
//        }
//    }
//    if (pre != NULL) {
//        if ( ((void *) pre) + pre->size + node_size  == (void *) freenode) {
//            pre->size = pre->size + node_size + freenode->size;
//            pre->next = freenode->next;
//            if (freenode->next) {
//                freenode->next->prev = pre;
//            } else {
//                tail = pre;
//            }
//        }
//    }
//}

void merge(Node * freenode){
    Node *pre = freenode->prev;
    Node *net = freenode->next;
    if(pre != NULL ){
        check_whether_to_merge(pre,freenode);
    }
    if(net != NULL){
        check_whether_to_merge(freenode,net);
    }
}

void check_whether_to_merge(Node * first, Node * sec){
    void * end_of_first = (void*)first + first->size +node_size;
    void * begin_of_sec = (void*) sec;
    if(end_of_first == begin_of_sec){
        first->size = sec->size + node_size + first->size;
        if(sec->next==NULL){
            first->next =NULL;
            tail = first;
        }
        else{
            first->next = sec->next;
        }
    }

}




void *bf_malloc(size_t size) {
    Node *curr = head;
    int bf = 0;
    while (curr ) {
        if (curr->size == size || (curr->size > size && curr->size < size + node_size)) {
            bf = 1;
            break;
        }
        else if (curr->size > size + node_size) {
            bf = 2;
        }
        curr = curr->next;
    }
    if (bf == 1) {
        return remove_node(curr);
    }
    if (bf == 2) {
        curr = head;
        while(curr){
            if (curr->size > size + node_size){
                return split(curr,size);
            }
            curr = curr->next;
        }
    }
    void *ptr = sbrk(size + node_size);
    Node *block = (Node *) ptr;
    block->prev = NULL;
    block->next = NULL;
    block->size = size;
    data_segment_size += size + node_size;
    return (void *) block + node_size;
}

//void printList() {
//    Node *traversal = head;
//    printf("Print begin\n");
//    while (traversal != NULL) {
//        printf("curr: %p, allocated size: %lu, allocated address: %p\n", traversal, traversal->size,(void *)traversal+node_size);
//        traversal = traversal->next;
//    }
//}



unsigned long get_data_segment_free_space_size(){
    unsigned long data_segment_free_space_size = 0;
    Node * curr = head;
    while(curr!= NULL){
        data_segment_free_space_size += node_size + curr->size;
        curr = curr->next;
    }
    return data_segment_free_space_size;
}


unsigned long get_data_segment_size(){
    return data_segment_size;
}
