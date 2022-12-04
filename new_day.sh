#!/bin/bash

DAY=$1

mkdir -p $DAY/input
touch $DAY/input/example.txt
touch $DAY/input/input.txt

cp template.cpp $DAY/first.cpp
cp template.cpp $DAY/second.cpp

echo "# day ${DAY}" >> CMakeLists.txt
echo "add_executable(${DAY}_first ${DAY}/first.cpp)" >> CMakeLists.txt
echo "add_executable(${DAY}_second ${DAY}/second.cpp)" >> CMakeLists.txt