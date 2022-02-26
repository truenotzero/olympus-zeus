#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

struct zeus_regs {
  uint16_t pc; // program counter
  uint8_t sp;  // stack pointer
  uint8_t a;   // accumulator
  uint8_t x;   // index register x
  uint8_t y;   // index register y
  struct {
    int c : 1; // carry
    int z : 1; // zero
    int i : 1; // interrupt disable
    int d : 1; // decimal mode
    int b : 1; // break command
    int v : 1; // overflow
    int n : 1; // negative
  } status;    // processor status
};

struct zeus_cpu {
  struct zeus_regs registers;
  union {
    uint8_t memory[65536];
    struct {
      uint8_t zero_page[0x100];       // $0000-$00FF, zero page memory
      uint8_t stack[0x100];           // $0100-$01FF, reserved for stack
      uint8_t general_memory[0xFDFA]; // $0200-$FFF0, general purpose use memory
      uint16_t int_handler; // $FFFA-FFFB, non-maskable interrupt handler
      uint16_t rst_handler; // $FFFC-FFFD, power on reset handler
      uint16_t brk_handler; // $FFFE-FFFF, break command interrupt handler
    };
  };
};

#endif // CPU_H_