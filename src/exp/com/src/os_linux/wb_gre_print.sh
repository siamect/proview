#! /bin/bash
#

if [ -z "$pwr_foe_gre_print" ]; then
  echo "Printer is not defined in env 'pwr_foe_gre_print'"
fi

echo "Printing $1"
$pwr_foe_gre_print $1

