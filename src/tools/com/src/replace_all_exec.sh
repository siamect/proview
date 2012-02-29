#!/bin/bash
#set -o xtrace
if [ "$2" == "c" ]; then
  echo "executing $1"
  awk -f license.awk $1 > t.tmp
  mv t.tmp $1
elif [ "$2" == "sh" ]; then
  echo "executing $1"
  awk -f license_sh.awk $1 > t.tmp
  mv t.tmp $1
elif [ "$2" == "wbl" ]; then
  echo "executing $1"
  awk -f license_wbl.awk $1 > t.tmp
  mv t.tmp $1
else
  echo "Unknown filtype"
fi 
