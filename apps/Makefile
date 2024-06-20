# Target programs
programs := \
	queue_tester.x \
	queue_tester.x \
	uthread_hello.x \
	uthread_yield.x \
	sem_buffer.x \
	sem_count.x \
	sem_prime.x \
	sem_simple.x \
	test_preempt.x \
	
# User-level thread library
UTHREADLIB := libuthread
UTHREADPATH := ../$(UTHREADLIB)
libuthread := $(UTHREADPATH)/$(UTHREADLIB).a

# Default rule
all: $(programs)

# Avoid builtin rules and variables
MAKEFLAGS += -rR

# Don't print the commands unless explicitly requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

# Current directory
CUR_PWD := $(shell pwd)

# Define compilation toolchain
CC	= gcc

# General gcc options
CFLAGS	:= -Wall -Wextra -Werror
CFLAGS	+= -pipe
## Debug flag
ifneq ($(D),1)
CFLAGS	+= -O2
else
CFLAGS	+= -g
endif
## Include path
CFLAGS 	+= -I$(UTHREADPATH)
## Dependency generation
CFLAGS	+= -MMD

# Linker options
LDFLAGS := -L$(UTHREADPATH) -luthread

# Application objects to compile
objs := $(patsubst %.x,%.o,$(programs))

# Include dependencies
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

# Rule for libuthread.a
$(libuthread): FORCE
	@echo "MAKE	$@"
	$(Q)$(MAKE) V=$(V) D=$(D) -C $(UTHREADPATH)

# Generic rule for linking final applications
%.x: %.o $(libuthread)
	@echo "LD	$@"
	$(Q)$(CC) -o $@ $< $(LDFLAGS)

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

# Cleaning rule
clean: FORCE
	@echo "CLEAN	$(CUR_PWD)"
	$(Q)$(MAKE) V=$(V) D=$(D) -C $(UTHREADPATH) clean
	$(Q)rm -rf $(objs) $(deps) $(programs)

# Keep object files around
.PRECIOUS: %.o
.PHONY: FORCE
FORCE:

#DELETE THIS LATER
sem_count:
	./sem_count.x

sem_countv:
	valgrind --leak-check=yes -s ./sem_count.x

sem_buffer:
	./sem_buffer.x

sem_bufferv:
	valgrind --leak-check=yes -s ./sem_buffer.x

sem_prime:
	./sem_prime.x

sem_primev:
	valgrind --leak-check=yes -s ./sem_prime.x

sem_simple:
	./sem_simple.x

sem_simplev:
	valgrind --leak-check=yes -s ./sem_simple.x

uthread_hello:
	./uthread_hello.x

uthread_hellov:
	valgrind --leak-check=yes -s ./uthread_hello.x

uthread_yield:
	./uthread_yield.x

uthread_yieldv:
	valgrind --leak-check=yes -s ./uthread_yield.x

queue_tester_example:
	./queue_tester_example.x

queue_tester_examplev:
	valgrind --leak-check=yes -s ./queue_tester_example.x

test_preempt:
	./test_preempt.x

test_preemptv:
	valgrind --leak-check=yes -s ./test_preempt.x