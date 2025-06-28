#!/bin/bash

# make

execute() {
  # echo "------------------------------"
  echo "test "$1

  touch /tmp/prueba
  touch /tmp/prueba2

  ./maze_bb -f "tests/"$1"-bb.maze" | head -n 1 >/tmp/prueba
  cat "tests/"$1"-bb.maze.sol_bb" | head -n 1 >/tmp/prueba2

  diff /tmp/prueba /tmp/prueba2

  # echo " +"
  # echo "----------------"
  # cat "tests/"$1"-bb.maze.sol_bb" | head -n 3
}
# "13" "100" "12" "13" "100"

for i in "07" "08" "09" "10" "11" "12" "13" "100" "200" "300" "500" "700" "900" "k01" "k02" "k03" "k05" "k10"; do
  # echo "scale=10; ("
  # for ((j = 0; j < 30; j++)); do
  execute $i
  # done
  # echo "0)/30|"
done
