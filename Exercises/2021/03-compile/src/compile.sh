#!/bin/bash

set -x #prints all commands being executed

module load gcc-glibc/9
module load eigen suitesparse

EXEC_NAME=test
LIB_NAME=liblinearalgebra.so

INCLUDE_FLAGS="-I${mkEigenInc} -I${mkSuitesparseInc} -I../include"
LIB_FLAGS="-Wl,-rpath=${PWD} -L${mkSuitesparseLib} -lumfpack -L. -llinearalgebra"

CXX="g++ -fPIC"

# step 0 : cleanup
rm -f test.o Utilities.o ${LIB_NAME} ${EXEC_NAME}

# step1 : compile Utilities.cpp
${CXX} ${INCLUDE_FLAGS} -c Utilities.cpp

# step 2 : make it a shared library named liblinearalgebra
${CXX} -shared -Wl,-soname,${LIB_NAME} \Utilities.o -o ${LIB_NAME}

# step 3 : compile test.cpp
${CXX} ${INCLUDE_FLAGS} -c test.cpp

# step 4 : make an executable linked against linearalgebra.so
${CXX} ${LIB_FLAGS} test.o -o ${EXEC_NAME}


set +x #reset variable
