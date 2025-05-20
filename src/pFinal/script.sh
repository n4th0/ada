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
  echo $solu

done
