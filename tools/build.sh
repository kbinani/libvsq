#!/bin/bash

(
  DIR="$(cd "$(dirname $0)/.."; pwd)"
  cd "$DIR"

  git submodule update --init --recursive

  rm -f "tests/cppunit.xml"
  rm -f "coverage.xml"

  if [ "x$CXX" = "x" ]; then
    CXX=$(which g++)
  fi
  GCC_ROOT=$(dirname "$CXX" | sed 's:/bin$::g')
  GCC_SUFFIX=$(basename "$CXX" | sed 's/g++//g')

  export CC="$GCC_ROOT/bin/gcc${GCC_SUFFIX}"
  export LD_LIBRARY_PATH="$GCC_ROOT/lib64:$LD_LIBRARY_PATH"

  rm -rf CMakeCache.txt CMakeFiles
  cmake . -DCOVERAGE=true || exit 1
  make clean || exit 1
  make || exit 1

  (
    cd "$DIR/tests"
    rm -rf CMakeCache.txt CMakeFiles
    cmake . -DCOVERAGE=true || exit 1
    make clean || exit 1
    make || exit 1

    ./libvsqtest --gtest_output=xml:gtest_result.xml || exit 1
  )

  rm -f cppcheck-result.xml
  cppcheck src include --enable=all -q --xml > cppcheck-result.xml 2>&1 || exit 1
  gcovr --gcov-executable="$GCC_ROOT/bin/gcov${GCC_SUFFIX}" -r . --exclude=tests/ --xml > coverage.xml || exit 1

  bash tools/audit.sh || exit 1

  doxygen > /dev/null
)
