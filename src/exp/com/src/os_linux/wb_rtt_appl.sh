#!/bin/bash
#
# Link rtt application
#
#
name=$1
action=$2
is_rttsys=$3
opsys=$4
menuname=dtt_appl_${name}_m
applname=ra_rtt_${name}
exename=ra_rtt_${name} 
arname=ra_rtt_${name}.a
arname_pict=ra_rtt_${name}_pict.a


if [ $is_rttsys = "0" ]
then
# echo "Not rttsys"

  if [ $action = "1" ]
  then
#   echo "Link"
    if [ ! -e $pwrp_lib/${arname} ]
    then
#     echo "Creating archive $arname"
      ar rc $pwrp_lib/${arname}
    fi

    if [ ! -e $pwrp_lib/${arname_pict} ]
    then
#     echo "Creating archive $arname_pict"
      ar rc $pwrp_lib/${arname_pict}
    fi

    cc=gcc
    cinc="-I$pwr_inc -I$pwrp_rttbld -I-"
    cflags="-DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86 -O3 -DGNU_SOURCE -DPWR_NDEBUG -D_REENTRANT"

    ${cc} -c -o $pwrp_obj/${menuname}.o \
        $pwrp_rttbld/${menuname}.c \
        ${cinc} ${cflags}

    ld=g++
    linkflags="-g -L/lib/thread -L$pwrp_lib -L$pwr_lib"

    ${ld} ${linkflags} -o $pwrp_exe/${exename} $pwrp_obj/${menuname}.o \
     $pwr_obj/dtt_rttsys.o $pwr_obj/rt_io_user.o \
     $pwrp_lib/${arname_pict} $pwrp_lib/${arname} \
     -lpwr_dtt\
     -lpwr_rt -lpwr_co -lpwr_msg_dummy -lrpcsvc -lpthread -lm -lposix1b -lrt 
  fi

  if [ $action = "2" ]
  then
#   echo "Compile"
    cc=gcc
    cinc="-I$pwr_inc -I$pwrp_rttbld -I$pwrp_inc -I$pwrp_cmn/common/inc -I-"
    cflags="-DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86 -O3 -DGNU_SOURCE -DPWR_NDEBUG -D_REENTRANT"

    ${cc} -c -o $pwrp_obj/${applname}.o \
        $pwrp_rtt/${applname}.c \
        ${cinc} ${cflags}
    ar rc $pwrp_lib/${arname} $pwrp_obj/${applname}.o 

  fi
fi








