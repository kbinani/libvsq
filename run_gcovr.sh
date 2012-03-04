#!/bin/sh
directory=$(cd $(dirname $0);pwd)
cd $directory && python ../../gcovr --root="$directory/" -exclude=$directory/tests --xml > coverage.xml.tmp
cat $directory/coverage.xml.tmp | sed -e 's:filename=\"/:filename=\":g' > $directory/coverage.xml
rm -f $direcgtory/coverage.xml.tmp
