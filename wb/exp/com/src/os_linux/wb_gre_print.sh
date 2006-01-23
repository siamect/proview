#! /bin/bash
#
# Proview   $Id: wb_gre_print.sh,v 1.1 2006-01-23 08:44:30 claes Exp $
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

if [ -z "$pwr_foe_gre_print" ]; then
  echo "Printer is not defined in env 'pwr_foe_gre_print'"
fi

echo "Printing $1"
$pwr_foe_gre_print $1

