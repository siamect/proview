#!/bin/bash
#
# Proview   $Id: pwr_stop.sh,v 1.3 2008-11-24 15:22:42 claes Exp $
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

  kill -9 `ps ax | grep "rt_neth_acp" | awk '{ print $1}'`
  kill -9 `ps ax | grep "rt_neth" | awk '{ print $1}'`
  kill -9 `ps ax | grep "rt_qmon" | awk '{ print $1}'`
  kill -9 `ps ax | grep "rt_emon" | awk '{ print $1}'`
  kill -9 `ps ax | grep "rt_tmon" | awk '{ print $1}'`
  kill -9 `ps ax | grep "rt_\|/pwr/exe/rs" | awk '{ print $1}'`
  kill -9 `ps ax | grep "\[rt_" | awk '{ print $1}'`
  kill -9 `ps ax | grep "plc_" | awk '{ print $1}'`
  kill -9 `ps ax | grep "jpwr.rt" | awk '{ print $1}'`
  kill -9 `ps ax | grep "rs_nmps" | awk '{ print $1}'`
  kill -9 `ps ax | grep "rs_remote" | awk '{ print $1}'`
  kill -9 `ps ax | grep "sev_server" | awk '{ print $1}'`
  kill -9 `ps ax | grep "opc_server" | awk '{ print $1}'`
  if [ -u $pwr_exe/rt_ini ]; then
    user=`ls -al $pwr_exe/rt_ini | awk '{ print $3}'`
  else
    user=$USER
  fi

  echo "User $user"

  eval `ipcs -s|grep ^0x|grep "[ \t]$user[ \t]"|awk '{printf "ipcrm sem %s;", $2
}'`

# remove message queues
  eval `ipcs -q|grep ^0x|grep "[ \t]$user[ \t]"|awk '{printf "ipcrm msg %s;", $2
}'`

# remove shared memory
  eval `ipcs -m|grep ^0x|grep "[ \t]$user[ \t]"|awk '{printf "ipcrm shm %s;", $2
}'`

  if [ -e $pwrp_exe/pwrp_stop.sh ]; then
    source $pwrp_exe/pwrp_stop.sh
  fi

  rm /tmp/pwr*$PWR_BUS_ID
  
  #rm -f /tmp/pwr*
  #id=`ipcs -s | grep $user | awk '{print $2}'`
  #id1=`echo $id | awk '{print $1}'`
  #id2=`echo $id | awk '{print $2}'`
  #id3=`echo $id | awk '{print $3}'`
  #ipcrm sem $id1
  #ipcrm sem $id2
  #ipcrm sem $id3
  #id=`ipcs -q | grep $user | awk '{print $2}'`
  #id1=`echo $id | awk '{print $1}'`
  #id2=`echo $id | awk '{print $2}'`
  #id3=`echo $id | awk '{print $3}'`
  #ipcrm msg $id1
  #ipcrm msg $id2
  #ipcrm msg $id3
