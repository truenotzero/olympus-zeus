#include "src/cpu.c"
#include <test.inc>

TEST_UNUSED_INIT()
TEST_UNUSED_CLEANUP()

int read16() {
  char data[] = {0xFE, 0xCA};
  uint16_t expected = 0xCAFE;
  uint16_t actual = zeus_read16(&data[0]);
  return expected - actual;
}

int write16() {
  uint16_t data = 0xCAFE;
  char expected[] = {0xFE, 0xCA};
  char actual[2];
  zeus_write16(&actual[0], data);
  return (actual[0] - expected[0]) + (actual[1] - expected[1]);
}

int check_sign_negative() {
  uint8_t data = -1;
  uint8_t expected = 1;
  uint8_t actual = get_sign_bit8(data);
  return expected - actual;
}

int check_sign_positive() {
  uint8_t data = INT8_MAX;
  uint8_t expected = 0;
  uint8_t actual = get_sign_bit8(data);
  return expected - actual;
}

int set_zero_flag() {
  struct zeus_cpu cpu;
  cpu.registers.z = 0;
  update_p_z(&cpu, 0);
  return cpu.registers.z - 1;
}

int clear_zero_flag() {
  struct zeus_cpu cpu;
  cpu.registers.z = 1;
  update_p_z(&cpu, 1);
  return cpu.registers.z;
}

int set_neg_flag() {
  struct zeus_cpu cpu;
  cpu.registers.n = 0;
  update_p_n(&cpu, -1);
  return cpu.registers.n - 1;
}

int clear_neg_flag() {
  struct zeus_cpu cpu;
  cpu.registers.n = 1;
  update_p_n(&cpu, 1);
  return cpu.registers.n;
}

int add_that_carries() {
  bool expected = true;
  bool actual = check_carry(0xFF, 1);
  return expected - actual;
}

int add_that_doesnt_carry() {
  bool expected = false;
  bool actual = check_carry(0xFE, 1);
  return expected - actual;
}

int add_that_overflows() {
  struct add_check_overflow_t r = add_check_overflow(INT8_MAX, 1);
  int8_t expected = INT8_MIN;
  return TEST_EQ(r.result, expected) AND TEST_EQ(r.carry, 1);
}

int add_that_doesnt_overflow() {
  struct add_check_overflow_t r = add_check_overflow(-1, 1);
  int8_t expected = 0;
  return TEST_EQ(r.result, expected) AND TEST_EQ(r.carry, 0);
}

struct test *register_tests() {
  static struct test tests[] = {
      TEST_ADD(read16),
      TEST_ADD(write16),
      TEST_ADD(check_sign_negative),
      TEST_ADD(check_sign_positive),
      TEST_ADD(set_zero_flag),
      TEST_ADD(clear_zero_flag),
      TEST_ADD(set_neg_flag),
      TEST_ADD(clear_neg_flag),
      TEST_ADD(add_that_carries),
      TEST_ADD(add_that_doesnt_carry),
      TEST_ADD(add_that_overflows),
      TEST_ADD(add_that_doesnt_overflow),
      TEST_DONE(),
  };

  return tests;
}
