#include "cpu.h"
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

enum zeus_op {
  // Add with carry
  ADC_IMM = 0x69,

};

/*static bool is_arithmetic_op(uint8_t op) {
  static uint8_t const masks[] = {
      0x01, 0x05, 0x09, 0x0D, 0x11, 0x15, 0x19, 0x1D,
  };
  uint8_t const *mask = 0;
  for (size_t i = 0; i < sizeof(masks) / sizeof(*masks); ++i) {
    if (op & masks[i]) {
      mask = &masks[i];
      return true;
    }
  }

  return false;
}*/

uint16_t zeus_read16(void const *loc) {
  char const *p = loc;
  uint16_t lo = p[0] & 0xFF;
  uint16_t hi = p[1] & 0xFF;
  return lo | (hi << 8);
}

void zeus_write16(void *loc, uint16_t a) {
  char *p = loc;
  p[0] = a & 0xFF;
  p[1] = (a >> 8) & 0xFF;
}

void zeus_reset(struct zeus_cpu *cpu) {
  *cpu = (struct zeus_cpu){
      .registers.pc = zeus_read16(cpu->bus.region.rst_handler),
  };
}

static uint8_t get_sign_bit8(uint8_t a) { return (a >> 7) & 1; }

static void update_p_z(struct zeus_cpu *cpu, uint8_t a) {
  cpu->registers.z = a == 0;
}

static void update_p_n(struct zeus_cpu *cpu, uint8_t a) {
  cpu->registers.n = get_sign_bit8(a);
}

static bool check_carry(uint8_t l, uint8_t r) {
  // we derive the following code by:
  // carry := l + r > UINT_MAX
  // r > (UINT_MAX - l)
  // limit is calculated before to prevent overflow
  uint8_t limit = UINT8_MAX - l;
  return r > limit;
}

static struct add_check_overflow_t {
  int8_t result;
  int8_t carry;
} add_check_overflow(int8_t l, int8_t r) {
  struct add_check_overflow_t ret = {
      .result = l + r,
  };
  int8_t sign_l = get_sign_bit8(l);
  int8_t sign_r = get_sign_bit8(r);
  int8_t sign_result = get_sign_bit8(ret.result);
  if (sign_l == sign_r || l > r) {
    // pos + pos = pos - no overflow
    // neg + neg = neg - no overflow
    // pos + neg = pos if pos > neg - no overflow
    ret.carry = sign_l != sign_result;
  } else {
    // pos + neg = neg if pos < neg - no overflow
    ret.carry = sign_r != sign_result;
  }

  return ret;
}

enum op_type {
  Z_OP_TYPE_CONTROL,
  Z_OP_TYPE_ARITHMETIC,
  Z_OP_TYPE_RMW,
  Z_OP_TYPE_UNOFFICIAL,
};

static enum op_type decode_type(uint8_t op) {
  switch (op & 0x1F) {
  case 0x00:
  case 0x04:
  case 0x08:
  case 0x0C:
  case 0x10:
  case 0x14:
  case 0x18:
  case 0x1C:
    return Z_OP_TYPE_CONTROL;

  case 0x01:
  case 0x05:
  case 0x09:
  case 0x0D:
  case 0x11:
  case 0x15:
  case 0x19:
  case 0x1D:
    return Z_OP_TYPE_ARITHMETIC;

  case 0x02:
  case 0x06:
  case 0x0A:
  case 0x0E:
  case 0x12:
  case 0x16:
  case 0x1A:
  case 0x1E:
    return Z_OP_TYPE_RMW;

  case 0x03:
  case 0x07:
  case 0x0B:
  case 0x0F:
  case 0x13:
  case 0x17:
  case 0x1B:
  case 0x1F:
    return Z_OP_TYPE_UNOFFICIAL;
  }

  return *(enum op_type volatile *)0;
}

// arithmetic.c
extern void zeus_exec_arithmetic(struct zeus_cpu *cpu, uint8_t op);

void zeus_exec(struct zeus_cpu *cpu) {
  // fetch
  enum zeus_op op = cpu->bus.raw[cpu->registers.pc];

  // decode
  switch (decode_type(op)) {
  case Z_OP_TYPE_CONTROL:
    break;
  case Z_OP_TYPE_ARITHMETIC:
    zeus_exec_arithmetic(cpu, op);
    break;
  case Z_OP_TYPE_RMW:
    break;
  case Z_OP_TYPE_UNOFFICIAL:
    break;
  };

  /*
  // fetch
  enum zeus_op op = cpu->bus.raw[cpu->registers.pc];
  uint8_t imm = cpu->bus.raw[cpu->registers.pc + 1];
  printf("pc : %#x\n", cpu->registers.pc);
  printf("op : %#x\n", op);
  printf("imm: %#x\n", imm);

  // decode + execute
  switch (op) {
  case ADC_IMM: {
    if (!cpu->registers.d) {
      // normal
      uint8_t a = cpu->registers.a;
      uint8_t c = cpu->registers.c;
      // first add a + imm
      struct add_check_overflow_t res = add_check_overflow(a, imm);
      // now add with carry
      struct add_check_overflow_t res2 = add_check_overflow(res.result, c);
      // update acc
      cpu->registers.a = res2.result;
      // update status (z, n, v, c)
      update_p_z(cpu, res2.result);
      update_p_n(cpu, res2.result);
      cpu->registers.v = res.carry | res2.carry;
      cpu->registers.c = check_carry(a, imm) || check_carry(res.result, c);
    } else {
      // bcd
      (void)(*(int volatile *)0); // crash, unimplemented
    }
  } break;
  }
  */
}
