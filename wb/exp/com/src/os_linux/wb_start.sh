#!/bin/bash
#
# Proview   $Id: wb_start.sh,v 1.2 2007-05-28 14:50:48 claes Exp $
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
# Start the navigator
#

username=$1
password=$2
volume=$3

export LC_TIME="en_US"
export LC_NUMERIC="POSIX"

wb $username $password $volume $4 $5 $6 $7





