TARGET = librobot.so
SRCS = robot.c umbrella.c eater.c
HEADERS = $(addprefix src/, ${SRCS:.c=.h})
OBJECTS = $(addprefix build/,${SRCS:.c=.o})
CC=gcc
CFLAGS = -O2 -std=gnu99 -Wall -Werror -fpic
LDFLAGS= -shared -lrobotdriver
PREFIX = /usr/local
VPATH = build/
TESTS = tests/robottest tests/AX12position tests/umbrellatest tests/eatertest

vpath %.c src/
vpath %.h src/

.PHONY: clean all build

all: build build/$(TARGET)

build:
	mkdir -p build
build/%.o: %.c build/%.d
	$(CC) -c -o $@ $< $(CFLAGS)
build/%.d : %.c
	$(CC) $(CFLAGS) -MM -MF $@ -MP $<
build/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

tests: LDFLAGS = -lrobot -lrobotdriver -lpathfollower
tests: $(TESTS)

clean:
	rm -f build/*.o build/*.so build/*.d $(TESTS)

install: build/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/librobot
	cp build/$(TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/librobot/
	chmod 0755 $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	ldconfig
	ldconfig -p | grep robot

-include $(subst .c,.d,$(SRCS))
