#! /bin/bash
#

FileName="$1"
Color="$2"
Printer="$pwr_rt_print"
ColorPrinter="$pwr_rt_colorprint"

if [ "$Color" == "1" ]; then
  if [ -z "$pwr_rt_colorprint" ]; then
    Printer=$pwr_rt_print
  else
    Printer=$pwr_rt_colorprint
  fi
else  
  if [ -z "$pwr_rt_print" ]; then
    Printer=$pwr_rt_colorprint
  else
    Printer=$pwr_rt_print
  fi
fi

if [ -z "$Printer" ]; then
  echo "Printer is not defined in env 'pwr_rt_print'"
else
  echo "Printing $1"
  $Printer $1
fi
