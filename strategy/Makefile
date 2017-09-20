# This basic makefile is composed of
#
# target : dependencies
# [tab] system command

# CCFLAGS = compiler options
# LDFLAGS = linker options
# CC = compiler
# -s = compiler stops after stage of compilation proper (output is assembler code)

CC=g++
ADD_PATH=-I /home/punit/Dropbox/Projects/strategy/strategy 

# -c = compile or assemble but do not link
# -Wall = enables all compiler warning messages
CFLAGS=-c -Wall $(ADD_PATH)
LDFLAGS=

# Sets variables to be constant strings. In this case,
# we're just naming all the source files to be used.
TEST_SOURCES=tests/catch.cpp tests/order_test.cpp tests/currency_test.cpp
SOURCES=currency.cpp order.cpp ipc_unix.cpp ipc_win.cpp
MAIN=main.cpp

# Sets the objects to be the same as sources but also replaces
# any occurrence of '.cpp' with '.o' in all the sourcefile names.
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)
OBJECTS=$(SOURCES:.cpp=.o)
MAIN_OBJECTS=$(MAIN:.cpp=.o)

EXECUTABLE=strategy
TEST_EXECUTABLE=strategy_test

# Default target
all : $(MAIN) $(SOURCES) $(EXECUTABLE)

# Linking, -o file = place output in file
$(EXECUTABLE) : $(MAIN_OBJECTS) $(OBJECTS)
	$(CC) $(LDFLAGS) $(MAIN_OBJECTS) $(OBJECTS) -o $@

# Testing target
test : $(TEST_SOURCES) $(SOURCES) $(TEST_EXECUTABLE)        

# Linking, -o file = place output in file
$(TEST_EXECUTABLE) : $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS) $(OBJECTS) -o $@

# First rebuild/create/find the file ending in .cpp or .o and then
# run the command
# $@ = filename of the target of the rule
# $< = the name of the first prerequisite (from implicit rule)
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:        
	rm -rf *.o \
        rm -rf tests/*.o



