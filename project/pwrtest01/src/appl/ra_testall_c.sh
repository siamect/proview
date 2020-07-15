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

exec "ra_plctest"
exec "ra_timetest"
exec "ra_nettimetest"
exec "ra_cdhtest"
exec "ra_gdhtest"
exec "ra_errhtest"
exec "ra_aproctest"
exec "ra_mhtest"
exec "ra_mhappltest"
exec "ra_qcomtest"
exec "ra_pwrrttest.py"
exec "rt_xtt_cmd @$pwrp_exe/ra_ccmtest"
exec "rt_xtt_cmd @$pwrp_exe/ra_xttscripttest"
dexec "ra_qmontest"
exec "ra_qmontest 2"

