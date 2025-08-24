CC = gcc

INCLUDE_DIRS := $(wildcard ../components/*/include)
INCLUDE := $(foreach dir,$(INCLUDE_DIRS), -I$(dir)) -I./include
SRC = $(wildcard ../components/*/source/*.c)
SRC += $(wildcard ./*.c)

CFLAGS = $(INCLUDE) -g -Wall -DUNIT_TEST

OUT := test_dht22.o

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	./$(OUT)

clean:
	rm -rf ./$(OUT)