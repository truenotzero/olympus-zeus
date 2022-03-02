CC:=clang
CFLAGS:=-Wall -Wextra -Wpedantic -Werror -O0 -g -Ieverest-test -I.
LDFLAGS:=

TARGET:=zeus
SRC_DIR:=src
BIN_DIR:=bin
TEST_SRC_DIR:=test
TEST_BIN_DIR:=$(BIN_DIR)/test

ALL_SRCS:=$(shell find $(SRC_DIR) -name '*.c')
ALL_OBJS:=$(ALL_SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

$(BIN_DIR)/$(TARGET): $(ALL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(ALL_OBJS): $(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(TEST_BIN_DIR)/%.test
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf bin

include everest-test/test.makefile
