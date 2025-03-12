CFLAGS = -g -Wall -Wno-unused-parameter
CC = GCC

.PHONY: all clean

PROGRAMS = main_tests

SRCS = hachage.c
TEST_SRCS = tests/main_tests.c tests/testsHachage.c

OBJS = $(SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)

# EXEC = main
TEST_EXEC = main_tests

all: 