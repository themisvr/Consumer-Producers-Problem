SRCDIR = src
HEADERDIR = include
OBJDIR = obj

CC = gcc
CFLAGS = -g -Wall -Wextra -Ofast -I$(HEADERDIR)
OSFLAGS = -lpthread

BINARY = feeder

SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))


$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BINARY) $(OSFLAGS)


$(OBJDIR):
	@mkdir $(OBJDIR)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean:
	rm -r *.txt $(BINARY) $(OBJDIR)/*