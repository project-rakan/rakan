# define the commands we will use for compilation and library building
AR = ar
ARFLAGS = rcs
CXX = g++

# define useful flags to cc/ld/etc.
CFLAGS += -g -Wall -Wpedantic -I. -I.. -O0 -std=c++11
LDFLAGS += -L. -lhw1
CPPUNITFLAGS = -L/googletest -lgtest

# define common dependencies
OBJS = src/Runner.o src/Graph.o src/Node.o src/Reader.o
HEADERS = src/Runner.h src/Graph.h src/Node.h src/Reader.h

# compile everything; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile
all: rakan

rakan: $(OBJS)
	$(CXX) $(CFLAGS) -o $<

src/%.o: src/%.cc $(HEADERS)
	$(CXX) $(CFLAGS) -c $<

clean:
	/bin/rm -f ./*.o src/*.o src/*~ test/*~ *.gcno *.gcda *.gcov
