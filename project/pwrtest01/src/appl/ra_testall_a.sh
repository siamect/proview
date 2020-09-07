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
}

exec "ra_nethtest"

