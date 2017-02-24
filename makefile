SCALE = 3
DEBUG =  


NAME = lookup

VERSION_MAJOR = 0
VERSION_MINOR = 1.0
VERSION       = $(VERSION_MAJOR).$(VERSION_MINOR)

LIBRARY = lib$(NAME).so.$(VERSION)

EXE = $(NAME).exe

CC = gcc

CFLAGS = -std=c11 -fPIC -O3 -Wall -pedantic-errors $(DEBUG)-DSCALE=$(SCALE)
LDFLAGS = -shared


SOURCES = lookup.c
EXE_SRC = exe.c

HEADERS = $(SOURCES:.c=.h)
EXE_H   = $(EXE_SRC:.c=.h)

OBJECTS = $(SOURCES:.c=.o)
EXE_OBJ = $(EXE_SRC:.c=.o)

%.o: %.c $(SOURCES) $(EXE_SRC) $(HEADERS) $(EXE_H)
	$(CC) $(CFLAGS) -c $< -o $@

all: clean lib exe

lib: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LIBRARY) $(OBJECTS) $(CFLAGS)

exe: $(OBJECTS) $(EXE_OBJ)
	$(CC) $(CFLAGS) $(OBJECTS) $(EXE_OBJ) -o $(EXE) -lm

clean:
	rm -rf $(LIBRARY) $(EXE) $(OBJECTS) $(EXE_OBJ) a.out

