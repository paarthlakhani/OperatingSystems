#!/bin/bash
x=$1
for((i = 1;i<=$x; i++));do
    cp Notes "test_$i.txt";
done
# rm test*.txt