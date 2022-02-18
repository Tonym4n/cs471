CXX := 		g++
CXXFLAGS :=	-g -Wall -std=c++17
VPATH :=	.

all:				scanner_parser

scanner_parser:		scanner_parser.o
					$(CXX) $^ -o $@

clean:	
					rm -f *.o *~ *.gch scanner_parser

#dependencies
scanner_parser.o:	scanner_parser.cpp
					$(CXX) $(CXXFLAGS) -c $^