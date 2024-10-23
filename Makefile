CC = gcc
CFLAGS = -Wall -I$(INCDIR)

SRCDIR = miris_src/modules
INCDIR = miris_src/include
BINDIR = bin

PROG = ./miris

SOURCE = main.c prompt.c node.c edge.c inNodes.c
HEADER = header.h graph.h edge.h inNodes.h
OBJS = $(patsubst %.c, $(BINDIR)/%.o, $(SOURCE))

SRCFILES = $(patsubst %, $(SRCDIR)/%, $(SOURCE))
HEADERS = $(patsubst %, $(INCDIR)/%, $(HEADER))

all: $(BINDIR) $(PROG)

$(BINDIR):
	mkdir -p $(BINDIR)

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

$(BINDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/helper.h
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f core $(PROG) $(OBJS)

TAGS: $(SRCFILES) $(HEADERS)
	etags -t $(SRCFILES) $(HEADERS)
