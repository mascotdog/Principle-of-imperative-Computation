#pragma once
#include "peg-client.h"
/* Stacks
 * 15-122 Principles of Imperative Computation, Spring 2011
 * Frank Pfenning
 */

 /* Client-side interface */
 // typedef ______ stackelem;

 /* Interface to stacks of items */

typedef struct stack_header* stack;

bool stack_empty(stack S);       /* O(1) */
stack stack_new();               /* O(1) */
void push(stack S, stackelem e); /* O(1) */
stackelem pop(stack S)           /* O(1) */
//@requires !stack_empty(S);
;
int stack_size(stack S);         /* O(1) */

/* Implementation of stacks */

/* Aux structure of linked lists */
struct slist_node {
    stackelem data;
    struct slist_node* next;
};
typedef struct slist_node slist;

/* is_ssegment(start, end) will diverge if list is circular! */
bool is_ssegment(slist* start, slist* end) {
    slist* p = start;
    while (p != end) {
        if (p == NULL) return false;
        p = p->next;
    }
    return true;
}

int slist_size(slist* start, slist* end)
//@requires is_ssegment(start, end);
{
    slist* p = start;
    int n = 0;
    while (p != end)
        //@loop_invariant is_ssegment(p, end);
    {
        //@assert p != NULL;
        n = n + 1;
        p = p->next;
    }
    return n;
}

/* Stacks */

struct stack_header {
    slist* top;
    slist* bottom;
    int size;
};

bool is_stack(stack S) {
    if (S == NULL) return false;
    if (S->top == NULL || S->bottom == NULL) return false;
    if (!is_ssegment(S->top, S->bottom)) return false;
    if (S->size != slist_size(S->top, S->bottom)) return false;
    return true;
}

int stack_size(stack S)
//@requires is_stack(S);
{
    return S->size;
}

bool stack_empty(stack S)
//@requires is_stack(S);
{
    return S->top == S->bottom;
}

stack stack_new()
//@ensures is_stack(\result);
//@ensures stack_empty(\result);
{
    stack S = (stack)malloc(sizeof stack_header);
    slist* p = (slist*) malloc(sizeof(slist_node)); /* does not need to be initialized! */
    S->top = p;
    S->bottom = p;
    S->size = 0;
    return S;
}

//void stack_delete(stack S)
//{
//    free(S);
//    S = NULL;
//}

void push(stack S, stackelem e)
//@requires is_stack(S);
//@ensures is_stack(S);
{
    slist* p = (slist*)malloc(sizeof slist_node);
    p->data = e;
    p->next = S->top;
    S->top = p;
    (S->size)++;
}

stackelem pop(stack S)
//@requires is_stack(S);
//@requires !stack_empty(S);
//@ensures is_stack(S);
{
    stackelem e = S->top->data;
    slist* p_delete = S->top;
    S->top = S->top->next;    
    free(p_delete);
    p_delete = NULL;
    (S->size)--;
    return e;
}
