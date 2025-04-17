#!/bin/bash

make

for ((i = 0; i < 14; i++)); do
  file=""

  if [[ i -lt 10 ]]; then
    file="./tests/0$i.maze"

  else
    file="tests/$i.maze"
  fi

  echo "---------"
  ./maze_greedy -f $file --p2D | head

  file=$file".sol_greedy"

  # diff /tmp/test $file
  cat $file | head
  echo "---------"

done
