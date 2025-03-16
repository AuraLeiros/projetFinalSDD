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

all: $(PROGRAMS)

main_tests: main_tests.o hachage.o testsHachage.o utils.o
     $(CC) $(CFLAGS) -o $@ $^

main_tests.o: main_tests.c
     $(CC) $(CFLAGS) -c $<

hachage.o: hachage.c
     $(CC) $(CFLAGS) -c $<

testsHachage.o: testsHachage.c  # Fix here
     $(CC) $(CFLAGS) -c $<

utils.o: utils.c
     $(CC) $(CFLAGS) -c $<

clean:
     rm -f *.o *.gch *~ $(PROGRAMS) ./rechercheMultiplesLC.txt
