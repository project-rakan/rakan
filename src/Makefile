# A small Makefile to keep the back-end sane.
# DO NOT PUSH TO PRODUCTION.

CXX = g++
EXE  = ../rakan_run
OBJS = ./rakan.o ./Runner.o ./Graph.o ./Node.o ./Reader.o
HEADER = ./Runner.h ./Graph.h ./Node.h ./Reader.h ./Queue.h
CPPFLAGS = -Wall -g -std=c++11
LFLAGS = c

# default target
$(EXE): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(EXE) $(OBJS) $(LFLAGS)

# source files
%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(LFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf $(EXE) *.o *~ *.dSYM
