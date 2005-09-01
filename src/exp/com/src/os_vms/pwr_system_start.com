$! 
$! Proview   $Id: pwr_system_start.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! pwr_system_start.com -- <short description>
$!
$! This command file creates a detached process
$! that runs the Proview/R System.
$!
$ set noon
$ saved_default = f$environment("DEFAULT")
$ say = "write sys$output"
$!
$ set def sys$login:
$ required_priv = "DETACH"
$ set proc/priv = ('required_priv')
$ if .not.f$privilege ("''required_priv'")
$ then
$  say "!! You need the following privileges: ''required_priv'"
$  exit
$ endif
$ message = f$environment("message")
$ set message/nofacility/noseverity/noidentification/notext
$ set message'message'
$
$ node = f$getsyi("NODENAME")
$ pwr_busid = "pwr_''node'_busid"
$ busid = f$trnlnm(pwr_busid)
$ if busid .eqs. ""
$ then
$   say "!! No Qcom bus id is defined."
$   exit
$ endif
$!
$ say "-- Taking down processes for PROVIEW/R System"
$ @pwr_exe:pwr_system_stop.com
$!
$!
$!
$! Promt for backup
$!
$ no_ahead = f$getdvi(f$trnlnm("sys$command"), "TT_NOTYPEAHD")
$!
$ height = f$getdvi(f$trnlnm("sys$output"), "TT_PAGE")
$ set terminal/inquire/page='height'/notype_ahead
$!
$ line = "Y"
$!
$ say ""
$ read/time_out=15/prompt="?? Load backup file <Y|N> (default=Y):" sys$command line 
$!
$ line = f$extract(0,1,line)
$ if line .eqs. "N" .or. line .eqs. "n"
$ then
$   pwr_bck = "pwr_''node'_bck"
$   def/group/nolog 'pwr_bck' "NO"
$   say "-- No load of backup will take place"
$ else
$   pwr_bck = "pwr_''node'_bck"
$   def/group/nolog 'pwr_bck' "YES" 
$ endif
$!
$ if .not. no_ahead  then set terminal/type_ahead
$!
$!
$ say "-- Creating process for PROVIEW/R System"
$!
$!
$ pwr_node = "pwr_''node'_node"
$ if f$trnlnm("''pwr_node'") .nes. "" then deass/group 'pwr_node'
$ if p1 .nes. "" then def/group/nolog 'pwr_node' 'p1'
$!
$ term = f$trnlnm ("SYS$OUTPUT")
$ pwr_term = "pwr_''node'_term"
$ def/group/nolog 'pwr_term' 'term'
$ purge/nolog 'f$trnlnm("sys$login")'pwr_vms_'node'.log/keep=3
$ cmd = f$search ("pwr_exe:pwr_system_run.com")
$ run	/detach/dump-
	/input='f$parse(cmd,,,,"no_conceal")'-
	/output='f$parse("sys$login:pwr_vms_"+node+".log",,,,"no_conceal")'-
	/process_name="PWR_START"-
	/page_fil=200000-
	/queue_limit=1000-
        /maximum_working_set=8192-
	/working_set=300-
        /prio=6-
        /priv=share-
	/job=0-
	/subprocess_limit= 100-
	/buffer_limit=100000 -
	/io_buffered=400 -
	/io_direct=400 -
	/enqueue_limit=2000 -
	/ast_limit=1000 -
	/file_limit=100 -
	sys$system:loginout.exe
$!
$ set def 'saved_default
$ exit
