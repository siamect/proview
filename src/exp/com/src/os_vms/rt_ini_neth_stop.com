$! 
$! Proview   $Id: rt_ini_neth_stop.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! rt_ini_neth_stop.com -- <short description>
$!
$! This command file stops the detached process
$! that runs the Proview/R Net Handler.
$!
$ set noon
$ grp = f$getjpi ("","GRP")
$ prcnam = f$fao ("* Neth_!3OL *", grp)
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnam'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnam
$ then
$ write sys$output "Net Handler process: ''prcnam' was not up."
$ else
$ write sys$output "Taking down the Net Handler: ''prcnam'"
$ stop "''prcnam'"
$ endif
$ if f$type(ctx) .eqs. "PROCESS_CONTEXT" then temp = f$context("process", ctx, "cancel")
$ exit
