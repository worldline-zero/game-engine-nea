CC = clang++

CFLAGS = -Wall -Wimplicit-fallthrough -g

LDFLAGS = -ldl -lGL -lglfw


OBJS = src/* lib/src/gl.c

INCFLAGS = -I ./lib/include/glad
build:
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCFLAGS) $(OBJS)
	mv a.out $(OBJNAME)
