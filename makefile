CC=gcc
BUILD_PATH=build

WARNING_CFLAGS = -Wall -Wextra -Wpedantic
CFLAGS=$(INCLUDE_PATH_CFLAG) -g -Og $(WARNING_CFLAGS)
RELEASE_CFLAGS = -O3 -DNDEBUG

NCURSES = -lncurses -lpanel

release: CFLAGS += $(RELEASE_CFLAGS)
release: Pong

$(BUILD_PATH):
	mkdir $(BUILD_PATH)

ThreadPool.o : ThreadPool.c ThreadPool.h | $(BUILD_PATH)
	$(CC) $(CFLAGS) -c -o $(BUILD_PATH)/$@ $<

ScreenElements.o : ScreenElements.c ScreenElements.h | $(BUILD_PATH)
	$(CC) $(CFLAGS) -c -o $(BUILD_PATH)/$@ $<

Pong : main.c ThreadPool.o ScreenElements.o | $(BUILD_PATH)
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$@ main.c $(BUILD_PATH)/ThreadPool.o $(BUILD_PATH)/ScreenElements.o $(NCURSES)