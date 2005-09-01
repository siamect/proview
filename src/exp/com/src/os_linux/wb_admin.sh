#! /bin/bash
#
# Proview   $Id: wb_admin.sh,v 1.4 2005-09-01 14:57:49 claes Exp $
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

wb_adm_opendb_func ()
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
  
  wb "$username" "$password" $volume &
}

wb_adm_create_func ()
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



wb_adm_parse ()
{

  local cmd

  cmd="opendb"
  if [ -z $1 ] || [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    wb_adm_opendb_func $1 $2 $3 $4 $5
    return
  fi
  cmd="create"
  if [ -z $1 ] || [ $1 = $cmd ] || [ ${cmd#$1} != $cmd ]; then
    shift
    wb_adm_create_func $@
    return
  fi
  echo "Unknown command"
}

#set -o xtrace
wb_adm_parse $@
#set +o xtrace
