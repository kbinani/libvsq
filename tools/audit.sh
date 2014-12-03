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
      if [ -z "$(head -2 "$FILE" | tail -1 | grep "$BASENAME")" ]; then
        echo "Error: missing license header for '$FILE'."
        OK=0
      fi
    fi
  done

  TEST_MAIN_CPP="$DIR/main.cpp"
  TEST_MAIN_OUT="$DIR/main.o"
  for FILE in $(git ls-files | grep -v '^tests/' |  grep -e '\.hpp' -e '\.h'); do
    (
      echo "#include \"../$FILE\"";
      echo "int main() {}";
    ) > "$TEST_MAIN_CPP"
    if ! g++ -std=c++11 -c "$TEST_MAIN_CPP" -o "$TEST_MAIN_OUT" >/dev/null 2>&1; then
      echo "Error: failed to compile with header '$FILE'."
      echo "tested source file is:"
      cat "$TEST_MAIN_CPP"
      OK=0
    fi
  done
  rm -f "$TEST_MAIN_CPP" "$TEST_MAIN_OUT"

  if [ $OK ]; then
    echo "SUCCESS"
  else
    echo "FAIL"
  fi
)
