#! /bin/bash
#
# Proview   $Id: pwrp_env.sh,v 1.10 2006-01-31 09:10:21 claes Exp $
# Copyright (C) 2005 SSAB Oxelösund AB.
#
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License as 
# published by the Free Software Foundation, either version 2 of 
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with the program, if not, write to the Free Software 
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#
#
#
declare -ix pwrc_status=$pwrc__success
declare -i pwrc__success=0
declare -i pwrc__dbnotfound=1
declare -i pwrc__noproj=3
declare -i pwrc__rootexist=4
declare -i pwrc__notconfigured=5
declare -i pwrc__projlistfile=6
declare -i pwrc__projalrexist=7
declare -i pwrc__basealrexist=8
declare -i pwrc__nobase=9
declare -i pwrc__projlistfileacc=10
declare -i pwrc__syntax=11
declare -i pwrc__rootacc=12
declare -i pwrc__rootcreate=13
declare -i pwrc__rootdelete=14
declare -i pwrc__dbdelete=15
declare -i pwrc__baseexist=16
declare -i pwrc__move=17
declare -i pwrc__notmysqlsrv=18
declare -i pwrc__datadir=19
declare -i pwrc__nodb=20
declare -i pwrc__notemplatedb=21
declare -i pwrc__dbalrexist=22
declare -i pwrc__copy=23
declare -i pwrc__dbcreate=24
declare -i pwrc__projnotset=25
declare -i pwrc__projnameinv=26
declare -i pwrc__mysqldbcopy=27
declare -i pwrc__mysqldbrena=28
declare -i pwrc__mysql=29
declare -i pwrc__noroot=30


pwrc_get_variables()
{
  local socket
  
  if [ -e /etc/proview.cnf ]; then
    socket=`eval cat /etc/proview.cnf | grep "\bmysql_socket\b" | awk '{print $2}'`
    if [ ! -z $socket ]; then
      mysql_socket="--socket $socket"
    fi

    pwr_projectroot=`eval cat /etc/proview.cnf | grep "\bprojectroot\b" | awk '{print $2}'`
    if [ -z $pwr_projectroot ]; then
      pwr_projectroot=/home/pwrp
    fi
  fi
  
  if [ -e $pwr_inc/pwr_version.h ]; then
#    pwr_dbversion=`eval grep "\bpwrv_cWbdbVersionShortStr\b" $pwr_inc/pwr_version.h | awk '{print $2}'`
    pwr_dbversion=`eval cat $pwr_inc/pwr_version.h | grep "\bpwrv_cWbdbVersionShortStr\b" | awk '{print $3}'`

    if [ -z $pwr_dbversion ]; then
      echo "Unable to get pwr db version"
    fi
    pwr_dbversion=${pwr_dbversion:1:3}
  fi
}

#
# Check project name syntax
#
# Arguments $1 project name
# Returns pwrc_status: 0 Success
#
pwrc_check_project_name()
{
  local pname=$1
  
  if [ "$pname" != "${pname/_/}" ]; then
    pwrc_status=$pwrc__projnameinv
    return
  fi
  pwrc_status=$pwrc__success
}

#
# Get the databases in a project
#
# Arguments 1: project name
#           2: project root
# Returns pwrc_status : 0 Success
#                   
#
pwrc_dblist_read()
{
  local volume
  local volid
  local dbid
  local volclass
  local pname=$1
  local proot=$2
  local dbfile
  local db
  let i=0
  let j=0
  let found=0

  pwrc_status=$pwrc__success

  if [ -n "${db_array[*]}" ]; then
    unset db_array
  fi

  # Add directory db
  db_array[$i]=$pname"_"$pwr_dbversion
  i=$i+1
  
  dbfile=$proot/common/db/pwrp_cnf_volumelist.dat
  if [ ! -e $dbfile ]; then
    echo "Can't find $dbfile"
    pwrc_status=$pwrc__notconfigured
    return
  fi

  while read volume volid dbid volclass; do
    if [ -n "$volume" ] && [ "${volume:0:1}" != "!" ]; then      
      db=$pname"_"$dbid"_"$pwr_dbversion

      # Check that the db is not already inserted
      found=0
      j=0
      while [ "${db_array[$j]}" != "" ]; do
        if [ "${db_array[$j]}" == $db ]; then
          found=1
          break;
        fi
        j=$j+1
      done
      
      if [ $found -eq 0 ]; then
        db_array[$i]=$db
        i=$i+1
      fi
    fi
  done < $dbfile
}


#
# Read the project list
#
# Arguments No
# Returns pwrc_status : 0 Success
#                       1 File not found
#
pwrc_prlist_read()
{
  local name
  local base
  local root
  local path
  local descr

  if [ -n "${name_array[*]}" ]; then
    unset name_array
    unset base_array
    unset root_array
    unset path_array
    unset desc_array
  fi
  if [ -n "${b_name_array[*]}" ]; then
    unset b_name_array
    unset b_root_array
  fi
  
  if [ ! -e $pwra_db/pwr_projectlist.dat ]; then
    echo "Can't find projectlist file"
    pwrc_status=$pwrc__projlistfile
    return
  fi
  
  let i=0
  let j=0
  while read name base root path descr; do
    if [ -n "$name" ] && [ "${name:0:1}" != "!" ] && [ "${name:0:1}" != "#" ]; then
      if [ "$name" == "%base" ]; then
        b_name_array[$j]=$base
	b_root_array[$j]=$root
        j=$j+1
      else
        name_array[$i]=$name
        base_array[$i]=$base
        root_array[$i]=$root
        path_array[$i]=$path
        descr=${descr#\"}
        descr=${descr%\"}
        desc_array[$i]=$descr
        i=$i+1
      fi
    fi
      
  done < $pwra_db/pwr_projectlist.dat

  pwrc_status=$pwrc__success
}

#
# List the project list
#
# Arguments No
# Returns nothing
#
pwrc_prlist_list()
{  
  let i=0
  local blank24="                        "
  local blank16="                "
  local blank8="        "

  echo ""
  while [ "${name_array[$i]}" != "" ]; do
    if [ "${base_array[$i]}" != "" ]; then
      if [ -n "$1" ]; then
        echo -n ${name_array[$i]} "${blank16:${#name_array[$i]}} "
        echo -n ${base_array[$i]} "${blank8:${#base_array[$i]}} "
        echo -n ${root_array[$i]} "${blank24:${#root_array[$i]}} "
        echo ${path_array[$i]} "${blank16:${#path_array[$i]}}"
#        echo ${desc_array[$i]}
      else
        echo ${name_array[$i]} "${blank16:${#name_array[$i]}} " ${desc_array[$i]}
      fi
    fi
    i=$i+1
  done
  echo ""
}

pwrc_prlist_list_project()
{  
  let i=0
  local blank24="                        "
  local blank14="              "
  local blank6="      "

  while [ "${name_array[$i]}" != "" ]; do
    if [ "${base_array[$i]}" != "" ]; then
      if [ $1 == ${name_array[$i]} ]; then
        echo -n ${name_array[$i]} "${blank14:${#name_array[$i]}} "
        echo -n ${base_array[$i]} "${blank6:${#base_array[$i]}} "
        echo ${desc_array[$i]}
      fi
    fi
    i=$i+1
  done
}

pwrc_prlist_get_index()
{
  let i=0

  pwrc_status=$pwrc__noproj

  while [ "${name_array[$i]}" != "" ]; do
    if [ "${base_array[$i]}" != "" ] && [ "${name_array[$i]}" == $1 ]; then
      pwrc_current_index=$i
      pwrc_status=$pwrc__success
      break;
    fi
    i=$i+1
  done
}

pwrc_prlist_get_base_index()
{
  let i=0

  pwrc_status=$pwrc__nobase

  while [ "${b_name_array[$i]}" != "" ]; do
    if [ "${b_root_array[$i]}" != "" ] && [ "${b_name_array[$i]}" == $1 ]; then
      pwrc_current_index=$i
      pwrc_status=$pwrc__success
      break;
    fi
    i=$i+1
  done
}

#
# Delete a project in the project list
#
# Arguments 1:  project name
# Returns pwrc_status : 0 Success
#                       1 Project not found
#
pwrc_prlist_delete()
{
  pwrc_prlist_get_index $1
  if [ $pwrc_status -ne $pwrc__success ]; then
    return
  fi
  unset base_array[$pwrc_current_index]
  pwrc_status=$pwrc__success
}

#
# Delete a base in the project list
#
# Arguments 1:  base name
# Returns pwrc_status : 0 Success
#                       1 Base not found
#
pwrc_prlist_base_delete()
{
  pwrc_prlist_get_base_index $1
  if [ $pwrc_status -ne $pwrc__success ]; then
    return
  fi
  unset b_root_array[$pwrc_current_index]
  pwrc_status=$pwrc__success
}

#
# Add a project to the project list
#
# Arguments 1:  dummy
#           2:  project name
#           3:  base name
#           4:  project root
#           5:  project hierarchy
#           6-: description
# Returns pwrc_status : Success
#                       projalrexist
#
pwrc_prlist_add()
{
  let i=0
  
  shift
  echo "Adding project $1"

  # Check if project exist
  pwrc_prlist_get_index $1
  if [ $pwrc_status -eq $pwrc__success ]; then
    pwrc_status=$pwrc__projalrexist
    return
  fi

  while [ "${name_array[$i]}" != "" ]; do
    i=$i+1
  done
  name_array[$i]=$1
  base_array[$i]=$2
  root_array[$i]=$3
  if [ -z $4 ]; then
    path_array[$i]="-"
  else
    path_array[$i]=$4
  fi
  shift 4
  desc_array[$i]=$@

  pwrc_status=$pwrc__success
}

# Arguments 1:  base name
#           2:  base root
# Returns pwrc_status : Success
#                       basealrexist Base already exist
#
pwrc_prlist_base_add()
{
  let i=0
  
  shift
  echo "Adding Base $1"

  # Check if base exist
  pwrc_prlist_get_base_index $1
  if [ $pwrc_status -eq $pwrc__success ]; then
    echo "Base already exist"
    pwrc_status=$pwrc__basealrexist
    return
  fi

  while [ "${b_name_array[$i]}" != "" ]; do
    i=$i+1
  done
  b_name_array[$i]=$1
  b_root_array[$i]=$2

  pwrc_status=$pwrc__success
}

#
# Write the project list
#
# Arguments No
# Returns pwrc_status : 0 Success
#                       1 No write access
#
pwrc_prlist_write()
{
  let i=0
 
  if [ -e $pwra_db/pwr_projectlist.dat ] && [ ! -w $pwra_db/pwr_projectlist.dat ]; then
    echo "Error: No write access to projectlist file"
    pwrc_status=$pwrc__projlistfileacc
    return
  fi
  
  {
    echo "! Name	base		root		path"

    # Write base array
    while [ "${b_name_array[$i]}" != "" ]; do
      if [ "${b_root_array[$i]}" != "" ]; then
        echo "%base "${b_name_array[$i]}"	"${b_root_array[$i]}
      fi
      i=$i+1
    done

    # Write project array
    i=0
    while [ "${name_array[$i]}" != "" ]; do
      if [ "${base_array[$i]}" != "" ]; then
        echo ${name_array[$i]}"	"${base_array[$i]}"	"${root_array[$i]}"	"${path_array[$i]}"	\""${desc_array[$i]}"\""
      fi
      i=$i+1
    done
  } > $pwra_db/pwr_projectlist.dat
  pwrc_status=$pwrc__success
}

pwrc_get_projectname()
{
  # If the project is configured, get name from config file,
  # else get it from env

  local pname
  let found=0

#  if [ -e $pwrp_db/pwrp_cnf_sysobject.dat ]; then
#    pname=`eval cat -n $pwrp_db/pwrp_cnf_sysobject.dat | grep "\b1\b" | awk '{print $2}'`
#    if [ ! -z $pname ]; then
#      found=1
#    fi
#  fi
  
  projectname=$pwrp_projectname
}

pwrc_create_func()
{
  local cmd
  local user_pwrp
  local proot
  local bname
  local pname
  let argc=$#

  cmd="project"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    #
    # Command is "create project"
    #
    # Arguments 2: project name
    #           3: base name
    #           4: project root
    #           5: project hierarchy
    #           6-: description
    pname=$2
    bname=$3
    proot=$4


    if [ $argc -lt 4 ]; then
      echo "Argument is missing"
      pwrc_status=$pwrc__syntax
      return
    fi

    pwrc_check_project_name $pname
    if [ $pwrc_status -ne $pwrc__success ]; then
      echo "Invalid project name '$pname'"
      return
    fi
        
    pwrc_prlist_read
    pwrc_prlist_get_base_index $bname
    if [ $pwrc_status -ne $pwrc__success ]; then
      echo "No such base '$bname'"
      return
    fi

    pwrc_prlist_get_index $pname
    if [ $pwrc_status -eq $pwrc__success ]; then
      echo "Project already exist '$pname'"
      pwrc_status=$pwrc__projalrexist
      return
    fi

    if [ -e "$proot" ]; then
      if [ ! -w "$proot" ]; then
        echo "No write access to project root"
	pwrc_status=$pwrc__rootacc
        return
      else
        if [ -e "$proot/common" ]; then
          echo "Error: project already exist"
          pwrc_status=$pwrc__rootexist
          return
        fi
      fi
    else
      if mkdir $proot
      then
        # Project root created
        echo "Project root created"
      else
        echo "Can't create project root"
	pwrc_status=$pwrc__rootcreate
	return
      fi
    fi
    mkdir $proot/login
    mkdir $proot/common
    mkdir $proot/common/src
    mkdir $proot/common/src/pop
    mkdir $proot/common/src/rtt
    mkdir $proot/common/src/rttbld
    mkdir $proot/common/src/tlog
    mkdir $proot/common/load
    mkdir $proot/common/db
    mkdir $proot/common/inc
    mkdir $proot/common/tmp
    mkdir $proot/common/web
    mkdir $proot/common/log
    mkdir $proot/$platform
    mkdir $proot/$platform/exe
    mkdir $proot/$platform/lib
    mkdir $proot/$platform/obj
    mkdir $proot/$platform/lis

    # Create project info file
    sysinfo="$proot/login/sysinfo.txt"
    echo "Revision history" >> $sysinfo
    echo "`date +%F`	$USER	Project created" >> $sysinfo

    # Create local setup script
    cat > $proot/login/login.sh << EOF
#! /bin/bash
#
#  Local setup 

# Printer command for plc documents
#export pwr_foe_gre_print="lpr -P lp1"

if [ -e "$pwrp_login/sysinfo.txt" ]; then
  echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_"
  echo "Welcome to"
  echo "\`$pwr_exe/pwrp_env.sh show project\`"
  echo ""
  cat \$pwrp_login/sysinfo.txt
  echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_"
fi
EOF

    chmod a+x $proot/login/login.sh

    # Set ownership to user and group pwrp
    user_pwrp=`eval cat /etc/passwd | grep "\bpwrp:"`
    #if [ ! -z "$user_pwrp" ]; then
    #  chown -R pwrp $proot/
    #fi

    user_pwrp=`eval cat /etc/group | grep "\bpwrp:"`
    if [ ! -z "$user_pwrp" ]; then
      chgrp -R pwrp $proot/
      chmod -R g+w $proot/
    fi

    # Insert in projectlist
    pwrc_prlist_read
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi
    pwrc_prlist_add $@
    pwrc_prlist_write
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi

    pwrc_set_func "project" $pname
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi

    # Create an empty directory database
    echo "Creating directory database"
    wb_cmd create volume/directory
    return
  fi
  
  echo "Unknown command"
}  

pwrc_add_func()
{
  local cmd
  local bname
  local broot
  let argc=$#


  cmd="base"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    #
    # Command is "add base"
    #
    # Arguments 2: base name
    #           3: base root
    bname=$2
    broot=$3

    if [ $argc -le 2 ]; then
      echo "Argument is missing"
      pwrc_status=$pwrc__syntax
      return
    fi
        
    pwrc_prlist_read
    if [ $pwrc_status -eq $pwrc__success ]; then
      pwrc_prlist_get_base_index $bname
      if [ $pwrc_status -eq $pwrc__success ]; then
        echo "Base already exist '$bname'"
        pwrc_status=$pwrc__basealrexist
        return
      fi
    fi

    # Insert in projectlist
    pwrc_prlist_base_add $@
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi
    pwrc_prlist_write
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi

    pwrc_status=$pwrc__success
    return
  fi
  
  echo "Unknown command"
}  

pwrc_delete_func()
{
  local cmd
  local proot

  cmd="project"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    local pname=$2
    local confirm=$3
    let i=0
  
    # Command is "delete project"
    if [ -z "$2" ]; then
      echo "Enter project name"
      pwrc_status=$pwrc__syntax
    fi
    
    pwrc_prlist_read
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi
    pwrc_prlist_get_index $pname
    if [ $pwrc_status -ne $pwrc__success ]; then
      echo "No such project '$pname'"
      return
    fi


    proot=${root_array[$pwrc_current_index]}
    pwrc_dblist_read $pname $proot
    
    if [ -z $confirm ] || [ $confirm != "noconfirm" ]; then
      echo "Delete project will"
      echo "	remove '$2' from the project list"
      echo "	delete all databases in project '$2'"
      echo "	delete all files in $proot"
      echo "Do you want do continue ? [y/n] "
      read REPLY
      if [ -z $REPLY ] || [ $REPLY != "y" ]; then
        return
      fi
    fi

    if [ ! -e "$proot" ]; then
      # Project root doesn't exist, remove from projectlist only
      echo "Warning: Project root not found"
    else
    
      if [ ! -w "$proot" ]; then
        echo "Error: no write access to project root"
        pwrc_status=$pwrc__rootacc
        return
      fi
    
      # Delete project directory tree
      if rm -R $proot
      then
        echo "Project root deleted"
      else
        if rmdir $proot
        then
          echo "Project root deleted"
        else
          echo "Unable to delete project root"
	  pwrc_status=$pwrc__rootdelete
        return
	fi
      fi

    fi

    pwrc_prlist_delete $2
    if [ $pwrc_status -eq $pwrc__success ]; then
      echo "Project $2 removed from projectlist"
      pwrc_prlist_write
    else
      echo "Something went wrong..."
    fi    
    return
  fi
  
  cmd="base"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    local bname=$2
    local confirm=$3
    let i=0
  
    # Command is "delete base"
    if [ -z "$2" ]; then
      echo "Enter base name"
      pwrc_status=$pwrc__syntax
    fi
    
    pwrc_prlist_read
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi
    pwrc_prlist_get_base_index $bname
    if [ $pwrc_status -ne $pwrc__success ]; then
      echo "No such base '$bname'"
      return
    fi

    pwrc_prlist_base_delete $2
    if [ $pwrc_status -eq $pwrc__success ]; then
      echo "Base $2 removed from projectlist"
      pwrc_prlist_write
    else
      echo "Something went wrong..."
    fi    
    return
  fi
  
  echo "Unknown command"
}  

pwrc_set_func()
{
  local cmd
  local pname
  local broot

  if [ -z $1 ]; then
    echo "Qualifier is missing"
    pwrc_status=$pwrc__syntax
    return
  fi

  cmd="project"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "set project"
    pname=$2


    # Load project list
    pwrc_prlist_read
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi    
    
    # Get project in project list
    pwrc_prlist_get_index $pname
    if [ $pwrc_status -ne $pwrc__success ]; then
      echo "No such project '$pname'"
      return
    fi

    proot=${root_array[$pwrc_current_index]}

    # Remove current pwrp_exe in PATH
    if [ ! -z $pwrp_exe ]; then
      exe_str=:$pwrp_exe
      export PATH=${PATH/$exe_str/}
    fi

    export pwrp_root=$proot
    export pwrp_login=$pwrp_root/login
    export pwrp_src=$pwrp_root/common/src
    export pwrp_pop=$pwrp_root/common/src/pop
    export pwrp_rtt=$pwrp_root/common/src/rtt
    export pwrp_rttbld=$pwrp_root/common/src/rttbld
    export pwrp_tlog=$pwrp_root/common/src/tlog
    export pwrp_load=$pwrp_root/common/load
    export pwrp_db=$pwrp_root/common/db
    export pwrp_inc=$pwrp_root/common/inc
    export pwrp_cmn=$pwrp_root
    export pwrp_tmp=$pwrp_root/common/tmp
    export pwrp_web=$pwrp_root/common/web
    export pwrp_log=$pwrp_root/common/log
    export pwrp_exe=$pwrp_root/$platform/exe
    export pwrp_lib=$pwrp_root/$platform/lib
    export pwrp_obj=$pwrp_root/$platform/obj
    export pwrp_lis=$pwrp_root/$platform/lis

    export PATH=$PATH:$pwrp_exe
    export CDPATH=$pwrp_root:$pwrp_root/common:$pwrp_root/x86_linux:$pwrp_src

    export pwrp_projectname=$pname

    # Set directorydb as default database
    # pwrc_setdb_func ""
      
    # Execute local setup script
    if [ -e "$pwrp_login/login.sh" ]; then
      source $pwrp_login/login.sh
    fi

    cd $pwrp_login

    pwrc_status=$pwrc__success
    return
  fi

  cmd="baseroot"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "set baseroot"
    baseroot=$2
    baseroot=${baseroot%/}
    if [ ! -e "$baseroot" ]; then
      echo "Base $baseroot doesn't exist"
      pwrc_status=$pwrc__baseexist
    else
      echo "Setting base $baseroot"

      # Remove current pwr_exe in PATH
      if [ ! -z $pwr_exe ]; then
        exe_str=:$pwr_exe
        export PATH=${PATH/$exe_str/}
      fi


      export pwrb_root=$baseroot
      export pwr_src=$baseroot/$os/$hw/exp/src
      export pwr_db=$baseroot/$os/$hw/exp/db
      export pwr_doc=$baseroot/$os/$hw/exp/doc
      export pwr_exe=$baseroot/$os/$hw/exp/exe
      export pwr_inc=$baseroot/$os/$hw/exp/inc
      export pwr_lib=$baseroot/$os/$hw/exp/lib
      export pwr_lis=$baseroot/$os/$hw/exp/lis
      export pwr_obj=$baseroot/$os/$hw/exp/obj
      export pwr_load=$baseroot/$os/$hw/exp/load
      
      export PATH=$PATH:$pwr_exe
    fi
    pwrc_status=$pwrc__success
    return
  fi

  if [ $1 = "db" ]; then
    pwrc_setdb_func $2
  else
    echo "Unknown command"
  fi
}  

pwrc_show_func()
{
  local cmd

  if [ -z $1 ]; then
    echo "Qualifier is missing"
    pwrc_status=$pwrc__syntax
    return
  fi
  
  cmd="project"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "show project"
    pwrc_get_projectname

    if [ -z "$pwrp_root" ]; then
      echo "No project is set"
    else
      pwrc_prlist_read
      pwrc_prlist_list_project $projectname
      if [ ! -e "$pwrp_root" ]; then
        echo "Project doesn't exist"
      fi
    fi
    return
  fi
  
  cmd="projects"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "show projects"

    pwrc_prlist_read
    pwrc_prlist_list $2
    return
  fi
  
  cmd="base"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "show base"
    baseroot=$pwrb_root
    
    if [ -z "$pwrb_root" ]; then
      echo "No base is set"
    else
      echo "Current base: $pwrb_root"

      if [ ! -e "$pwrb_root" ]; then
        echo "Base doesn't exist"
      fi
    fi
    return
  fi

  cmd="db"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then

    db=$2
    pwrc_get_projectname
    
    if [ -z "$pwrp_root" ]; then
      echo "No project is set"
      return
    fi
  
    if [ -z "$2" ]; then
      # Display current database
      current_db=${PWRP_DB#*:}
      current_db=${current_db%"_"$pwr_dbversion}

      if [ $current_db = $projectname ]; then
        echo "Current database is dbdirectory ($PWRP_DB)"
      else
        current_db=${PWRP_DB#:$projectname"_"}
        current_db=${current_db%"_"$pwr_dbversion}
        echo "Current database is '$current_db' ($PWRP_DB)"
      fi
    elif [ $2 == "-a" ] || [ $2 == "-l" ]; then
      # Display all databases
  
      # Check if database db exist, this can only be done on the sqlserver
      server=0
      if [ -z "$pwrp_mysql_server" ]; then
        server=1
      else        
        node="`eval uname -n`"
        if [ $node = $pwrp_mysql_server ]; then
          server=1
        fi
      fi
        
      if [ $server = 0 ]; then
        pwrc_status=$pwrc__notmysqlsrv
        return
      fi
      
      # Get location for mysql database    
      datadir=`eval mysqladmin $mysql_socket variables| grep datadir | awk '{ print $4 }'`
      datadir=${datadir%/}

      if [ -z $datadir ]; then
        echo "Error: Can't get database directory from mysql"
        pwrc_status=$pwrc__datadir
        return
      fi

      databases=`eval ls -d $datadir/$projectname"_*_"$pwr_dbversion`

      let first=1
      for dbname in $databases
      do
        if [ $first != 1 ]; then
          echo -n " "
        else
          first=0;
        fi
        if [ $2 == "-a" ]; then
          dbname=${dbname#$datadir/$projectname"_"}      
          echo -n "${dbname%"_"$pwr_dbversion}"
        else
          echo -n $dbname
        fi
      done
      echo ""
      return

    else
      # Check if database db exist, this can only be done on the sqlserver
      server=0
      if [ -z "$pwrp_mysql_server" ]; then
        server=1
      else        
        node="`eval uname -n`"
        if [ $node = $pwrp_mysql_server ]; then
          server=1
        fi
      fi
        
      if [ $server = 0 ]; then
        pwrc_status=$pwrc__notmysqlsrv
        return
      fi
      
      # Get location for mysql database    
      datadir=`eval mysqladmin $mysql_socket variables| grep datadir | awk '{ print $4 }'`
      datadir=${datadir%/}

      if [ -z $datadir ]; then
        echo "Error: Can't get database directory from mysql"
        pwrc_status=$pwrc__datadir
        return
      fi

      if [ "$db" = "dbdirectory" ]; then
        dbname=$projectname"_"$pwr_dbversion
      else
        dbname=$projectname"_"$db"_"$pwr_dbversion
      fi

      if [ -e "$datadir/$dbname" ]; then
        echo "Database '"${dbname%"_"$pwr_dbversion}"'"
      else
        echo "Database doesn't exist"
        pwrc_status=$pwrc__dbnotfound
        return
      fi

    fi
    return
  fi

  
  echo "Unknown command"
}

pwrc_modify_func()
{
  local cmd

  if [ -z $1 ]; then
    echo "Qualifier is missing"
    pwrc_status=$pwrc__syntax
    return
  fi
  cmd="project"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "modify project"
    # 
    # Argments $2 project name
    #          -n 'name' modify project name
    #          -r 'root' modify project root
    #          -b 'base' modify base
    #	       -d 'descr' modify description
    local dbname
    local pname
    local new_pname
    local path
    local root
    local base
    local desc
    let argc=$#
    let modify_name=0
    let modify_path=0
    let modify_root=0
    let modify_base=0
    let modify_desc=0
    let something_modified=0
    
    echo "Modify project: $@"
        
    pname=$2
    shift 2
    while [ -n "$1" ]; do
      if [ "$1" == "-n" ]; then
        shift
        new_pname=$1
        modify_name=1
        shift
      elif [ "$1" == "-r" ]; then
        shift
        root=$1
        modify_root=1
        shift
      elif [ "$1" == "-h" ]; then
        shift
        path=$1
        modify_path=1
        shift
      elif [ "$1" == "-b" ]; then
        shift
        base=$1
        modify_base=1
        shift
      elif [ "$1" == "-d" ]; then
        shift
        desc=$@
        modify_desc=1
        shift
        break;
      else
        echo "Syntax error"
	return
      fi
    done
    
    pwrc_prlist_read
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi

    pwrc_prlist_get_index $pname
    if [ $pwrc_status -ne $pwrc__success ]; then
      echo "No such project '$pname'"
      pwrc_status=$pwrc__noproj
      return
    fi

    if [ $modify_root -eq 1 ]; then    
      # Check that new root doesn't exist
      if [ -e "$root" ]; then
        echo "New root already exist"
        pwrc_status=$pwrc__rootexist
	return
      fi
      if mv ${root_array[$pwrc_current_index]} $root
      then
        root_array[$pwrc_current_index]=$root
        something_modified=1
      else
        echo "Error from mv"
        pwrc_status=$pwrc__move
        return
      fi
    fi
    if [ $modify_path -eq 1 ]; then    
      path_array[$pwrc_current_index]=$path
      something_modified=1
    fi
    if [ $modify_base -eq 1 ];then    
      pwrc_prlist_get_base_index $base
      if [ $pwrc_status -ne $pwrc__success ]; then
        echo "No such base $base"
        pwrc_status=$pwrc__nobase
	return
      fi

      pwrc_prlist_get_index $pname
      base_array[$pwrc_current_index]=$base
      something_modified=1
    fi
    if [ $modify_desc -eq 1 ];then    
      desc_array[$pwrc_current_index]=$desc
      something_modified=1
    fi

    if [ $something_modified -eq 1 ]; then
      pwrc_prlist_write      
      if [ $pwrc_status -ne $pwrc__success ]; then
        return
      fi
    fi

    if [ $modify_name -eq 1 ]; then
      # This can only be done on mysql server
      if [ ! -z "$pwrp_mysql_server" ]; then
        node="`eval uname -n`"

        if [ $node != $pwrp_mysql_server ]; then
          echo "Error: current host is not mysql server ($pwrp_mysql_server)"
          pwrc_status=$pwrc__notmysqlsrv
          return
        fi
      fi

      # Get location for mysql database    
      datadir=`eval mysqladmin $mysql_socket variables| grep datadir | awk '{ print $4 }'`
      datadir=${datadir%/}

      if [ -z $datadir ]; then
        echo "Error: Can't get database directory from mysql"
        pwrc_status=$pwrc__datadir
        return
      fi

      # Get databases in from-project
      pwrc_dblist_read $pname ${root_array[$pwrc_current_index]}
      pwrc_status=$pwrc__success
    
      # Check that the databases can be found and that the new doesn't exist
      i=0
      while [ "${db_array[$i]}" != "" ]; do      
        if [ ! -e $datadir/${db_array[$i]} ]; then
          echo "Error: Can't find database ${db_array[$i]}"
          pwrc_status=$pwrc__nodb
          return
        fi

        i=$i+1
      done
    
      # Rename databases
      i=0
      while [ "${db_array[$i]}" != "" ]; do
        dbname=$new_pname${db_array[$i]#$pname}
        if wb_cp -rename $datadir/${db_array[$i]} $datadir/$dbname
        then
	  echo "Database $datadir/${db_array[$i]} -> $datadir/$dbname"
        else
	  echo "Error copying mysql database '$datadir/${db_array[$i]}'"
          pwrc_status=$pwrc__mysqldbrena
        fi
        i=$i+1
      done
      name_array[$pwrc_current_index]=$new_pname
      pwrc_prlist_write
      if [ $pwrc_status -ne $pwrc__success ]; then
        return
      fi
    fi
    pwrc_status=$pwrc__success
  else
    echo "Unknown command"
  fi

}

pwrc_copy_func()
{
  local cmd

  if [ -z $1 ]; then
    echo "Qualifier is missing"
    pwrc_status=$pwrc__syntax
    return
  fi
    
  cmd="project"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    # Command is "copy project"
    # 
    # Argments $2: from-project name
    #          $3: to-project name
    #          $4: to-project root
    #	       $5: to-project hierarchy
    #          $6: confirm ("noconfirm" if no confirmation)
    local from_pname=$2
    local to_pname=$3
    local to_proot=$4
    local to_phier=$5
    local confirm=$6
    local dbname
    local ver
    let argc=$#

    if [ $argc -lt 4 ]; then
      echo "Argument is missing"
      pwrc_status=$pwrc__syntax
      return
    fi

    if [ -z "$to_phier" ]; then
      to_phier="-"
    fi

    # Load project list
    pwrc_prlist_read
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi    

    # Check that to-project doesn't exist in project list
    pwrc_prlist_get_index $to_pname
    if [ $pwrc_status -eq $pwrc__success ]; then
      echo "Project already exist '$to_pname'"
      pwrc_status=$pwrc__projalrexist
      return
    fi

    # Get from-project in project list
    pwrc_prlist_get_index $from_pname
    if [ $pwrc_status -ne $pwrc__success ]; then
      echo "No such project '$from_pname'"
      return
    fi
    
    # Check to-project root
    if [ -e "$to_proot" ]; then
      if [ ! -w "$to_proot" ]; then
        echo "No write access to project root"
        pwrc_status=$pwrc__rootacc
        return
      else
        if [ -e "$to_proot/common" ]; then
          echo "Error: project root '$to_proot' already exist"
          pwrc_status=$pwrc__rootexist
          return
        fi
      fi
    else
      # Check permission to create directory tree
      if mkdir $to_proot
      then
        # Project root created
        rmdir $to_proot
      else
        echo "Can't create project root '$to_proot'"
        pwrc_status=$pwrc__rootcreate
	return
      fi
    fi
    
    from_proot=${root_array[$pwrc_current_index]}


    # Confirmation
    if [ -z $confirm ] || [ $confirm != "noconfirm" ]; then
      echo "Copy project will"
      echo "	add '$to_pname' to the project list"
      echo "	copy directory tree '$from_proot' to '$to_proot'"

      echo "Do you want do continue ? [y/n] "
      read REPLY
      if [ -z $REPLY ] || [ $REPLY != "y" ]; then
        return
      fi
    fi

    # Copy directory tree
    if cp -R $from_proot $to_proot
    then
      echo "Project directory tree copied"
    else
      echo "Unable to copy directory tree"
      pwrc_status=$pwrc__copy
      return
    fi
    
    # Add new project into project list
    pwrc_prlist_add dummy $to_pname ${base_array[pwrc_current_index]} $to_proot $to_phier ${desc_array[pwrc_current_index]} "(Copy of $from_pname)"
    pwrc_prlist_write
    if [ $pwrc_status -ne $pwrc__success ]; then
      return
    fi

    pwrc_status=$pwrc__success    
  else
    echo "Unknown command"
  fi
}

pwrc_setdb_func()
{
  echo "setdb is obsolete"
  pwrc_status=$pwrc__success
}

pwrc_save_func()
{
  local cmd
  local pname
  local broot

  if [ -z $1 ]; then
    echo "Qualifier is missing"
    pwrc_status=$pwrc__syntax
    return
  fi

  if [ $1 = "file" ]; then
    pwrc_save_file_func $2
  else
    echo "Unknown command"
  fi
}  

pwrc_save_file_func()
{
  new_file=$1
  
  if [ -e $new_file ]; then
    let version=9

    while [ $version -ge 1 ]
    do
      old_file=$new_file.$version
      old_file_ren=$new_file.$((version+1))
      if [ -e $old_file ]; then
        mv $old_file $old_file_ren
      fi
      let version=$version-1
    done

    old_file=$new_file.1
    echo "-- Saving file $new_file -> $old_file"
    mv $new_file $old_file
  fi
}

pwrc_help_func()
{
  cat << EOF

pwrp_env.sh  		- Utilities for pwr project environment

help			- Display help
create project 'name'	- Create at new project
set project 'name'	- Setup environment to a project
set baseroot 'root'	- Setup environment to proview base distribution
set db ['db']		- Set database. If db ommitted, directory db is set.
show project		- Show current project
show projects		- Show all projects
show base		- Show current base root

EOF
}

pwrp_env ()
{
  source pwrp_env.sh $1 $2 $3 $4
}

pwrc_parse ()
{
  platform="x86_linux"
  os="os_linux"
  hw="hw_x86"

  local cmd

  pwrc_get_variables

  cmd="help"
  if [ -z $1 ] || [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_help_func $@
    return
  fi
  cmd="show"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_show_func $@
    return $pwrc_status
  fi
  cmd="delete"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_delete_func $@
    return $pwrc_status
  fi
  cmd="create"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_create_func $@
    return $pwrc_status
  fi
  cmd="add"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_add_func $@
    return $pwrc_status
  fi
  cmd="copy"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_copy_func $@
    return $pwrc_status
  fi
  cmd="modify"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_modify_func $@
    return $pwrc_status
  fi
  cmd="set"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_set_func $@
    return $pwrc_status
  fi
  cmd="save"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_save_func $@
    return $pwrc_status
  fi
  cmd="setdb"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    pwrc_setdb_func $@
    return $pwrc_status
  else
    echo "Unknown command"
  fi
}


#set -o xtrace
pwrc_parse $@
#set +o xtrace
