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
