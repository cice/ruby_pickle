#include "virtual_machine.h"

void init_vm(VirtualMachine *vm) {
  vm->stack = ALLOC(Stack);
  init_stack(vm->stack);
  vm->memo = ALLOC_N(VALUE, MEMOSIZE);
}

void free_vm(VirtualMachine *vm) {
  free_stack(vm->stack);
  free(vm->memo);
  free(vm);
}

void vmpush(VirtualMachine *vm, VALUE value) {
  push(vm->stack, value);
}

VALUE vmpop(VirtualMachine *vm) {
  return pop(vm->stack);
}

VALUE vmpeek(VirtualMachine *vm) {
  return peek(vm->stack);
}

void vmput(VirtualMachine *vm, int pos) {
  vm->memo[pos] = peek(vm->stack);
}

VALUE vmget(VirtualMachine *vm, int pos) {
  return vm->memo[pos];
}