

EXEC_NAME=test
LIB_NAME=liblinearalgebra.so

INCLUDE_FLAGS=-I${mkEigenInc} -I${mkSuitesparseInc} -I../include
LIB_FLAGS=-Wl,-rpath=${PWD} -L${mkSuitesparseLib} -lumfpack -L. -llinearalgebra

CXX=g++ -fPIC



all:test
	# default path

clean:
	rm -f Utilities.o test.o $(LIB_NAME) $(EXEC_NAME)

Utilities.o:Utilities.cpp
	$(CXX) $(INCLUDE_FLAGS) -c Utilities.cpp

$(LIB_NAME):Utilities.o
	$(CXX) -shared -Wl,-soname,$(LIB_NAME) Utilities.o -\o $(LIB_NAME)

test.o:test.cpp
	$(CXX) $(INCLUDE_FLAGS) -c test.cpp

$(EXEC_NAME):$(LIB_NAME) test.o
	$(CXX) $(LIB_FLAGS) test.o -o $(EXEC_NAME)
