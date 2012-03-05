#!/bin/sh
directory=$(cd $(dirname $0);pwd)
cd $directory && python ../../gcovr --root="$directory/" --exclude="$directory/tests/*" --xml | sed -e 's:filename=\"/:filename=\":g' > $directory/coverage.xml

