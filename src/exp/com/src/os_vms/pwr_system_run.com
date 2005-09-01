$! 
$! Proview   $Id: pwr_system_run.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! pwr_system_run.com -- <short description>
$!
$! This command file starts rs_init which creates attached processes
$! that runs the 
$!   Proview/R Message Handler,
$!   Proview/R Net Handler,
$!
$ set noon
$ if f$trnlnm("sys$login") .eqs. ""
$ then
$   define/job sys$login 'f$environment("default")'
$ endif
$ @login
$!
$ sho log pwr*
$ node = f$getsyi ("NODENAME")
$ pwr_busid = "pwr_''node'_busid"
$ prcnam = "PWR_''f$trnlnm(pwr_busid)'"
$ set process/name="''prcnam'"
$ 
$!
$! Check where the log should go...
$!
$ output = f$trnlnm ("PWR_OUTPUT")
$ write sys$output "Redirecting output to ''output'"
$ if output .nes. ""
$ then
$   windownam = "PWR_''f$trnlnm(pwr_busid)'@''node'"
$   iconnam = windownam
$   crea/nolog/term-
	/display='output'/noprocess-
	/define=pwr_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define sys$output pwr_tty:
$   define sys$input pwr_tty:
$ endif
$!
$!
$! Does he want it to run with the debugger?
$!
$ if f$trnlnm ("PWR_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$!
$ write sys$output "Starting PROVIEW/R Init..."
$ run pwr_exe:rt_ini.exe 'debug'
$ exit
