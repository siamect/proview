#!/bin/bash
#
# Proview   $Id: wb_distr_keepboot.sh,v 1.3 2006-01-11 14:18:38 claes Exp $
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
# Keep the old versions of bootfiles
#

new_file=$1
  
if [ -e $new_file ]; then
  let version=30

  while [ $version -ge 1 ]
  do
    old_file=$new_file.$version
    old_file_ren=$new_file.$((version+1))
    if [ -e $old_file ]; then
      mv $old_file $old_file_ren
    fi
    let version=$version-1
  done

  old_file=$new_file.1
  echo "-- Saving file $new_file -> $old_file"
  mv $new_file $old_file
fi
