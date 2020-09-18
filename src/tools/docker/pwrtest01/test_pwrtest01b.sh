#!/bin/bash

echo "Here in test_pwrtest01b.sh"

Xorg -noreset +extension GLX +extension RANDR +extension RENDER -config ./dummy_display_xorg.conf :99 &

sleep 20
su - pwrp
cd /home/pwrp
source .bashrc
export DISPLAY=:99
export PWR_BUS_ID=999
export PYTHONPATH=$pwr_exe
pwr_pkg.sh -i /home/pwrp/pwrp_pkg_pwrtest01b_0001.tgz

# Rename boot, node and plcfiles
nname=`eval uname -n`

bootfile=`eval ls -1 $pwrp_load/ld_boot*.dat`
nodefile=`eval ls -1 $pwrp_load/ld_node*.dat`
plcfile=`eval ls -1 $pwrp_exe/plc_*_0999_plc`
echo "alias pwrtest01b $nname 172.18.0.23" > $pwrp_load/pwrp_alias.dat
echo "alias pwrtest01a pwrtest01a 172.18.0.22" >> $pwrp_load/pwrp_alias.dat
#mv $plcfile $pwrp_exe/"plc_"$nname"_0999_plc"

# Start project
rt_ini -i -n pwrtest01b&
sleep 10

export PYTHONPATH=$pwr_exe
ra_testall_b.sh

sleep 5