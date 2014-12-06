#!/bin/bash

(
  OK=1

  DIR=$(cd "$(dirname "$0")"; pwd)
  cd "$DIR/../"
  for FILE in $(git ls-files | grep -e '\.hpp' -e '\.cpp' -e '\.h'); do
    astyle \
      --style=linux \
      --indent=force-tab=4 \
      --indent=force-tab-x=4 \
      --align-pointer=type \
      --align-reference=type \
      --indent-switches \
      --indent-preproc-define \
      --pad-oper \
      --pad-header \
      --unpad-paren \
      --add-one-line-brackets \
      --mode=c \
      --suffix=none \
      --formatted \
      --lineend=windows \
      "$FILE"
    if [ -n "$(echo "$FILE" | grep -v '^tests/')" ]; then
      BASENAME=$(basename "$FILE")
      if [ -z "$(head -2 "$FILE" | tail -1 | grep "@file $BASENAME")" ]; then
        echo "Error: missing license header for '$FILE'."
        OK=0
      fi
    fi
  done

  TEST_MAIN_CPP="$DIR/main.cpp"
  TEST_MAIN_OUT="$DIR/main.o"
  if [ ! -e "$CXX" ]; then
    CXX="$(which g++)"
  fi
  for FILE in $(git ls-files | grep -v '^tests/' |  grep -e '\.hpp' -e '\.h'); do
    (
      echo "#include \"../$FILE\"";
      echo "int main() {}";
    ) > "$TEST_MAIN_CPP"
    if ! "$CXX" -std=c++11 -c "$TEST_MAIN_CPP" -o "$TEST_MAIN_OUT" >/dev/null 2>&1; then
      echo "Error: failed to compile with header '$FILE'."
      echo "tested source file is:"
      cat "$TEST_MAIN_CPP"
      OK=0
    fi
  done
  rm -f "$TEST_MAIN_CPP" "$TEST_MAIN_OUT"

  ALL_HEADER_CONTENT_ACTUAL="$DIR/libvsq_actual.tmp"
  ALL_HEADER_CONTENT_EXPECTED="$DIR/libvsq_expected.tmp"

  cat "$DIR/../include/libvsq/libvsq.h" \
   | tr -d '\r' \
   | grep include \
   | sed 's:^#include "./\(.*\)"$:\1:g' \
   | sort \
   > "$ALL_HEADER_CONTENT_ACTUAL"
  (
    cd "$DIR/../include/libvsq"
    git ls-files | grep -v libvsq.h | sort > "$ALL_HEADER_CONTENT_EXPECTED"
  )
  if ! diff -u "$ALL_HEADER_CONTENT_EXPECTED" "$ALL_HEADER_CONTENT_ACTUAL"; then
    echo "Error: libvsq.h is incomplete"
    OK=0
  fi
  rm -f "$ALL_HEADER_CONTENT_EXPECTED" "$ALL_HEADER_CONTENT_ACTUAL"

  if [ "$OK" = "1" ]; then
    echo "SUCCESS"
    exit 0
  else
    echo "FAIL"
    exit 1
  fi
)
