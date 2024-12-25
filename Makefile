# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c queries.c image.c readwrite.c -o  image_editor -lm

clean:
	rm -f $(TARGETS)

pack:
	zip -FSr 311CA_IvanAndreiCristian_Tema3.zip README Makefile *.c *.h

.PHONY: pack clean
