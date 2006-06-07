#!/bin/bash
root=`eval pwd`

if [ -e $root/src/exp/inc/src/pwr_version.h ]; then
  verl=`eval cat $root/src/exp/inc/src/pwr_version.h | grep "\bpwrv_cPwrVersionStr\b" | awk '{print $3}'`
  verl=${verl:1:6}
  ver=${verl:0:2}${verl:3:1}${verl:5:1}
fi

# Check pwra
aroot=/usr/local/adm
if [ ! -e $aroot/db ]; then
  if mkdir -p $aroot/db 2>/dev/null; then
    echo "Administration directory created: $aroot/db"
    cp -u $root/src/tools/pkg/deb/adm/pwr* $aroot/db/
  else
    echo "Unable to create a global administration directory $aroot/db"
    aroot=$root/adm
    echo "Using local administration directory $aroot/db"
    if mkdir -p $aroot/db; then
      cp -u $root/src/tools/pkg/deb/adm/pwr* $aroot/db/
    else
      echo "Unable to create local administration directory $aroot/db"
    fi
  fi
fi

# Insert base in projectlist
if [ ! -e $aroot/db/pwr_projectlist.dat ]; then
  echo "Creating $aroot/db/pwr_projectlist.dat"
  echo "%base $verl $root/rls" > $aroot/db/pwr_projectlist.dat
else
  set +e
  ptst=`eval grep "^%base" $aroot/db/pwr_projectlist.dat | grep "\b$verl\b"`
  set -e
  if [ "$ptst" = "" ]; then
    echo "Adding base to $aroot/db/pwr_projectlist.dat"
    echo "%base $verl $root/rls" >> $aroot/db/pwr_projectlist.dat
  else
    echo "Base $verl already exist in $aroot/db/pwr_projectlist.dat"
  fi
fi

# Setup pwre
export pwre_bin=$root/src/tools/pwre/src/os_linux
export pwre_env_db=$root/pwre_db
source $pwre_bin/pwre_function

# Add pwre environment
btype="dbg"
os="linux"
hw="x86"
ename=$ver$hw
desc=$verl


pwre add $ename $root/src $root/rls $btype $os $hw "$desc"
pwre init $ename

# Build
pwre create_all_modules
pwre build_all_modules


echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo ""
echo "Add the following lines to your .bashrc-file"
echo ""
echo "export pwra_db=$aroot/db"
echo "source \$pwra_db/pwr_setup.sh"
echo "pwrp set base $verl"
echo ""
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
