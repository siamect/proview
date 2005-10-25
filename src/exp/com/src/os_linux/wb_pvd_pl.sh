#!/bin/bash
#
# Proview   $Id: wb_pvd_pl.sh,v 1.1 2005-10-25 12:05:37 claes Exp $
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
set -o xtrace

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

wb_pl_opendb_func ()
{
    #
    # Command is "opendb"
    #
    # Arguments 1: project
    #           2: username
    #           3: password
    #           4: database
    #           5: volume
  local project=$1
  local username=$2
  local password=$3
  local database=$4
  local volume=$5

echo "args: $1 $2 $3 $4 $5"
  
  source pwrp_env.sh set project $project
  
  if [ "$database" != "dbdirectory" ]; then
    source pwrp_env.sh setdb $database
  fi
  
  wb -q "$username" "$password" $volume &
}

wb_pl_check_func ()
{
  cmd="move"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    #
    # Command is "check move"
    #
    # Arguments 2: source root
    #           3: destination root
    local srcroot=$2
    local destroot=$3
    local destdir=${destroot%/*}

    if [ ! -e "$srcroot" ]; then
      echo "Source root not found"
      pwrc_status=$pwrc_noroot
    fi

    if [ -e "$destroot" ]; then
      if [ ! -w "$destroot" ]; then
        echo "No write access to project root"
	pwrc_status=$pwrc__rootacc
        return
      else
        if [ -e "$destroot/common" ]; then
          echo "Error: destination root already exist"
          pwrc_status=$pwrc__rootexist
          return
        fi
      fi
    else
      if [ ! -e "$destdir" ]; then
        echo "Can't create destination root"
        pwrc_status=$pwrc__rootcreate
        return
      else
        if [ ! -w "$destdir" ]; then
          echo "No write access to project root"
	  pwrc_status=$pwrc__rootacc
          return
        fi
      fi
    fi
    pwrc_status=$pwrc__success
    return
  fi

  cmd="create"
  if [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    #
    # Command is "check move"
    #
    # Arguments 2: destination root
    local destroot=$2
    local destdir=${destroot%/*}

    if [ -e "$destroot" ]; then
      if [ ! -w "$destroot" ]; then
        echo "No write access to project root"
	pwrc_status=$pwrc__rootacc
        return
      else
        if [ -e "$destroot/common" ]; then
          echo "Error: destination root already exist"
          pwrc_status=$pwrc__rootexist
          return
        fi
      fi
    else
      if [ ! -e "$destdir" ]; then
        echo "Can't create destination root"
        pwrc_status=$pwrc__rootcreate
        return
      else
        if [ ! -w "$destdir" ]; then
          echo "No write access to project root"
	  pwrc_status=$pwrc__rootacc
          return
        fi
      fi
    fi
    pwrc_status=$pwrc__success
    return
  fi
}

wb_pl_create_func ()
{
  local cmd
  
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

    source pwrp_env.sh create $@
    if [ $pwrc_status -ne 0 ]; then
      return $pwrc_status
    fi

    #source pwrp_env.sh set project $pname
    #if [ $pwrc_status -ne 0 ]; then
    #  return $pwrc_status
    #fi

    #wb_cmd create volume/name=\"directory\"/ident=254.254.254.253/class=\$DirectoryVolume
  fi
}



wb_pl_parse ()
{

  local cmd

  cmd="opendb"
  if [ -z $1 ] || [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    wb_pl_opendb_func $1 $2 $3 $4 $5
    return
  fi
  cmd="create"
  if [ -z $1 ] || [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    wb_pl_create_func $@
    return
  fi
  cmd="check"
  if [ -z $1 ] || [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    wb_pl_check_func $@
    return $pwrc_status
  fi
  echo "Unknown command"
}

#set -o xtrace
wb_pl_parse $@
#set +o xtrace
