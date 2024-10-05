CC = gcc
CFLAGS = -Wall
OBJDIR = build
BINARY = linkedlist

# List of object files in the build directory
OBJFILES = $(OBJDIR)/main.o $(OBJDIR)/linked_list.o

# Default target to build the executable
all: $(BINARY)

# Rule to create the binary by linking object files
$(BINARY): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(BINARY) $(OBJFILES)

# Compile main.c into main.o inside the build directory
$(OBJDIR)/main.o: main.c linked_list.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJDIR)/main.o

# Compile linked_list.c into linked_list.o inside the build directory
$(OBJDIR)/linked_list.o: linked_list.c linked_list.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c linked_list.c -o $(OBJDIR)/linked_list.o

# Create the build directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up all object files and the binary
clean:
	rm -rf $(OBJDIR) $(BINARY)