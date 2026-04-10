CC = gcc
CFLAGS = -Wall -Iinclude -I/opt/homebrew/include -O2
LDFLAGS = -L/opt/homebrew/lib -lraylib -lm -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRCDIR = src
OBJDIR = obj
BINDIR = bin

# List of source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE = $(BINDIR)/asteroids

all: directories $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

directories:
	mkdir -p $(OBJDIR) $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean directories
