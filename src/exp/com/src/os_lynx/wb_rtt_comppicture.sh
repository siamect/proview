#! /bin/bash
#
# Compile a rtt picture
#
# set -o xtrace

name=$1
pgmname=$2
is_rttsys=$3
opsys=$4

# echo $name $pgmname $is_rttsys $opsys

if [ $opsys = "16" ]
then
# echo "Opsys is lynx"

  if [ $is_rttsys = "0" ]
  then
    ar_name_pict=${pwrp_lib}/ra_rtt_${pgmname}_pict.a

    cc=gcc
    cinc="-I$pwr_inc -I$pwrp_rttbld -I-"
    cflags="-DOS_LYNX -DOS=lynx -DHW_X86=1 -DPOSIX_SOURCE -DWall" 
    ${cc} -c -o $pwrp_obj/${name}.o $pwrp_rttbld/${name}.c ${cinc} ${cflags}
    ar rc ${ar_name_pict} $pwrp_obj/${name}.o
  else
#   echo "Is rttsys"
    ar_name_pict=${pwr_lib}/libpwr_dtt.a
    bld_dir=$pwre_broot/os_lynx/hw_x86/bld/lib/dtt
#   echo $ar_name $ar_name_pict

    cc=gcc
    cinc="-I$pwr_inc -I${bld_dir} -I-"
    cflags="-DOS_LYNX -DOS=lynx -DHW_X86=1 -DPOSIX_SOURCE -DWall" 
    ${cc} -c -o ${bld_dir}/${name}.o \
       ${bld_dir}/${name}.c ${cinc} ${cflags}
    ar rc ${ar_name_pict} ${bld_dir}/${name}.o
  fi
else
  echo "Opsys is not lynx, not yet supported"

#  if [ $is_rttsys = "0" ]
#  then
#   echo "Not rttsys"
#  else
#   echo "Is rttsys"
#  fi

fi

