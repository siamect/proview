$! 
$! Proview   $Id: rt_ini_neth_start.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! rt_ini_neth_start.com -- <short description>
$!
$! This command file creates a detached process
$! that runs the Proview/R Net Handler.
$!
$ set noon
$ required_priv = "ALTPRI,DETACH"
$ set proc/priv = ('required_priv')
$ if .not.f$privilege ("''required_priv'")
$ then
$  write sys$output "You need the following privileges: ''required_priv'"
$  exit
$ endif
$ message = f$environment("message")
$ set message/nofacility/noseverity/noidentification/notext
$ purge/nolog 'f$trnlnm("sys$login")'neth_vms.log/keep=3
$ set message'message'
$ cmd = f$search ("pwr_exe:rs_ini_neth_run.com")
$ write sys$output "Creating process for Net Handler..."
$ run	/detach/dump-
	/output='f$parse("sys$login:neth_vms.log",,,,"no_conceal")'-
	/input='f$parse(cmd,,,,"no_conceal")'-
	/process_name="Neth_start"-
	/prio=6-
	/job=0-
	sys$system:loginout.exe
$ exit
