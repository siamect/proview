#
# pwr_cleanup
#
# Removes PROVIEW IPC objects for the defined bus id (PWR_BUS_ID)
#
lipcrm `lipcs -m |awk '{print $1}'|grep "pwr.*$PWR_BUS_ID"`
lipcrm `lipcs -s |awk '{print $1}'|grep "pwr.*_$PWR_BUS_ID"`
lipcrm `lipcs -q |awk '{print $1}'|grep "pwr.*_$PWR_BUS_ID"`
