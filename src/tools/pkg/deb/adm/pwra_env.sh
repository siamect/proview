#! /bin/bash
#
#
#

pwra_set_func()
{
  local cmd
  local baseroot
  local project

  if [ -z $1 ]; then
    echo "Qualifier is missing"
    return
  fi

  cmd="base"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "set base"
    basename=$2
    basename=${basename//\./\\.}
    baseroot=`eval cat $pwra_db/pwr_projectlist.dat | grep "[[:<:]]"$basename"[[:>:]]" | grep "^"%base"[[:>:]]" | awk '{print $3}'`
    if [ -z $baseroot ]; then
      echo "Unable to find base '$2'"
      return
    fi

    baseroot=${baseroot%/}
    if [ ! -e "$baseroot" ]; then
      echo "Base $baseroot doesn't exist"
    elif [ "$3" != "cross" ]; then
      if [ ! -e $baseroot/$os/$hw/exp/exe/pwrp_env.sh ]; then
        echo "Not a base directory: $baseroot"
        return
      fi
      source $baseroot/$os/$hw/exp/exe/pwrp_env.sh set baseroot $baseroot
    fi
    return
  fi

  cmd="baseroot"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "set baseroot"
    baseroot=$2
    baseroot=${baseroot%/}
    if [ ! -e "$baseroot" ]; then
      echo "Base $baseroot doesn't exist"
    else
      if [ ! -e $baseroot/$os/$hw/exp/exe/pwrp_env.sh ]; then
        echo "Not a base directory: $baseroot"
        return
      fi
      source $baseroot/$os/$hw/exp/exe/pwrp_env.sh set baseroot $baseroot
    fi
    return
  fi

  cmd="project"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "set project"

    project=$2
    basename=`eval cat $pwra_db/pwr_projectlist.dat | grep "^"$project"[[:>:]]" | awk '{print $2}'`
    basename=${basename//\./\\.}
    if [ -z $basename ]; then
      echo "Unable to find project '$2'"
      return
    fi
    
    baseroot=`eval cat $pwra_db/pwr_projectlist.dat | grep "[[:<:]]"$basename"[[:>:]]" | grep "^"%base"[[:>:]]" | awk '{print $3}'`
    if [ -z $baseroot ]; then
      echo "Unable to find base '$2'"
      return
    fi

    baseroot=${baseroot%/}
    if [ ! -e "$baseroot" ]; then
      echo "Base $baseroot doesn't exist"
    else
      if [ ! -e $baseroot/$os/$hw/exp/exe/pwrp_env.sh ]; then
        echo "Not a base directory: $baseroot"
        return
      fi
      source $baseroot/$os/$hw/exp/exe/pwrp_env.sh set baseroot $baseroot
      source $baseroot/$os/$hw/exp/exe/pwrp_env.sh set project $project
      export PS1='\u@\h/$pwrp_projectname/-.\W> '    
    fi
    return
  fi

  if [ -z $pwr_exe ]; then
    echo "No project is defined"
    return
  fi
  source $pwr_exe/pwrp_env.sh set $1 $2 $3

}  


pwra_help_func()
{

  source $pwr_exe/pwrp_env.sh $@

  cat << EOF

pwra_env.sh  		- Utilities for pwr project environment

help			- Display help
set base 'distribution'	- Setup environment to proview base distribution
set baseroot 'root'	- Setup environment to proview base distribution
EOF

}


pwra_parse ()
{
  unamestr=`eval uname`
  machine=`eval uname -m`
  if [ $unamestr == "Darwin" ]; then
    os="os_macos"
    hw="hw_x86_64"
  elif [ $unamestr == "FreeBSD" ]; then
    os="os_freebsd"
    hw="hw_x86_64"
  elif [ $unamestr == "OpenBSD" ]; then
    os="os_openbsd"
    hw="hw_x86_64"
  else
    if [ $machine != "x86_64" ]; then
      machine="x86"
    fi
    os="os_linux"  
    hw="hw_"$machine
  fi

  local cmd

  cmd="help"
  if [ -z $1 ] || [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    pwra_help_func $2 $3
    return
  fi

  cmd="set"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    pwra_set_func $2 $3
    return
  fi

  if [ -z $pwr_exe ]; then
    echo "No project is defined"
    return
  fi
  source $pwr_exe/pwrp_env.sh $@

}

pwra_parse $@
