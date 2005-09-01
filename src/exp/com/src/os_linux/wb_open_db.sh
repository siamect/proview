#
# Proview   $Id: wb_open_db.sh,v 1.3 2005-09-01 14:57:49 claes Exp $
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
# Open a new terminal and start the navigator
# This procedure is started from the navigator 'Open db' method for
# DbConfig and DbTrace objects.
#
username=$1
password=$2
database=$3
title=$4

set -o xtrace
if [ -z $database ]; then
  echo "Database is missing"
  return
fi

echo "-- Opening volume '$database'"
wb $username $password $database
