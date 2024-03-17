#
# Makefile for C++ pthpi
#
CXX = g++
CXXFLAGS = -Wall -Werror
LIBS = -lpthread -lm
RM = /bin/rm
RMFLAGS = -rf
SRCS = pthpi.cpp

# OBJS = $(SRCS:.c=.out.o)
# PROGS = $(SRCS:.c=.out)

OBJS = $(SRCS:.cpp=.o)
PROGS = $(SRCS:.cpp=)


# This is the multi-executable target...
# If you type 'make' at the command line it will build
# all programs
.PHONY: all

all: $(PROGS)

$(PROGS): %: %.o Makefile
	$(CXX) $< -o $@ $(LIBS)

# This line will convert any .c file to a .o file.
%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	./pthpi