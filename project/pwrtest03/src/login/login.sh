#! /bin/bash
#
#  Description
#
#  Project login script.
#  This script is executed when the project is attached, and contains
#  settings for the development environment.
#
#
#  Revision history
#  2021-01-31	claes	File created
#

#  Local setup 

# Printer command for plc documents
#export pwr_foe_gre_print="lpr -P lp1"

# Specification of external directories with includefiles, included in the plc code
# by ra_plc_user.h. The syntax is a compiler option string
#export PWR_EXT_INC="-I/usr/local/include"

if [ -e "$pwrp_doc/sysinfo.txt" ]; then
  echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_"
  echo "Welcome to"
  echo "`$pwr_exe/pwrp_env.sh show project`"
  echo ""
  cat $pwrp_doc/sysinfo.txt
  echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_"
fi
