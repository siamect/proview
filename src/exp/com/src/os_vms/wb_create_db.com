$! 
$! Proview   $Id: wb_create_db.com,v 1.2 2005-09-01 14:57:49 claes Exp $
$! Copyright (C) 2005 SSAB Oxelösund AB.
$!
$! This program is free software; you can redistribute it and/or 
$! modify it under the terms of the GNU General Public License as 
$! published by the Free Software Foundation, either version 2 of 
$! the License, or (at your option) any later version.
$!
$! This program is distributed in the hope that it will be useful 
$! but WITHOUT ANY WARRANTY; without even the implied warranty of 
$! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
$! GNU General Public License for more details.
$!
$! You should have received a copy of the GNU General Public License 
$! along with the program, if not, write to the Free Software 
$! Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
$!
$!
$!	Create the database directory and copy en empty database to the
$!	directory.
$!
$!	Wb will be copied from PWR_WB_DIRECTORY if this is defined as a
$!	logical name. Otherwise it will be copied from pwr_db.
$!
$!	Parameters	p1	database id
$!
$ db_dir = "pwrp_root:[common.db.''p1']"
$ if f$search( "pwrp_root:[common.db]''p1'.dir").eqs.""
$ then
$   create/directory/log 'db_dir'
$ endif
$ if f$search( "''db_dir'wb.rdb").eqs.""
$ then
$   write sys$output "-- Creating database ''db_dir'wb..."
$   if f$trnlnm("PWR_WB_DIRECTORY").nes.""
$   then
$     rmu /copy/online/directory='db_dir' pwr_wb_directory:wb
$   else
$     rmu /copy/online/directory='db_dir' pwr_db:wb
$   endif
$   if $status.eq.%X10000001.or.$status.eq.%X12C88700
$   then
$     write sys$output "-- Database ''db_dir'wb created"
$   else
$     write sys$output "** Error when creating database"
$   endif
$   set file 'db_dir'wb*.*.0 /prot=(s:rwed,o:rwed,g:re,w:re)
$ endif
