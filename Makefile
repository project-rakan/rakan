# define the commands we will use for compilation and library building
AR = ar
ARFLAGS = rcs
CXX = g++

# define useful flags to cc/ld/etc.
CFLAGS += -g -Wall -Wpedantic -I. -I.. -O0
LDFLAGS += -L. -lhw1
CPPUNITFLAGS = -L/googletest -lgtest

# define common dependencies
OBJS = src/Runner.o src/Graph.o src/Node.o src/Reader.o src/ReaderUtils.o
HEADERS = src/Runner.h src/Graph.o src/Node.o src/Reader.o src/ReaderUtils.o
TESTOBJS = test/test_suite.o

# compile everything; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile
all: test/test_suite

test/test_suite: $(TESTOBJS) libhw1.a
	$(CXX) $(CFLAGS) -o test/test_suite $(TESTOBJS) \
	$(CPPUNITFLAGS) $(LDFLAGS) -lpthread $(LDFLAGS)

src/%.o: src/%.cc $(HEADERS)
	$(CXX) $(CFLAGS) -std=c++11 -c $<

test/test_suite.o: test/%.cc $(HEADERS)
	$(CXX) $(CFLAGS) -std=c++11 -c $<

clean:
	/bin/rm -f src/*.o test/*.o src/*~ test/*~ *.gcno *.gcda *.gcov \
	test/test_suite

