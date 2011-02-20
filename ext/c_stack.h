#include <stdio.h>
#include <stdlib.h>
#include <ruby.h>

struct node{
	VALUE data;
  struct node *link;
};
typedef struct node StackNode;
 
typedef struct{
	StackNode *head;
	StackNode *list_node;
} Stack;
// typedef struct stack Stack;

void push(Stack *stack, VALUE e);
VALUE pop(Stack *stack);
VALUE peek(Stack *stack); 

void init_stack(Stack *stack);
void free_stack(Stack *stack);