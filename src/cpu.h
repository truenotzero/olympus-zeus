#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

#define ZEUS_MAX_MEM 0x10000
#define ZEUS_ZERO_PAGE_SIZE 0x100
#define ZEUS_STACK_SIZE 0x100

struct zeus_regs {
  uint16_t pc; // program counter
  uint8_t sp;  // stack pointer
  uint8_t a;   // accumulator
  uint8_t x;   // index register x
  uint8_t y;   // index register y
  union {
    struct {
      unsigned c : 1; // carry
      unsigned z : 1; // zero
      unsigned i : 1; // interrupt disable
      unsigned d : 1; // decimal mode
      unsigned b : 1; // break command
      unsigned : 2;   // pad
      unsigned v : 1; // overflow
      unsigned n : 1; // negative
    };                // processor status
    uint8_t p;
  };
};

union zeus_bus {
  uint8_t raw[ZEUS_MAX_MEM];
  struct {
    uint8_t zero_page[0x100];    // $0000-$00FF, zero page memory
    uint8_t stack[0x100];        // $0100-$01FF, reserved for stack
    uint8_t general_bus[0xFDFA]; // $0200-$FFF0, general purpose use memory
    uint8_t int_handler[2];      // $FFFA-FFFB, non-maskable interrupt handler
    uint8_t rst_handler[2];      // $FFFC-FFFD, power on reset handler
    uint8_t brk_handler[2];      // $FFFE-FFFF, break command interrupt handler
  } region;
};

struct zeus_cpu {
  struct zeus_regs registers;
  union zeus_bus bus;
};

void zeus_reset(struct zeus_cpu *);
void zeus_exec(struct zeus_cpu *);

uint16_t zeus_read16(void const *);
void zeus_write16(void *, uint16_t);

#endif // CPU_H_
