CC = gcc
CFLAGS = -std=c99
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)

parser : $(OBJS)
	$(CC) -o parser $(OBJS)

-include $(patsubst %.o, %.d, $(OBJS))

.PHONY : clean test
clean :
	rm parser $(OBJS)
test:
	./parser ../Test/test3.ir ../IR_optimize_result/test.3ir
# ../initial_SSA_formal/test2.ir ../IR_optimize_result/test.2ir