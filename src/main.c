#include "cpu.h"
#include <stdio.h>

int main() {
  printf("%d\n", 0 == 0);
  printf("%d\n", 0 != 0);

  // main function
  struct zeus_cpu cpu;
  zeus_write16(&cpu.bus.region.rst_handler, 0);
  zeus_reset(&cpu);
  cpu.registers.c = 1;
  cpu.bus.raw[0] = 0x69; // adc imm
  cpu.bus.raw[1] = 0x10; // imm operand

  zeus_exec(&cpu);
  printf("a: %#x\n", cpu.registers.a);
  printf("ok\n");

  return 0;
}
