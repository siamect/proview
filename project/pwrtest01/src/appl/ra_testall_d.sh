#!/bin/bash

exec()
{
    echo "Executing $1"
    $1
}

dexec()
{
    echo "Executing $1"
    $1 &
    pid[$i]=$!
    i=$i+1
}

let i=0

exec "ra_sevtest.py"

# Wait for spawned processes
for p in ${pid[*]}; do
  wait $p
done
