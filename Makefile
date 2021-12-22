#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS	:= -Wall -Wextra -g

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -lncurses -lpthread

# define output directory
OUTPUT	:= output

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

CLIENT := client
SERVER := server
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)


RM = rm -f
MD	:= mkdir -p
MV := mv
DD := rmdir

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

all: all-before $(OUTPUT) $(CLIENT) $(SERVER) all-after
	@echo Makefile complete!

$(CLIENT): src/client.o
	$(CC) $(CFLAGS) $(INCLUDES) -o output/client src/client.o $(LFLAGS) $(LIBS)

$(SERVER): src/server.o
	$(CC) $(CFLAGS) $(INCLUDES) -o output/server src/server.o $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

all-before:
	$(MD) ./obj

all-after:
	$(MV) src/client.o ./obj
	$(MV) src/server.o ./obj

# define object directory
OBJ_PATH := ./obj/

clean:
	$(RM) output/client output/server
	$(RM) $(OBJ_PATH)*.o
	$(DD) ./obj
	@echo Cleanup complete!