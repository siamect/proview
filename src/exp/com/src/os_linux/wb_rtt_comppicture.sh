#! /bin/bash
#
# Proview   $Id: wb_rtt_comppicture.sh,v 1.2 2005-09-01 14:57:49 claes Exp $
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
# Compile a rtt picture
#
# set -o xtrace

name=$1
pgmname=$2
is_rttsys=$3
opsys=$4

# echo $name $pgmname $is_rttsys $opsys

if [ $opsys = "64" ]
then
# echo "Opsys is linux"

  if [ $is_rttsys = "0" ]
  then
    ar_name_pict=${pwrp_lib}/ra_rtt_${pgmname}_pict.a

    cc=gcc
    cinc="-I$pwr_inc -I$pwrp_rttbld -I-"
    cflags="-DOS_LINUX -DOS=linux -DHW_X86=1 -DPOSIX_SOURCE -DWall" 
    ${cc} -c -o $pwrp_obj/${name}.o $pwrp_rttbld/${name}.c ${cinc} ${cflags}
    ar rc ${ar_name_pict} $pwrp_obj/${name}.o
  else
#   echo "Is rttsys"
    ar_name_pict=${pwr_lib}/libpwr_dtt.a
    bld_dir=$pwre_broot/os_linux/hw_x86/bld/lib/dtt
#   echo $ar_name $ar_name_pict

    cc=gcc
    cinc="-I$pwr_inc -I${bld_dir} -I-"
    cflags="-DOS_LINUX -DOS=linux -DHW_X86=1 -DPOSIX_SOURCE -DWall" 
    ${cc} -c -o ${bld_dir}/${name}.o \
       ${bld_dir}/${name}.c ${cinc} ${cflags}
    ar rc ${ar_name_pict} ${bld_dir}/${name}.o
  fi
else
  echo "Opsys is not linux, not yet supported"

#  if [ $is_rttsys = "0" ]
#  then
#   echo "Not rttsys"
#  else
#   echo "Is rttsys"
#  fi

fi

