# Target library
lib     := libuthread.a
CC      := gcc
objs    := queue.o uthread.o preempt.o context.o sem.o
targets := queue uthread preempt context sem

CFLAGS    := -Wall -Wextra -Werror -MMD
CFLAGS    += -g

ifneq ($(V),1)
Q = @
endif

all: $(lib)

# all: $(targets)
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

$(targets): $(objs)
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(lib): $(objs)
	@echo "CC $@"
	$(Q) ar rcs  $@ $^

clean:
	$(Q)rm -f $(lib) $(targets) $(objs) $(deps)