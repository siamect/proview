$! 
$! Proview   $Id: rt_ini_neth_run.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! rt_ini_neth_run.com -- run the net handler
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R Net Handler.
$!
$ set noon
$!@login
$ node = f$getsyi ("NODENAME")
$ grp = f$getjpi ("","GRP")
$ prcnam = f$fao ("* Neth_!3OL *", grp)
$ acpprcnam = f$fao ("* NethACP_!3OL *", grp)
$ set process/name="''prcnam'"
$!
$!
$! Check where the log should go...
$!
$ output = f$trnlnm ("NETH_OUTPUT")
$ write sys$output "Redirecting output to ''output'"
$ if output .nes. ""
$ then
$   windownam = f$fao ("Nethandler !3OL on !AS", grp, node)
$   iconnam = f$fao ("Neth !3OL !AS", grp, node)
$   crea/nolog/term-
	/display='output'/noprocess-
	/define=neth_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define sys$output neth_tty:
$   define sys$input neth_tty:
$ endif
$!
$! Start the Nethandler ACP
$!
$ spawn/nowait/process="''acpprcnam'" run/nodebug pwr_exe:rt_neth_acp
$!
$! Does he want it to run with the debugger?
$!
$ if f$trnlnm ("NETH_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ run pwr_exe:rt_neth.exe 'debug'
$ exit
