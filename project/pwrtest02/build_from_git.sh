#!/bin/bash
#
# Build pwrtest02 from git clone
#

cd $pwre_sroot/pwrtest02

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
platform=${hw:3}_${os:3}

#srcdir=$pwre_croot/src/tools/docker/pwrtest02
srcdir=/home/claes/wb

export pwrp_root=$pwre_croot/project/pwrtest02
broot=$pwre_broot/$os/$hw/bld/project/pwrtest02/bld
if [ ! -e $broot ]; then
  mkdir -p $broot
fi
if [ ! -e $pwrp_root/bld ]; then
  ln -s $broot $pwrp_root/bld
fi
export pwrp_exe=$pwrp_root/bld/$platform/exe
export pwrp_lib=$pwrp_root/bld/$platform/lib
export pwrp_obj=$pwrp_root/bld/$platform/obj
export pwrp_lis=$pwrp_root/bld/$platform/lis
export pwrp_inc=$pwrp_root/bld/common/inc
export pwrp_load=$pwrp_root/bld/common/load
export pwrp_tmp=$pwrp_root/bld/common/tmp
export pwrp_web=$pwrp_root/bld/common/web
export pwrp_log=$pwrp_root/bld/common/log

echo "* Create directories"
if [ ! -e $pwrp_obj ]; then
  mkdir -p $pwrp_obj
fi
if [ ! -e $pwrp_exe ]; then
  mkdir -p $pwrp_exe
fi
if [ ! -e $pwrp_lib ]; then
  mkdir -p $pwrp_lib
fi
if [ ! -e $pwrp_lis ]; then
  mkdir -p $pwrp_lis
fi
if [ ! -e $pwrp_load ]; then
  mkdir -p $pwrp_load
fi
if [ ! -e $pwrp_inc ]; then
  mkdir -p $pwrp_inc
fi
if [ ! -e $pwrp_lis ]; then
  mkdir -p $pwrp_lis
fi
if [ ! -e $pwrp_tmp ]; then
  mkdir -p $pwrp_tmp
fi
if [ ! -e $pwrp_web ]; then
  mkdir -p $pwrp_web
fi
if [ ! -e $pwrp_log ]; then
  mkdir -p $pwrp_log
fi

export pwrp_db=$pwre_sroot/pwrtest02/src/db
export pwrp_pop=$pwre_sroot/pwrtest02/src/pop
if [ ! -e $pwrp_db ]; then
  mkdir -p $pwrp_db
fi
if [ ! -e $pwrp_pop ]; then
  mkdir -p $pwrp_pop
fi

echo "* Configure adm"
export pwra_db=$pwre_broot/$os/$hw/bld/project/adm/db
if [ ! -e $pwra_db ]; then
  mkdir -p $pwra_db
  cp $pwre_croot/src/tools/pkg/deb/adm/* $pwra_db/
fi
if [ "`eval grep CVolPwrtest02 $pwra_db/pwr_volumelist.dat`" == "" ]; then
  echo "CVolPwrtest02	0.0.240.210	pwrtest02" >> $pwra_db/pwr_volumelist.dat
fi
if [ "`eval grep VolPwrTest02 $pwra_db/pwr_volumelist.dat`" == "" ]; then
  echo "VolPwrTest02	0.254.254.210	pwrtest02" >> $pwra_db/pwr_volumelist.dat
fi
if [ ! -e $pwra_db/pwr_projectlist.dat ] || [ "`eval grep base $pwra_db/pwr_projectlist.dat`" == "" ]; then
  echo "%base V0.0.0 $pwre_broot" > $pwra_db/pwr_projectlist.dat
fi
if [ "`eval grep pwrtest02 $pwra_db/pwr_projectlist.dat`" == "" ]; then
  echo "pwrtest02	V0.0.0	$pwrp_root	Test-pwrtest02	\"\"" >> $pwra_db/pwr_projectlist.dat
fi
source $pwra_db/pwr_setup.sh
source $pwra_db/pwra_env.sh set project pwrtest02

#set -o xtrace

echo "* Configure directory volume"
wb_cmd create volume/database=wbload/directory
wb_cmd @$srcdir/configure_directory pwrtest02

echo "* Configure class volume"
ls $srcdir/configure_cvolpwrtest02.pwr_com
wb_cmd -c cvolpwrtest02 @$srcdir/configure_cvolpwrtest02
wb_cmd -q create snapshot/file=\"$pwrp_db/cvolpwrtest02.wb_load\"/out=\"$pwrp_load/cvolpwrtest02.dbs\"
co_convert -so -d $pwrp_inc $pwrp_db/cvolpwrtest02.wb_load

echo "* Configure root volume"
wb_cmd -v volpwrtest02 @$srcdir/configure_volpwrtest02
wb_cmd -v volpwrtest02 build node/name=pwrt02 
 
echo "* Build test programs"
cp $srcdir/wa_ldhtest.cpp $pwrp_appl
cp $srcdir/wa_ldhtest.h $pwrp_appl
cp $srcdir/wa_pwrwbtest.py $pwrp_appl
cp $srcdir/ra_test.py $pwrp_appl
cp $srcdir/makefile $pwrp_appl
cd $pwrp_appl
make

echo "* Run test programs"
export PYTHONPATH=$pwr_eexe
wa_ldhtest volpwrtest02
wa_pwrwbtest.py

echo "* Build package"
cd $pwrp_exe
tar -czf $pwrp_load/pwrtest02.tar.gz wa_ldhtest wa_pwrwbtest.py ra_test.py 
exit

echo "* Set opsys $opsys"
cat > initdir.pwr_com <<EOF 
  set attr/name=Bus999-PwrTest02/attr=OperatingSystem/value="$opsys"/noconf
  save
  exit
EOF

wb_cmd @initdir

echo "* Build class volume"
co_convert -so -d $pwrp_inc $pwrp_db/cvolpwrtest02.wb_load
co_convert -po -d $pwrp_inc $pwrp_db/cvolpwrtest02.wb_load
wb_cmd -q create snapshot/file=\"$pwrp_db/cvolpwrtest02.wb_load\"/out=\"$pwrp_load/cvolpwrtest02.dbs\"

echo "* Load volpwrtest01a"
wb_cmd wb load/nofocode/load=\"$pwrp_db/volpwrtest01a.wb_dmp\"/out=\"$pwrp_db/volpwrtest01a.lis\"

echo "* Set opsys $opsys"

cat > initvol.pwr_com <<EOF 
  set attr/name=VolPwrTest02:/attr=OperatingSystem/value="$opsys"/noconf
  save
  exit
EOF

wb_cmd -v volpwrtest02 @initvol

wb_cmd -v volpwrtest02 build node pwrtest02 /force

echo "* Build package"
wb_cmd distr/node=pwrtest02/package

exit


