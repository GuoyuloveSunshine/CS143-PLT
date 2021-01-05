#ifndef STACK
#define STACK


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define TOKEN_MAX 16

typedef int Elemtype;
typedef struct cnode{
    Elemtype cell;
    struct cnode *next;
    struct cnode *prev;
} CNode;

// 栈ADT
typedef struct cstack{
    CNode *head;
    CNode *tail;
    unsigned int size;
} CStack;



CStack *cstack_init(){
    CStack *stack = (CStack*)malloc(sizeof(CStack));
    if(!stack){
        perror("init stack error.");
        return NULL;
    }
    stack->size = 0;
    stack->head = stack->tail = NULL;
    return stack;
}

int cstack_push(CStack *cstack, Elemtype cell){
    if(cstack == NULL)
        return 0;
    CNode *node = (CNode*)malloc(sizeof(CNode));
    if(!node)
        return 0;
    node->cell = cell;
    node->next = node->prev = NULL;
    if(cstack->size == 0){
        cstack->head = cstack->tail = node;
        cstack->size++;
        return 1;
    }
    node->prev = cstack->tail;
    cstack->tail->next = node;
    cstack->tail = node;
    cstack->size++;
    return 1;
}

Elemtype cstack_pop(CStack *cstack){
    if(cstack == NULL || cstack->size == 0)
        return -1;
    CNode *node = cstack->tail;
    if(cstack->head == node){
        cstack->head = cstack->tail =  NULL;
    }
    else{
        node->prev->next = NULL;
        cstack->tail = node->prev;
    }
    Elemtype cell = node->cell;
    free(node);
    cstack->size--;
    return cell;
}

int cstack_isempty(CStack *cstack){
    if(cstack->head ==NULL && cstack->tail == NULL) return 1;
    else return 0;
}


Elemtype cstack_gettop(CStack *cstack){
    if(cstack == NULL || cstack->size == 0)
        return -1;
    return cstack->tail->cell;
}

int cstack_clear(CStack *cstack){
    if(cstack == NULL)
        return 0;
    CNode *node = cstack->head;
    while(node){
        CNode *temp = node->next;
        free(node);
        node = temp;
    }
    free(cstack);
    return 1;
}

#endif