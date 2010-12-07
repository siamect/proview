#! /bin/bash

initst=`eval pidof rt_ini`
if [ -z $initst ]; then
  source /etc/pwrp_profile
  export pwra_db=/usr/pwrp/adm/db
  source $pwra_db/pwr_setup.sh
  source $pwra_db/pwra_env.sh set project pwrdemo48
  export PWR_BUS_ID=999
  source pwr_stop.sh
  rt_ini &
  sleep 10
  initst=`eval pidof rt_ini`
  if [ -z $initst ]; then
    zenity --error --text "Unable to start the Demo Project Runtime Enviroment"
    source pwr_stop.sh
    exit
  fi
fi
firefox http://ubuntu/index.html
source pwr_stop.sh
