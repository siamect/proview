#
# pwr_cleanup.sh
#
# NOTE! This file removes all IPC objects for the user.
# 	If you have other program which creates IPC objects,
#	then do not use this script  
#
# remove semaphores
eval `ipcs -s|grep ^0x|grep "[ \t]$USER[ \t]"|awk '{printf "ipcrm sem %s;", $2}'`

# remove message queues
eval `ipcs -q|grep ^0x|grep "[ \t]$USER[ \t]"|awk '{printf "ipcrm msg %s;", $2}'`

# remove shared memory
eval `ipcs -m|grep ^0x|grep "[ \t]$USER[ \t]"|awk '{printf "ipcrm shm %s;", $2}'`

rm /tmp/pwr*$PWR_BUS_ID
