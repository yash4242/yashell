CC=gcc
ODIR=obj
SDIR=src

shell: $(SDIR)/*.c
	$(CC) -g $(SDIR)/*.c -o shell
clean:
	rm shell