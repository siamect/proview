#!/bin/bash
#
# Proview   $Id: wb_rtt_appl_remote.sh,v 1.2 2005-09-01 14:57:49 claes Exp $
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

function from_vms
{
  local dir=$1

  if [ -n "$(echo ${dir} | grep :)" ]; then
#   Convert name from VMS to UNIX
    dir=/${dir//$/}
    dir=${dir//.]/}
    dir=${dir//.>/}
    dir=${dir//]/}
    dir=${dir//>/}
    dir=${dir//./\/}
    dir=${dir//:</\/}
    dir=${dir//>/}
    dir=${dir//:[/\/}
    dir=${dir//]/}
  fi

  echo $dir
}


pgmname=$1
action=$2
opsys=$3
debug=$4
p_rttbld=$5
p_root=$6

p_rttbld=$(from_vms ${p_rttbld})
p_root=$(from_vms ${p_root})

os=lynx
if [ -z $(uname -m | grep 86) ]; then
  hw=ppc
else
  hw=x86
fi

# echo p_root: ${p_root}
# echo p_rttbld: ${p_rttbld}

export pwrp_rttbld=${p_rttbld}
export pwrp_exe=${p_root}/${hw}_${os}/exe
export pwrp_lib=${p_root}/${hw}_${os}/lib
export pwrp_obj=${p_root}/${hw}_${os}/obj


# echo exe: $pwrp_exe lib: $pwrp_lib obj: $pwrp_obj rttbld: $pwrp_rttbld

source $pwr_exe/wb_rtt_appl.sh $1 $2 $3 $4

