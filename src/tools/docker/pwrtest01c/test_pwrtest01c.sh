#!/bin/bash

echo "Here in test_pwrtest01b.sh"

Xorg -noreset +extension GLX +extension RANDR +extension RENDER -config ./dummy_display_xorg.conf :99 &

sleep 20
su - pwrp
cd /home/pwrp
pwd
source .bashrc
export DISPLAY=:99
pwd
ls -l
export PWR_BUS_ID=999
export PYTHONPATH=$pwr_exe
pwr_pkg.sh -i /home/pwrp/pwrp_pkg_pwrtest01c_0001.tgz

# Rename boot, node and plcfiles
nname=`eval uname -n`

bootfile=`eval ls -1 $pwrp_load/ld_boot*.dat`
nodefile=`eval ls -1 $pwrp_load/ld_node*.dat`
plcfile=`eval ls -1 $pwrp_exe/plc_*_0999_plc`
echo "19-MAR-2020 17:06:54.32" > $pwrp_load/ld_boot_"$nname"_0999.dat
echo "pwrtest01" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "Common" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "plc_"$nname"_0999_plc" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "VolPwrTest01c 0.254.254.203" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "$nname 0.254.254.203 127.0.0.1 0 0 0 0 600000 0.002000 0 0 - -" > $pwrp_load/ld_node_"$nname"_0999.dat
mv $plcfile $pwrp_exe/"plc_"$nname"_0999_plc"

ip address

# Start project
rt_ini -i&
sleep 10
ps aux

export PYTHONPATH=$pwr_exe
ra_testall_c.sh
ls -l $pwrp_log

sleep 3