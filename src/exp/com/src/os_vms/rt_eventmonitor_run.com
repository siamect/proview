$! 
$! Proview   $Id: rt_eventmonitor_run.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! rs_mh_handler_run.com -- <short description>
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R Event Monitor.
$!
$ set noon
$! @login
$ node = f$getsyi ("NODENAME")
$ grp = f$getjpi ("","GRP")
$ prcnam = f$fao ("* Msgh_!3OL *", grp)
$ set process/name="''prcnam'"
$!
$!
$! Check where the log should go...
$!
$ output = f$trnlnm ("MSGH_OUTPUT")
$ write sys$output "Redirecting output to ''output'"
$ if output .nes. ""
$ then
$   windownam = f$fao ("Message handler !3OL on !AS", grp, node)
$   iconnam = f$fao ("Msgh !3OL !AS", grp, node)
$   crea/nolog/term-
	/display='output'/noprocess-
	/define=msgh_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define sys$output msgh_tty:
$   define sys$input msgh_tty:
$ endif
$!
$!
$! Does he want it to run with the debugger?
$!
$ if f$trnlnm ("MSGH_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ run pwr_exe:rt_eventmonitor.exe 'debug'
$ exit
