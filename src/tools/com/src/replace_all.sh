#!/bin/bash
#set -o xtrace

if [ "$1" == "cpp" ]; then
  file="*.cpp"
  ftype="c"
elif [ "$1" == "c" ]; then
  file="*.c"
  ftype="c"
elif [ "$1" == "h" ]; then
  file="*.h"
  ftype="c"
elif [ "$1" == "hpp" ]; then
  file="*.hpp"
  ftype="c"
elif [ "$1" == "java" ]; then
  file="*.java"
  ftype="c"
elif [ "$1" == "sh" ]; then
  file="*.sh"
  ftype="sh"
elif [ "$1" == "pwr_com" ]; then
  file="*.pwr_com"
  ftype="sh"
elif [ "$1" == "wb_load" ]; then
  file="*.wb_load"
  ftype="wbl"
elif [ "$1" == "msg" ]; then
  file="*.msg"
  ftype="wbl"
elif [ "$1" == "uil" ]; then
  file="*.uil"
  ftype="wbl"
elif [ "$1" == "xxx" ]; then
  file="*.xxx"
  ftype="c"
else
  echo ""
  echo "Replace a string in all files of a certain type"
  echo "Note! Edit the awk-files first."
  echo ""
  echo "Usage:    replace_all.sh 'filetype'"
  echo ""
  echo "filetype  c, cpp, h, hpp, java, sh, pwr_com, wb_load, msg, uil"
  echo "          or xxx (for test)"
  echo ""
  exit
fi

find $pwre_croot -name "$file" -exec ./replace_all_exec.sh {} $ftype \;
