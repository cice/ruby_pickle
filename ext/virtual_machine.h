#include "c_stack.h"

#define MEMOSIZE 1024

typedef struct{
  Stack *stack;
  VALUE *memo;
} VirtualMachine;

void init_vm(VirtualMachine *vm);
void free_vm(VirtualMachine *vm);

VALUE vmpop(VirtualMachine *vm);
VALUE vmpeek(VirtualMachine *vm);
void vmpush(VirtualMachine *vm, VALUE value);

void vmput(VirtualMachine *vm, int pos);
VALUE vmget(VirtualMachine *vm, int pos);