#!/bin/bash

echo "Here in test_pwrdemo.sh"

su - pwrp
cd /home/pwrp
source .bashrc
source $pwra_db/pwra_env.sh set project pwrdemo57
export PWR_BUS_ID=999

# Rename boot, node and plcfiles
nname=`eval uname -n`

bootfile=`eval ls -1 $pwrp_load/ld_boot*.dat`
nodefile=`eval ls -1 $pwrp_load/ld_node*.dat`
plcfile=`eval ls -1 $pwrp_exe/plc_*_0999_plc | sed -n '1p'`
echo "19-MAR-2020 17:06:54.32" > $pwrp_load/ld_boot_"$nname"_0999.dat
echo "pwrdemo" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "Common" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "plc_"$nname"_0999_plc" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "VolPwrDemo 0.254.254.200" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "$nname 0.254.254.200 127.0.0.1 0 0 0 0 600000 0.002000 0 0 - -" > $pwrp_load/ld_node_"$nname"_0999.dat
echo "old plcfile $plcfile"
echo "new plcfile $pwrp_exe/plc_"$nname"_0999_plc"
mv $plcfile $pwrp_exe/"plc_"$nname"_0999_plc"

# Start project
rt_ini&
sleep 10
ps aux
ra_status
