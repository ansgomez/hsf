#!/bin/bash

cd ./lib
tar zxvf lib.tar.gz 
sudo mv libmgl.so.7.0.0.$(shell getconf LONG_BIT) /usr/lib/libmgl.so.7.0.0
rm libmgl.so.7.0.0*
cd ..
make all

