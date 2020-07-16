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

btype="dbg"
ename=$ver$hw"rpi"
desc=$verl
gui="gtk"

if [ $gui == "qt" ]; then
  export pwre_conf_qt=1
elif [ $gui == "gtk" ]; then
  export pwre_conf_gtk=1
fi

pwre add $ename $root/src $root/rls/os_$os/hw_$hw $root/rls $btype $os arm "$desc"
pwre init $ename

export jdk=/usr/lib/jvm/java-11-openjdk-amd64

export pwre_cc=arm-linux-gnueabihf-gcc
export pwre_cxx=arm-linux-gnueabihf-g++
export pwre_ar=arm-linux-gnueabihf-ar
export pwre_host_exe=$pwre_broot/os_$os/hw_$hw/exp/exe

pwre configure --ebuild
pwre create all
pwre import rt
pwre ebuild rt
