#include "cpu.h"
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

enum zeus_op {
  ADC_IMM = 0x69,
};

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

void zeus_exec(struct zeus_cpu *cpu) {
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
}
