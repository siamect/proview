#!/bin/bash
set -e

cd pwr
root=`eval pwd`

if [ -e $root/src/exp/inc/src/pwr_version.h ]; then
  verl=`eval cat $root/src/exp/inc/src/pwr_version.h | grep "\bpwrv_cPwrVersionStr\b" | awk '{print $3}'`
  verl=${verl:1:6}
  ver=${verl:0:2}${verl:3:1}${verl:5:1}
fi

# Setup pwre
export pwre_bin=$root/src/tools/pwre/src
export pwre_env_db=$root/pwre_db
source $pwre_bin/pwre_function

# Add pwre environment
unamestr=`eval uname`
machine=`eval uname -m`
if [ $machine == "amd64" ]; then
  machine="x86_64"
fi
if [ $machine != "x86_64" ]; then
  machine="x86"
fi

if [ $unamestr == "Darwin" ]; then
  os="macos"
  hw="x86_64"
elif [ $unamestr == "FreeBSD" ]; then
  os="freebsd"
  hw=$machine
else
  os="linux"
  hw=$machine
fi

btype="dbg"
ename=$ver$hw
desc=$verl
gui="gtk"

pwre init $ename
pwre module project
pwre build pwrdemo build src
