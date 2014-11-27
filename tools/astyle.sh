#!/bin/bash

(
  cd "$(cd "$(dirname "$0")"; pwd)/../"
  for FILE in $(git ls-files | grep -e '\.hpp' -e '\.cpp'); do
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
  done
)

