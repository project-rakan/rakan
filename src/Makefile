# A small Makefile to keep the back-end sane.
# DO NOT PUSH TO PRODUCTION.

CXX = g++
OBJS = rakan.o Graph.o Node.o Reader.o Runner.o
EXE  = rakan
HEADER = Graph.h Node.h Reader.h Runner.h
CPPFLAGS = -Wall -g -std=c++11

# default target
$(EXE): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(EXE) $(OBJS)

# source files
%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf $(EXE) *.o *~ *.dSYM
	