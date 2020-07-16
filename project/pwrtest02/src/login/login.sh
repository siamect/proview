#! /bin/bash
#
#  Local setup 

# Printer command for plc documents
#export pwr_foe_gre_print="lpr -P lp1"

export PWR_BUS_ID=999

if [ -e "$pwrp_root/src/login/sysinfo.txt" ]; then
  echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_"
  echo "Welcome to"
  echo "`$pwr_exe/pwrp_env.sh show project`"
  echo ""
  cat $pwrp_login/sysinfo.txt
  echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_"
fi
