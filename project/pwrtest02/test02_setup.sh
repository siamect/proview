#!/bin/bash
nodename=`eval uname -n`
unamestr=`eval uname`
onamestr=`eval uname -o`
machine=`eval uname -m`
if [ $unamestr == "Darwin" ]; then
    os="os_macos"
    hw="hw_x86_64"
    opsys=256
elif [ $unamestr == "FreeBSD" ]; then
    os="os_freebsd"
    hw="hw_x86_64"
    opsys=1024
elif [ $unamestr == "OpenBSD" ]; then
    os="os_openbsd"
    hw="hw_x86_64"
    opsys=2048
elif [ $onamestr == "Cygwin" ]; then
    os="os_cygwin"
    hw="hw_x86"
    opsys=4096
else
    if [ $machine == "x86_64" ]; then
	machine="x86_64"
        opsys=128
    elif [ ${machine:0:3} == "arm" ]; then
	machine="arm"
        opsys=512
    else
	machine="x86"
        opsys=64
    fi
    os="os_linux"  
    hw="hw_"$machine
fi

export pwra_db=$pwre_broot/$os/$hw/bld/project/adm/db
source $pwra_db/pwr_setup.sh
source $pwra_db/pwra_env.sh set project pwrtest02
