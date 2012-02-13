BINS=proto_test oo_test
OBJS=$(BINS:%=%.o)
OPT=-ggdb3
CFLAGS=-Wall -Wextra -Wno-unused-parameter -Wno-unused-function -fno-stack-protector -fno-omit-frame-pointer -I. $(OPT)

all: $(OBJS)
	gcc -o proto_test proto_test.o
	gcc -o oo_test oo_test.o

clean: 
	rm -f *.o
	rm -f $(BINS)
