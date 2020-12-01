#!/bin/bash
set -e

exec > >(tee build.log)
exec 2>&1

root=`eval pwd`

declare -i buildversion_set=0
if [ "$1" = "--version" ] && [ "$2" != "" ] && [ "$3" != "" ]; then
    buildversion=$2" "$3
fi

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
export pwre_bin=$root/src/tools/pwre/src
export pwre_env_db=$root/pwre_db
source $pwre_bin/pwre_function

# Add pwre environment
unamestr=`eval uname`
machine=`eval uname -m`
if [ $machine == "armv7l" ]; then
  machine="arm"
elif [ $machine == "amd64" ]; then
  machine="x86_64"
else
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

if [ $gui == "qt" ]; then
  export pwre_conf_qt=1
elif [ $gui == "gtk" ]; then
  export pwre_conf_gtk=1
fi

pwre add $ename $root/src \"\" $root/rls $btype $os $hw "$desc"
pwre init $ename

# Build
mkdir -p $pwre_broot
if [ "$buildversion" != "" ]; then
  pwre configure --version $buildversion
fi
pwre configure --parallel
pwre create_all_modules
pwre build_all_modules $gui 
#pwre build_kernel $gui
#pwre method_build $gui


# Create a package version html file
$root/src/tools/pkg/deb/pwrrt/build.sh -v
co_convert -t -d $pwr_doc $pwr_eexe/xtt_version_help.dat

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo ""
echo "Add the following lines to your .bashrc-file"
echo ""
echo "export pwra_db=$aroot/db"
echo "source \$pwra_db/pwr_setup.sh"
echo "pwrp set base $verl"
echo "export PWR_BUS_ID=999"
echo ""
echo "Create a project with pwra"
echo ""
echo "> pwra"
echo ""
echo "Read Getting Started Guide on www.proview.se about how to create"
echo "and configure a project"
echo ""
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
