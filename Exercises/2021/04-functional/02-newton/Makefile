CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O3 -I. -Wno-conversion-null -Wno-deprecated-declarations

EXEC     = main
LDFLAGS ?= 
LIBS    ?= 

# Flags for the muParser example.
CPPFLAGMP ?= -I../../../../Examples/include
EXECMP     = main_getpot_muparser
LDFLAGMP  ?= -L../../../../Examples/lib -Wl,-rpath=../../../../Examples/lib
LIBMP     ?= -lmuparser

all: $(EXEC) $(EXECMP)

%.o: %.cpp newton.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXECMP).o: %.o: %.cpp newton.hpp muparser_fun.hpp
	$(CXX) -c $(CPPFLAGS) $(CPPFLAGMP) $(CXXFLAGS) $<

$(EXEC): %: %.o newton.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) newton.o $< $(LIBS) -o $@

$(EXECMP): %: %.o newton.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGMP) newton.o $< $(LIBS) $(LIBMP) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC) $(EXECMP)
	$(RM) *~
