$! 
$! Proview   $Id: pwr_restwb.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$!	PWR_RESTORE_WB
$!
$! 	File pwr_restwb.com
$!
$!	Command file started by wb_distr_db.
$!	Restore a workbench on an operator station.
$!
$!
$ set noverif
$!
$! USAGE:
$! Procedure restores the proview workbench
$!
$! ARGUMENTS :
$! None 
$!
$! This procedure is activated ( and communicate ) via decnet 
$! by wb_distribute
$! 
$ say :== write sys$output
$ on error then goto error_comm
$ on control_y then goto error_comm
$ if ( f$trnlnm ("sys$net") .eqs. "" ) 
$ then 
$   say "This procedure should be called by ssab_utl !!!"
$   exit
$ endif
$ say "*************************************************************"
$ say "PROCEDURE PWR_RESTWB "
$ say "*************************************************************"
$ open/read/write link sys$net
$
$! Say hello and wait for the wb_id
$ reply = "HELLO WB_DISTR_DB"
$ write link reply
$ read link wb_id
$ if wb_id.eqs."" 
$ then
$   wb_dir = "pwrp_root:[common.db]"
$   rbf_name = "pwrp_root:[common.db]wb_distr_db.rbf"
$ else
$   wb_dir = "pwrp_root:[common.db.'wb_id']"
$   rbf_name = "pwrp_root:[common.db]wb_distr_db_''wb_id'.rbf"
$   if f$search("pwrp_root:[common.db]''wb_id'.dir").eqs.""
$   then
$     create/directory pwrp_root:[common.db.'wb_id']
$   endif
$ endif
$ if f$search("''rbf_name'").eqs.""
$ then 
$     reply = "%DISTR-E-NORBF, file ''rbf_name' not found"
$     write link reply
$     goto exit_now:
$ endif
$! Delete old wb files first
$ write link "DELETING"
$ purge/nolog 'rbf_name'
$ delete/noconf/nolog 'wb_dir'wb.rdb.*
$ delete/noconf/nolog 'wb_dir'wb.rda.*
$ delete/noconf/nolog 'wb_dir'wb.snp.*
$ delete/noconf/nolog 'wb_dir'wb_body.rda.*
$ delete/noconf/nolog 'wb_dir'wb_body.snp.*
$ delete/noconf/nolog 'wb_dir'wb_head.rda.*
$ delete/noconf/nolog 'wb_dir'wb_head.snp.*
$ delete/noconf/nolog 'wb_dir'wb_sys.rda.*
$ delete/noconf/nolog 'wb_dir'wb_sys.snp.*
$ write link "RESTORING"
$ rmu/restore/new_version/nocdd/direct='wb_dir' 'rbf_name'
$ delete_/noconf/nolog 'rbf_name'.*
$ reply = "- wb restored on node ''f$getsyi( "NODENAME")'"
$ write link reply
$ goto exit_now
$
$exit_now:
$
$ close link
$ say "Procedure completed , decnet link closed " , f$time()
$ set noverif
$ exit 1
$!
$!
$! Something went wrong !!!
$!
$ error_comm:
$!
$! If the link is open signal the exit
$!
$ write link "SSABUTL-E-ERROR, something went wrong..."
$ close link
$ say " **Error** ERROR COMMUNICATION "
$ say "Procedure aborted " , f$time()
$ set noverif
$ exit
