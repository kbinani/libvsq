#!/bin/bash

(
  DIR="$(cd "$(dirname $0)/.."; pwd)"
  cd "$DIR"

  rm -f "tests/cppunit.xml"
  rm -f "coverage.xml"

  GCC_SUFFIX="-4.9.2"
  GCC_ROOT="/opt/gcc-4.9.2/"
  DOXYGEN=/usr/local/bin/doxygen

  export LD_LIBRARY_PATH="$GCC_ROOT/lib64:$LD_LIBRARY_PATH"
  export CXX="$GCC_ROOT/bin/g++${GCC_SUFFIX}"
  export CC="$GCC_ROOT/bin/gcc${GCC_SUFFIX}"

  cd "$DIR"
  cmake . -DCOVERAGE=true
  make clean
  make

  cd "$DIR/tests"
  cmake . -DCOVERAGE=true
  make clean
  make

  cd "$DIR"
  tests/libvsqtest -o tests/cppunit.xml -f tests
  rm -f cppcheck-result.xml
  cppcheck src include --enable=all -q --xml > cppcheck-result.xml 2>&1
  gcovr --gcov-executable="$GCC_ROOT/bin/gcov${GCC_SUFFIX}" -r . --exclude=tests/ --xml > coverage.xml

  cd "$DIR"
  "$DOXYGEN"
)
