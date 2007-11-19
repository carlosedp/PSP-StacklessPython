#!/bin/bash

# This scripts  generates a zip  file containing the  standard library
# and some  additional modules. It should  be unzipped at  the root of
# the MS.

mkdir python

echo Copying standard library
cp -a ../Lib/* python/

echo Copying elementtree
cp -a elementtree/elementtree python/site-packages/

echo Copying pysqlite2
cp -a pysqlite/pysqlite2 python/site-packages/

echo Cleaning SVN files
cd python
find -name ".svn" | xargs rm -rf
cd ..

echo Zipping
zip -r psp.zip python
rm -rf python
