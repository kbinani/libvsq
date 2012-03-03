#!/bin/sh
directory=$(cd $(dirname $0);pwd)
cppcheck $directory --enable=all -q --xml 2>$directory/cppcheck-result.xml

