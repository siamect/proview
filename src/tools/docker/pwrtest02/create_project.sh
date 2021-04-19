#!/bin/bash
ver="V5.7"
pname="pwrtest02"
proot="/usr/pwrp/$pname"

ls -l ./pwrtest02.tar.gz

Xorg -noreset +extension GLX +extension RANDR +extension RENDER -config ./dummy_display_xorg.conf :99 &

sleep 20

export DISPLAY=:99
export pwra_db=/usr/pwrp/adm/db
cat $pwra_db/pwr_projectlist.dat
source $pwra_db/pwr_setup.sh
pwrp set base $ver


#if [ "$1" == "create" ]; then
  pwrp create project $pname $ver $proot "Wb test project"
  echo $pname $ver $proot $pname \"Wb test project\" >> $pwra_db/pwr_projectlist.dat
  echo Vol$pname 0.254.254.210 $pname >> $pwra_db/pwr_volumelist.dat
  echo CVol$pname 0.0.240.210 $pname >> $pwra_db/pwr_volumelist.dat
#fi

dir=`eval pwd`

source $pwra_db/pwra_env.sh set project $pname
cd $dir

echo "* Configure directory"
ls /configure_directory.pwr_com
ls -l $pwrp_db/directory.wb_load
wb_cmd @/configure_directory $pname
ls -l $pwrp_db/directory.wb_load

echo "* Configure class volume"
ls -l $pwrp_db/cvolpwrtest02.wb_load
ls -l /configure_cvolpwrtest02.pwr_com
wb_cmd -c cvolpwrtest02 @/configure_cvolpwrtest02
ls -l $pwrp_db/cvolpwrtest02.wb_load
wb_cmd -q create snapshot/file=\"$pwrp_db/cvol$pname.wb_load\"/out=\"$pwrp_load/cvol$pname.dbs\"
co_convert -so -d $pwrp_inc $pwrp_db/cvol$pname.wb_load 

echo "* Configure root volume"
ls -l $pwrp_db/volpwrtest02.db
ls -l /configure_volpwrtest02.pwr_com
wb_cmd -v vol$pname @/configure_volpwrtest02

echo "* Build node pwrt02"
wb_cmd -v vol$pname build node/name=pwrt02

mv /pwrtest02.tar.gz $pwrp_exe/
cd $pwrp_exe
tar -xzf pwrtest02.tar.gz

echo "* Run test programs"
cd $dir
export PYTHONPATH=$pwr_exe

/wa_testall.sh
