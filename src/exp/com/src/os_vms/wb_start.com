$! 
$! Proview   $Id: wb_start.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! Start the navigator
$!
$! Arguments	
$!		p1	Username
$!		p2	Password
$!		p3	Database Id
$!		p4	Volume
$!
$ on error then goto exit_now
$ on control_y then goto exit_now
$ beep = ""
$ say = "write sys$output"
$
$ if p3.nes.""
$ then
$   define/proc pwrp_db pwrp_root:[common.db.'p3']
$ endif
$
$ if ( f$search("pwrp_db:wb.rdb").eqs."")
$ then
$   say "** Database ''p3' does not exist ''beep'"
$   beep
$ else
$   mc pwr_exe:wb "''p1'" "''p2'" "''p4'"
$ endif
$
$exit_now:
$ if f$trnlnm( "pwrp_db", "lnm$process").nes."" then deassign/proc pwrp_db
