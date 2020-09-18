#!/bin/bash

exec()
{
    echo "Executing $1"
    $1
}

# Spawn process
dexec()
{
    echo "Executing $1"
    $1 &
    pid[$i]=$!
    i=$i+1
}

let i=0

dexec "ra_nethtest"
exec "ra_qcom2test"

# Wait for spawned processes
for p in ${pid[*]}; do
  wait $p
done
