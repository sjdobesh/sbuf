# choose a compiler
CC := gcc

# library name stem
STEM := sbuf

# set directories
SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib
BIN_DIR := bin
TEST_DIR := test

# set targets
# executable
TEST := $(BIN_DIR)/test
# srcs
SRC := $(SRC_DIR)/$(STEM).c
TEST_SRC := $(TEST_DIR)/test.c
# objects
SRC_OBJ := $(OBJ_DIR)/$(STEM).o
TEST_OBJ := $(OBJ_DIR)/test.o
# libs
STATIC_LIB := $(LIB_DIR)/lib$(STEM).a
SHARED_LIB := $(LIB_DIR)/lib$(STEM).so

# set flags
CPPFLAGS := -fpic -MMD -MP
CFLAGS := -Wall -Wextra -Werror -ansi -pedantic -Os
LIBFLAGS := -shared

# set rules

.PHONY: all test lib clean

all: test lib
	./$(TEST)

test: $(TEST)

lib: $(SHARED_LIB) $(STATIC_LIB)

$(TEST): $(SRC_OBJ) $(TEST_OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(STATIC_LIB): $(SRC_OBJ) | $(LIB_DIR)
	ar -rs $^ -o $@

$(SHARED_LIB): $(SRC_OBJ) | $(LIB_DIR)
	$(CC) $(LIBFLAGS) $^ -o $@

$(SRC_OBJ): $(SRC) | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): $(TEST_SRC) | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(LIB_DIR):
	mkdir -p $@

clean:
	@$(RM) -rfv $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
