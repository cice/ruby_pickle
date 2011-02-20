#include "c_stack.h"

void handleError() {
  exit(1);
}

void push(Stack *stack, VALUE e) {
	stack->list_node = ALLOC(StackNode);
	
	if(stack->list_node == NULL)
	{
		puts("Error : Cannot Allocate Memory.");
    handleError();
	}
	
	stack->list_node->data = e;
	stack->list_node->link = stack->head;
	stack->head = stack->list_node;
}
 
VALUE pop(Stack *stack) {
	if(stack->head == NULL) {
    return Qnil;
	}
	
	VALUE tmp = stack->head->data;
	stack->list_node = stack->head;
	stack->head = stack->head->link;
	free(stack->list_node);
	
	return tmp;
}

VALUE peek(Stack *stack) {
	if(stack->head == NULL) {
    return Qnil;
	}

  VALUE tmp = stack->head->data;
  return tmp;
}
 
void init_stack(Stack *stack) {
  stack->head = NULL;
  stack->list_node = NULL;
}

void free_node(StackNode *node) {
  if(node->link != NULL) {
    free_node(node->link);
  }
  free(node->link);
}

void free_stack(Stack *stack) {
  free_node(stack->head);
  free(stack);
}