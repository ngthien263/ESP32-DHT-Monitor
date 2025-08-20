CC = gcc
OUT = test_dht22_update_data.out

INCLUDE = -I../main/include  -I./include
SRC = test_dht22_update_data.c \
      ../main/dht22.c \
	  ../main/dht22_private.c \
      unity.c

CFLAGS = $(INCLUDE) -Wall -DUNIT_TEST

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
