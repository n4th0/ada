#!/bin/bash

make

for ((i = 0; i < 14; i++)); do
  test="tests/"
  solu="tests/"

  if [[ i -lt 10 ]]; then
    test=$test"0$i-bb.maze"
    solu=$solu"0$i-bb.maze.sol_bb"
  else
    test=$test"$i-bb.maze"
    solu=$solu"$i-bb.maze.sol_bb"
  fi

  echo "------------------------------"
  echo "test "$i
  ./maze_bb -f $test
  echo "----------------"
  cat $solu | head -n 3

done

execute() {
  echo "------------------------------"
  echo "test "$1
  ./maze_bb -f "tests/"$1"-bb.maze"
  echo "----------------"
  cat "tests/"$1"-bb.maze.sol_bb" | head -n 3
}

execute "200"
execute "300"
execute "500"
execute "700"
execute "900"
execute "k01"
execute "k02"
execute "k03"
execute "k05"
execute "k10"
