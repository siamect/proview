#!/bin/bash
#
# Import loadfile
# Arg1 import base root
# Arg2 target base root
#
tst=`eval echo $1 | grep :`
if [ -z $tst ]; then
  # Local import root, use cp
  copy="cp"
else
  # Remote import root, use scp
  copy="scp"
fi

$copy $1/exp/load/*.dbs $2/exp/load/
cp -p $2/exp/load/pwrs.dbs $2/rt/load/
cp -p $2/exp/load/pwrb.dbs $2/rt/load/
cp -p $2/exp/load/rt.dbs $2/rt/load/
cp -p $2/exp/load/basecomponent.dbs $2/bcomp/load/
cp -p $2/exp/load/nmps.dbs $2/nmps/load/
cp -p $2/exp/load/opc.dbs $2/opc/load/
cp -p $2/exp/load/profibus.dbs $2/profibus/load/
cp -p $2/exp/load/otherio.dbs $2/otherio/load/
cp -p $2/exp/load/remote.dbs $2/remote/load/
cp -p $2/exp/load/tlog.dbs $2/tlog/load/
cp -p $2/exp/load/abb.dbs $2/abb/load/
cp -p $2/exp/load/inor.dbs $2/inor/load/
cp -p $2/exp/load/klocknermoeller.dbs $2/klocknermoeller/load/
cp -p $2/exp/load/othermanufacturer.dbs $2/othermanu/load/
cp -p $2/exp/load/siemens.dbs $2/siemens/load/
cp -p $2/exp/load/ssabox.dbs $2/ssabox/load/
cp -p $2/exp/load/telemecanique.dbs $2/telemecanique/load/
