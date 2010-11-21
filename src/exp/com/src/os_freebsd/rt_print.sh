#!/bin/bash
#
# Proview   $Id: rt_print.sh,v 1.2 2005-09-01 14:57:49 claes Exp $
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
