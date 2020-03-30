#!/bin/bash

echo "Here in test_pwrtest01a.sh"

su - pwrp
cd /home/pwrp
pwd
source .bashrc
pwd
ls -l
export PWR_BUS_ID=999
pwr_pkg.sh -i /home/pwrp/pwrp_pkg_pwrtest01a_0001.tgz

# Rename boot, node and plcfiles
nname=`eval uname -n`

bootfile=`eval ls -1 $pwrp_load/ld_boot*.dat`
nodefile=`eval ls -1 $pwrp_load/ld_node*.dat`
plcfile=`eval ls -1 $pwrp_exe/plc_*_0999_plc`
echo "19-MAR-2020 17:06:54.32" > $pwrp_load/ld_boot_"$nname"_0999.dat
echo "pwrtest01" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "Common" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "plc_"$nname"_0999_plc" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "VolPwrTest01a 0.254.254.201" >> $pwrp_load/ld_boot_"$nname"_0999.dat
echo "$nname 0.254.254.201 172.17.0.2 0 0 0 0 600000 0.002000 0 0 - -" > $pwrp_load/ld_node_"$nname"_0999.dat
echo "pwrtest01b 0.254.254.202 172.17.0.3 0 0 0 0 600000 0.002000 0 0 - -" >> $pwrp_load/ld_node_"$nname"_0999.dat
mv $plcfile $pwrp_exe/"plc_"$nname"_0999_plc"

ip address

# Start project
rt_ini -i&
sleep 10
ps aux

i=0
while [ $i -lt 100 ]; do 
  ra_status
  sleep 2
  let i=i+1
done
