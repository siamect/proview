#!/bin/bash

echo "Here in test_pwrtest01a.sh"

Xorg -noreset +extension GLX +extension RANDR +extension RENDER -config ./dummy_display_xorg.conf :99 &

sleep 20
source /home/pwrp/.bashrc
pwr_pkg.sh -i /home/pwrp/pwrp_pkg_pwrtest01a_0001.tgz
chown root $pwrp_exe/plc_pwrtest01a_0999_plc
chmod u+s $pwrp_exe/plc_pwrtest01a_0999_plc
ls -l $pwrp_exe/plc_pwrtest01a_0999_plc

su - pwrp
cd /home/pwrp
source .bashrc
export DISPLAY=:99
export PWR_BUS_ID=999
export PYTHONPATH=$pwr_exe

# Rename boot, node and plcfiles
nname=`eval uname -n`

bootfile=`eval ls -1 $pwrp_load/ld_boot*.dat`
nodefile=`eval ls -1 $pwrp_load/ld_node*.dat`
plcfile=`eval ls -1 $pwrp_exe/plc_*_0999_plc`
echo "alias pwrtest01a $nname 172.18.0.22" > $pwrp_load/pwrp_alias.dat
echo "alias pwrtest01b pwrtest01b 172.18.0.23" >> $pwrp_load/pwrp_alias.dat
#mv $plcfile $pwrp_exe/"plc_"$nname"_0999_plc"

# Start project
rt_ini -i -n pwrtest01a&
sleep 10

#echo "Nodes-Pwrtest01a-IO-MbServer.Status:"
#rt_gdhget Nodes-Pwrtest01a-IO-MbServer.Status
#echo "Nodes-Pwrtest01a-IO-MbServer.Connections:"
#rt_gdhget Nodes-Pwrtest01a-IO-MbServer.Connections

export PYTHONPATH=$pwr_exe
ra_testall_a.sh

sleep 70
#echo "Nodes-Pwrtest01a-IO-MbServer.RX_packets:"
#rt_gdhget Nodes-Pwrtest01a-IO-MbServer.RX_packets
#echo "Nodes-Pwrtest01a-IO-MbServer.TX_packets:"
#rt_gdhget Nodes-Pwrtest01a-IO-MbServer.TX_packets
#cat $pwrp_log/pwr*.log | grep plc