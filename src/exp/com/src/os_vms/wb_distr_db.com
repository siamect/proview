$! 
$! Proview   $Id: wb_distr_db.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$!	WB_DISTR_DB
$!
$! 	File wb_distr_db.com
$!
$!	Command file started by the distributor.
$!
$!	Copy a workbench to an operator station.
$!	Takes a rmu backup of the workbench an copies it to
$!	the opstation. Starts ssab_rest_wb to restore wb on 
$!	the opstation.
$!
$!	p1	Database Id
$!	p2	Operator station
$!	p3	Project
$!
$ say :== write sys$output
$ ON ERROR THEN GOTO error
$ ON CONTROL_Y THEN GOTO error
$
$ if p2.eqs.""
$ then
$   say "Node is missing"
$   exit
$ endif
$
$ if p3.eqs.""
$ then
$   say "Project is missing"
$   exit
$ endif
$ wb_id = p1
$
$ if wb_id.eqs.""
$ then
$    wb_dir = "pwrp_root:[common.db]"
$    rbf_name = "wb_distr_db.rbf"
$ else
$    wb_dir = "pwrp_root:[common.db.''wb_id']"
$    rbf_name = "wb_distr_db_''wb_id'.rbf"
$ endif
$! Create a backup file
$ say "- Creating a backup file"
$ on error then goto rmuerror
$ rmu/backup/nolog/online 'wb_dir'wb 'wb_dir''rbf_name'
$
$ on error then goto error
$ i = 0
$loop:
$ node = f$element( i, "/", p2)
$ if node.eqs."/" then exit
$ node = f$edit( node, "COMPRESS, TRIM")
$ i = i + 1
$
$ say "- Copy backupfile to ''node'"
$ copy 'wb_dir''rbf_name' -
	'node'"''p3' ''p3'"::pwrp_root:[common.db]'rbf_name'
$ delete/nolog/noconf 'wb_dir''rbf_name'.*
$!
$!close the link in case it was already open
$!
$ set noon
$ set message/nofac/noid/nosev/notex
$ close link
$ set on
$ set message/fac/id/sev/tex
$! say "Starting communication with  " , node, " the " , f$extract(0,17,f$time())
$!
$! Verify that there is someone to talk to on the remote node
$!
$ file = "''node'""''p3' ''p3'""::pwr_restwb.com"
$ if ( f$search (file) .eqs. "" ) 
$ then
$   copy/nolog pwr_exe:pwr_restwb.com 'node'"''p3' ''p3'"::
$ endif
$!
$!
$! say "Reply within one minute to keep the line open "
$ open/read/write/error=error_link link -
	'node'"''p3' ''p3'"::"TASK=PWR_RESTWB"
$ read link remote_answer
$ if ( remote_answer.eqs."HELLO WB_DISTR_DB") 
$ then 
$   write link "''wb_id'"
$   read link remote_answer
$ endif
$ if ( remote_answer.eqs."DELETING") 
$ then 
$   say "- Delete old workbench on node ''node'"
$   read link remote_answer
$ endif
$ if ( remote_answer.eqs."RESTORING") 
$ then 
$   say "- Restore backup file on node ''node'"
$   read link remote_answer
$ endif
$!
$ say remote_answer
$ close link 
$ goto loop
$!
$! Cannot get link to remote node , inform the user
$!
$error_link:
$ say "%PWR-E-NOLINK, Unable to open link to ''node'"
$ goto loop
$!
$! Communication went bad
$!
$error:
$ set noon
$ set message/nofac/noid/nosev/notex
$ close link
$ IF ( $SEVERITY .EQ. 1 ) 
$ THEN
$   say "** ERROR** closing the line to the remote node "
$   say "** Look in sys$login:netserver.log on the remote node for more information"
$ ENDIF
$ set on
$ set message/fac/id/sev/tex
$ say " "
$ say "Procedure aborted...." , f$time()
$ goto loop
$!
$! Rmu backup error
$!
$rmuerror:
$ say "Close all other sessions to wb before copying"
$ say "Procedure aborted...." , f$time()
$ exit
