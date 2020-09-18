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

exec "wa_ldhtest volpwrtest02"
exec "wa_pwrwbtest.py"

