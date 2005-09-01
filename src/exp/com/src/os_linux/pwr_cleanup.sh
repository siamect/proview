#
# Proview   $Id: pwr_cleanup.sh,v 1.2 2005-09-01 14:57:49 claes Exp $
# Copyright (C) 2005 SSAB Oxelösund AB.
#
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License as 
# published by the Free Software Foundation, either version 2 of 
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with the program, if not, write to the Free Software 
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
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
