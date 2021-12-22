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

all: all-before $(OUTPUT) $(CLIENT) $(SERVER) all-after
	@echo Makefile complete!

LIBSBOTH = src/view.o src/login.o src/socket.o src/process.o

CLIENT_LIB = src/client.o $(LIBSBOTH)
SERVER_LIB = src/server.o $(LIBSBOTH)

$(CLIENT): $(CLIENT_LIB)
	$(CC) $(CFLAGS) $(INCLUDES) -o output/client $(CLIENT_LIB) $(LFLAGS) 

$(SERVER): $(SERVER_LIB)
	$(CC) $(CFLAGS) $(INCLUDES) -o output/server  $(SERVER_LIB) $(LFLAGS) 

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

all-before:
	$(MD) ./obj

all-after:
	$(MV) src/*.o ./obj

# define object directory
OBJ_PATH := ./obj/

clean:
	$(RM) output/client output/server
	$(RM) $(OBJ_PATH)*.o
	$(RM) src/*.o
	$(DD) ./obj
	@echo Cleanup complete!