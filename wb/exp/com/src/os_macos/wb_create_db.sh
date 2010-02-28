#!/bin/bash
#
# Proview   $Id: wb_create_db.sh,v 1.1 2006-01-23 08:44:30 claes Exp $
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
#      Create the database directory and copy en empty database to the
#      directory.
#

db=$1

source pwrp_env.sh copy template $db noconfirm

if [ $? != 0 ]; then
  return 1;
fi
