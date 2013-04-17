#!/bin/bash

# Get systemname from bootfile
zero="0000"
nname=`eval uname -n`
nname=${nname%%.*}

bootfile=$pwrp_load/ld_boot_${nname}_${zero:0:4-${#PWR_BUS_ID}}${PWR_BUS_ID}.dat

if [ ! -e "$bootfile" ]; then
  echo "Unable to find bootfile: $bootfile"
  return;
fi

let i=0
while read line; do
 if [ $i -eq 1 ]; then
   systemname=$line
   break;
 fi
 let i=$i+1
done < $bootfile

echo $systemname

export CLASSPATH=$pwr_lib/pwr_rt.jar:$pwr_lib/pwr_jop.jar:$pwr_lib/pwr_jopg.jar:$pwr_lib/pwr_jopc.jar:$pwr_lib/pwr_bcomp.jar:$pwr_lib/pwr_bcompfc.jar:$pwr_lib/pwr_abb.jar:$pwrp_lib/pwrp_$systemname.jar
export LD_LIBRARY_PATH=$pwr_exe
$jdk/bin/java jpwr.jop.JopOp
