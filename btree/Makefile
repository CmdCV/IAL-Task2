CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -lm -fsanitize=address -g
FILES=iter/stack.c test_util.c test.c exa/exa.c btree.c character.c

test:
	@$(CC) $(CFLAGS) $(FILES) rec/btree.c -o pepega_rec
	@$(CC) $(CFLAGS) $(FILES) iter/btree.c -o pepega_iter
	@./pepega_rec > pepega_rec.out
	@./pepega_iter > pepega_iter.out
	@diff --color -u --report-identical-files --text pepega_rec.out pepega_iter.out

clean:
	rm -f pepega_rec pepega_iter pepega_rec.out pepega_iter.out
