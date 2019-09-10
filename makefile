APPNAME=main
APPMAIN=./src/main.c
TESTMAIN=ppr_tb_test_cli
###########################################################################
# Which compiler
CC=g++
FC=g77
###########################################################################
# Where to install
TARGET=./
###########################################################################
# Where are include files kept
LIBS=-lcppunit
INCLUDES=-I./src -I./test
###########################################################################
# Compile option
CFLAGS=-g -Wall -coverage

SRC:=$(filter-out $(APPMAIN),$(wildcard ./src/*.c),$(wildcard ./src/*.h))
TEST:=$(wildcard ./test/*.c)
OBJ:=$(SRC:.c=.o) $(TEST:.c=.o)

###########################################################################
# Control Script
all: clean compile report
clean:
	find ./ -name *.o -exec rm -v {} \;
	find ./ -name *.gcno -exec rm -v {} \;
	find ./ -name *.gcda -exec rm -v {} \;
	-rm $(APPNAME)
	-rm $(TESTMAIN)
	-rm *_result.xml
	-rm doxygen_*
	-rm -rf html
	-rm -rf latex

###########################################################################
# Body
compile: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TESTMAIN) $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -c $< -o $@

report:
	./$(TESTMAIN)
	doxygen doxygen.conf src > /dev/null
	doxygen doxygen.conf > /dev/null
	gcovr -r . --html --html-details -o example-html-details.html
	cppcheck -v --enable=all --xml --xml-version=2 test/ppr_tb_test_cli.c 2> cppcheck-result.xml
	gcovr --xml --output=gcover_result.xml src/
	
	