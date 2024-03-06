#
# Makefile for C++ pthsort
#
CXX = g++
CXXFLAGS = -Wall -Werror
LIBS = -lpthread
RM = /bin/rm
RMFLAGS = -rf
SRCS = pthsv.cpp

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

# Type 'make clean' up the subdirectory...
# Be Very Careful Not To Haphazzardly Make Changes!!!!!!!
clean:
	$(RM) $(RMFLAGS) $(PROGS) $(OBJS) tests-out

test:
	./test-pthsv.sh

test-all:
	./test-pthsv.sh -c
