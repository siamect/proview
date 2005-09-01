$! 
$! Proview   $Id: wb_open_db.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! Open a new terminal and start the navigator
$! This procedure is started from the navigator 'Open db' method for 
$! DbConfig and DbTrace objects.
$!
$! Arguments	
$!		p1	Database Id
$!		p2	Username
$!		p3	Password
$!		p4	Volume
$!		p5	Title
$!
$
$ beep = ""
$ say = "write sys$output"
$
$ if p1.eqs.""
$ then
$   write sys$output "** Database Id is missing ''beep'"
$   exit
$ endif
$ define/proc pwrp_db pwrp_root:[common.db.'p1']
$
$ if ( f$search("pwrp_db:wb.rdb").eqs."")
$ then
$   write sys$output "** Database ''p1' does not exist ''beep'"
$   exit
$ endif
$ if p4.eqs.""
$ then
$   write sys$output "-- Opening database ''p1'"
$ else
$   write sys$output "-- Opening database ''p1' volume ''p4'"
$ endif
$ create/terminal/wait -
	/window_attributes=(rows=24,title="PWR_DEV on WorkBench ''p5'",icon_name="PWR_DEV") -
	mc pwr_exe:wb "''p2'" "''p3'" "''p4'"
$
$ deassign/proc pwrp_db
