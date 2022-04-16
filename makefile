CC=gcc
CFLAGS=-Wall -Wextra -Werror

cache:
	$(CC) $(CFLAGS) -o cache cache.c