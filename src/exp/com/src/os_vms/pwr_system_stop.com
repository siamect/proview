$! 
$! Proview   $Id: pwr_system_stop.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! pwr_system_stop.com -- <short description>
$!
$! Stops all Proview/R system processes.
$!	
$! !!! Nota bene !!!
$!
$! This commandfile must be executed at sys$login on
$! a Proview/R project account.
$!
$ set noon
$ !
$ node = f$getsyi ("NODENAME")
$ pwr_busid = "pwr_''node'_busid"
$ prcnam = "PWR_''f$trnlnm(pwr_busid)'"
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnam'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnam
$ then
$ write sys$output "-- PROVIEW/R System process: ''prcnam' was not up."
$ else
$ write sys$output "-- Taking down the PROVIEW/R System process: ''prcnam'"
$ stop "''prcnam'"
$ endif
$ if f$type(ctx) .eqs. "PROCESS_CONTEXT" then temp = f$context("process", ctx,"cancel") 
$!
$ wait 00:00:02
$ exit
