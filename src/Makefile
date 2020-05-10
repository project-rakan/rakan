# A small Makefile to keep the back-end sane.
# DO NOT PUSH TO PRODUCTION.

CXX = g++
EXE  = rakan
OBJS = ./rakan.o ./Runner.o ./Graph.o ./Node.o ./Reader.o
HEADER = ./Runner.h ./Graph.h ./Node.h ./Reader.h 
CPPFLAGS = -Wall -g -std=c++11
# LFLAGS = -L/usr/lib -L/opt/lib -L/usr/local/lib -lamqpcpp -ldl -lssl -lpthread -levent

# default target
$(EXE): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(EXE) $(OBJS)

# source files
%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf $(EXE) *.o *~ *.dSYM
