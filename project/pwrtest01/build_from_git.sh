#!/bin/bash
#
# Build pwrtest01 from git clone
#

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

export pwrp_root=$pwre_croot/project/pwrtest01
broot=$pwre_broot/$os/$hw/bld/project/pwrtest01/bld
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

echo "* Configure adm"
export pwra_db=$pwre_broot/$os/$hw/bld/project/adm/db
if [ ! -e $pwra_db ]; then
  mkdir -p $pwra_db
  cp $pwre_croot/src/tools/pkg/deb/adm/* $pwra_db/
fi
if [ ! -e $pwra_db ]; then
  mkdir -p $pwra_db
  cp $pwre_croot/src/tools/pkg/deb/adm/* $pwra_db/
fi
if [ "`eval grep CVolPwrtest01 $pwra_db/pwr_volumelist.dat`" == "" ]; then
  echo "CVolPwrtest01	0.0.240.254	pwrtest01" >> $pwra_db/pwr_volumelist.dat
fi
if [ "`eval grep VolPwrTest01a $pwra_db/pwr_volumelist.dat`" == "" ]; then
  echo "VolPwrTest01a	0.254.254.201	pwrtest01" >> $pwra_db/pwr_volumelist.dat
fi
if [ "`eval grep VolPwrTest01b $pwra_db/pwr_volumelist.dat`" == "" ]; then
  echo "VolPwrTest01b	0.254.254.202	pwrtest01" >> $pwra_db/pwr_volumelist.dat
fi
if [ "`eval grep VolPwrTest01c $pwra_db/pwr_volumelist.dat`" == "" ]; then
  echo "VolPwrTest01c	0.254.254.203	pwrtest01" >> $pwra_db/pwr_volumelist.dat
fi
if [ "`eval grep VolPwrTest01d $pwra_db/pwr_volumelist.dat`" == "" ]; then
  echo "VolPwrTest01d	0.254.254.204	pwrtest01" >> $pwra_db/pwr_volumelist.dat
fi
if [ ! -e $pwra_db/pwr_projectlist.dat ] || [ "`eval grep base $pwra_db/pwr_projectlist.dat`" == "" ]; then
  echo "%base V0.0.0 $pwre_broot" > $pwra_db/pwr_projectlist.dat
fi
if [ "`eval grep pwrtest01 $pwra_db/pwr_projectlist.dat`" == "" ]; then
  echo "pwrtest01	V0.0.0	$pwrp_root	Test-pwrtest01	\"\"" >> $pwra_db/pwr_projectlist.dat
fi

source $pwra_db/pwr_setup.sh
source $pwra_db/pwra_env.sh set project pwrtest01


echo "* Set opsys $opsys"
cat > initdir.pwr_com <<EOF 
  set attr/name=Bus999-PwrTest01a/attr=OperatingSystem/value="$opsys"/noconf
  set attr/name=Bus999-PwrTest01b/attr=OperatingSystem/value="$opsys"/noconf
  set attr/name=Bus999-PwrTest01c/attr=OperatingSystem/value="$opsys"/noconf
  set attr/name=Bus999-PwrTest01d/attr=OperatingSystem/value="$opsys"/noconf
  save
  exit
EOF

wb_cmd @initdir

echo "* Build class volume"
co_convert -so -d $pwrp_inc $pwrp_db/cvolpwrtest01.wb_load
co_convert -po -d $pwrp_inc $pwrp_db/cvolpwrtest01.wb_load
wb_cmd -q create snapshot/file=\"$pwrp_db/cvolpwrtest01.wb_load\"/out=\"$pwrp_load/cvolpwrtest01.dbs\"

echo "* Load volpwrtest01a"
wb_cmd wb load/nofocode/load=\"$pwrp_db/volpwrtest01a.wb_dmp\"/out=\"$pwrp_db/volpwrtest01a.lis\"

echo "* Set opsys $opsys"

cat > initvol.pwr_com <<EOF 
  set attr/name=VolPwrTest01a:/attr=OperatingSystem/value="$opsys"/noconf
  save
  exit
EOF

wb_cmd -v volpwrtest01a @initvol

wb_cmd -v volpwrtest01a build node pwrtest01a /force

echo "* Build package"
wb_cmd distr/node=pwrtest01a/package

echo "* Load volpwrtest01b"
wb_cmd wb load/nofocode/load=\"$pwrp_db/volpwrtest01b.wb_dmp\"/out=\"$pwrp_db/volpwrtest01b.lis\"

echo "* Set opsys $opsys"

cat > initvol.pwr_com <<EOF 
  set attr/name=VolPwrTest01b:/attr=OperatingSystem/value="$opsys"/noconf
  save
  exit
EOF

wb_cmd -v volpwrtest01b @initvol

wb_cmd -v volpwrtest01b build node pwrtest01b /force

echo "* Build package 01b"
wb_cmd distr/node=pwrtest01b/package

echo "* Load volpwrtest01c"
wb_cmd wb load/nofocode/load=\"$pwrp_db/volpwrtest01c.wb_dmp\"/out=\"$pwrp_db/volpwrtest01c.lis\"

echo "* Set opsys $opsys"

cat > initvol.pwr_com <<EOF 
  set attr/name=VolPwrTest01c:/attr=OperatingSystem/value="$opsys"/noconf
  save
  exit
EOF

wb_cmd -v volpwrtest01c @initvol

wb_cmd -v volpwrtest01c build node pwrtest01c /force

echo "* Build package"
wb_cmd distr/node=pwrtest01c/package

echo "* Load volpwrtest01d"
wb_cmd wb load/nofocode/load=\"$pwrp_db/volpwrtest01d.wb_dmp\"/out=\"$pwrp_db/volpwrtest01d.lis\"

echo "* Set opsys $opsys"

cat > initvol.pwr_com <<EOF 
  set attr/name=VolPwrTest01d:/attr=OperatingSystem/value="$opsys"/noconf
  save
  exit
EOF

wb_cmd -v volpwrtest01d @initvol

wb_cmd -v volpwrtest01d build node pwrtest01d /force

echo "* Build package"
wb_cmd distr/node=pwrtest01d/package

exit

echo "* Copy pwg files"
cp $pwrp_pop/*.pwg $pwrp_exe/
cp $pwrp_pop/*.pwg $pwrp_web/
# Objects in level and temp control

echo "* Copy image files"
cp $pwrp_pop/*.png $pwrp_exe/
cp $pwrp_pop/*.png $pwrp_web/

echo "* Copy trace files"
cp $pwrp_load/*.flw $pwrp_web/

echo "* Copy jar files"
cp $pwr_lib/pwr_rt_client.jar $pwrp_web/
cp $pwr_lib/pwr_jop.jar $pwrp_web/
cp $pwr_lib/pwr_jopg.jar $pwrp_web/
cp $pwr_load/../web/* $pwrp_web/

