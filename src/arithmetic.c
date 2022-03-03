#include "cpu.h"

enum arithmetic_opcode {
  Z_OP_ORA,
  Z_OP_AND,
  Z_OP_EOR,
  Z_OP_ADC,
  Z_OP_STA,
  Z_OP_LDA,
  Z_OP_CMP,
  Z_OP_SBC,
};

static enum arithmetic_opcode decode(uint8_t op) {
  switch (op & 0xF0) {
  case 0x00:
    return Z_OP_ORA;
  case 0x20:
    return Z_OP_AND;
  case 0x40:
    return Z_OP_EOR;
  case 0x60:
    return Z_OP_ADC;
  case 0x80:
    return Z_OP_STA;
  case 0xA0:
    return Z_OP_LDA;
  case 0xC0:
    return Z_OP_CMP;
  case 0xE0:
    return Z_OP_SBC;
  }

  return *(enum arithmetic_opcode volatile *)0;
}

// currently implemented:
// arithmetic adressing modes only!
enum zeus_address_mode {
  Z_MODE_INDEXED_INDIRECT,
  Z_MODE_ZERO_PAGE,
  Z_MODE_IMMEDIATE,
  Z_MODE_ABSOLUTE,
  Z_MODE_INDIRECT_INDEXED,
  Z_MODE_ZERO_PAGE_X,
  Z_MODE_ABSOLUTE_X,
  Z_MODE_ABSOLUTE_Y,

};

static enum zeus_address_mode get_addressing_mode(uint8_t op) {
  switch (op & 0x1F) {
  case 0x01:
    return Z_MODE_INDEXED_INDIRECT;
  case 0x05:
    return Z_MODE_ZERO_PAGE;
  case 0x09:
    return Z_MODE_IMMEDIATE;
  case 0x0D:
    return Z_MODE_ABSOLUTE;
  case 0x11:
    return Z_MODE_INDIRECT_INDEXED;
  case 0x15:
    return Z_MODE_ZERO_PAGE_X;
  case 0x19:
    return Z_MODE_ABSOLUTE_X;
  case 0x1D:
    return Z_MODE_ABSOLUTE_Y;
  };

  return zeus_p_noreturn(enum zeus_address_mode);
}

void zeus_exec_arithmetic(struct zeus_cpu *cpu, uint8_t op) {
  zeus_p_unused(cpu);

  // first decode addressing mode and fetch args
  switch (get_addressing_mode(op)) {
  case Z_MODE_INDEXED_INDIRECT:
    //
    break;
  case Z_MODE_ZERO_PAGE:
    //
    break;
  case Z_MODE_IMMEDIATE:
    //
    break;
  case Z_MODE_ABSOLUTE:
    //
    break;
  case Z_MODE_INDIRECT_INDEXED:
    //
    break;
  case Z_MODE_ZERO_PAGE_X:
    //
    break;
  case Z_MODE_ABSOLUTE_X:
    //
    break;
  case Z_MODE_ABSOLUTE_Y:
    //
    break;
  }
  // process opcode
  switch (decode(op)) {
  case Z_OP_ORA:
    //
    break;
  case Z_OP_AND:
    //
    break;
  case Z_OP_EOR:
    //
    break;
  case Z_OP_ADC:
    //
    break;
  case Z_OP_STA:
    //
    break;
  case Z_OP_LDA:
    //
    break;
  case Z_OP_CMP:
    //
    break;
  case Z_OP_SBC:
    //
    break;
  }
}
