#!/bin/bash
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

