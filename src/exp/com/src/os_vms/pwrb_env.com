$! 
$! Proview   $Id: pwrb_env.com,v 1.3 2005-09-01 14:57:49 claes Exp $
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
$! pwrb_env.com -- <short description>
$!
$!
$! Commands
$!
$	say := write sys$output
$	command = f$edit(p1, "trim,lowercase")
$
$	if command .eqs. "setroot" then goto c_setroot
$	if command .eqs. "unsetroot" then goto c_unsetroot
$	if command .eqs. "create" then goto c_create
$	if command .eqs. "define" then goto c_define
$	if command .eqs. "undefine" then goto c_undefine
$	if command .eqs. "show" then goto c_show
$	if command .eqs. "help" then goto c_help
$	if command .eqs. "" then goto c_help
$	say "%PWRB_ENV-F-UNKNOWNCMD, Unknown command """, command, """"
$
$exit_now:
$	exit
$
$!
$! env help
$!
$c_help:
$	type sys$input:
pwrb_env setroot <directory>

	sets root to base tree.

pwrb_env unsetroot <directory>

	deassigns the root to base tree.

pwrb_env define

	defines logical names to the base tree.

pwrb_env fill

	fills the basesystem

pwrb_env show

	shows current definitions of directory trees.

pwrb_env undefine

	deassigns logical names to the base tree.

pwrb_env help

	shows this helptext.

$	goto exit_now
$
$! benv setroot <directory>
$!
$c_setroot:
$
$	if f$trnlnm("PWRE_ENVIRONMENT") .nes. "" then goto exit_now
$
$	defjob = "define/job/nolog"
$	br := """[""" - """]""" - """<""" - """>"""
$	dev	= p2
$	if f$locate(":",dev) .ge. f$length(dev)
$	then
$	  dev = f$parse("sys$login",,,"device","no_conceal") - ":"
$	  dir = f$parse("sys$login",,,"directory","no_conceal") - 'br' - 'br'
$	  dir = "''dir'" + "''p2'"  - 'br' - 'br'
$	else
$	  dev = f$parse(p2,,,"device","no_conceal") - ":"
$	  dir =  f$parse(p2,,,"directory","no_conceal") - 'br' - 'br'
$	endif
$	if .not. f$getdvi(dev,"exists")
$	then
$	  say "Device does not exist: ''dev'"
$	  goto exit_now
$	endif
$	dev = f$getdvi("''dev'","devnam") - "_" - ":"
$	root == "''dev'" + ":" + "[" + "''dir'" + "]"
$	dir =  f$parse(root,,,"directory") - 'br' - 'br'
$	dir = dir - "000000."
$	root == "''dev'" + ":" + "[" + "''dir'" + ".]"
$
$	if p3 .nes. "__env_export_root"
$	then
$	  defjob/trans=(conc,term) pwrb_root	'root'
$	else
$	  define/nolog/trans=(conc,term) 'p4'	'root'
$	endif
$	goto exit_now
$
$
$! benv setroot <directory>
$!
$c_unsetroot:
$	call undefjob pwrb_root
$	goto exit_now
$
$
$c_show:
$	r = f$trnlnm("pwrb_root")
$	if r .eqs. "" then r = "not defined."
$	say "pwrb_root is ", r
$
$	goto exit_now
$
$c_define:
$
$	if f$trnlnm("PWRE_ENVIRONMENT") .nes. "" then goto c_define_pwra_env
$
$       dbg = ""
$	if p2.eqs."debug" then dbg = "_dbg"
$
$	hardware = f$extract(0,3,f$getsyi("hw_name"))
$	if hardware .eqs. "VAX" 
$       then 
$         pwre_os = "VMS"
$         pwre_hw = "VAX"
$       else
$         pwre_os = "VMS"
$         pwre_hw = "AXP"
$       endif
$	defjob = "define/job/nolog"
$
$       defjob pwr_exe          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.exe'dbg']
$       defjob pwr_exe_en_us    pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.exe'dbg'.en_us]
$       defjob pwr_exe_sv_se    pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.exe'dbg'.sv_se]
$       defjob pwr_lib          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.lib'dbg']
$       defjob pwr_obj          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.obj'dbg']
$       defjob pwr_load         pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.load]
$       defjob pwr_cnf          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.cnf]
$       defjob pwr_db           pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.db]
$       defjob pwr_doc          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.doc]
$       defjob pwr_inc          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.inc]
$       defjob pwr_lis          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.lis]
$       defjob pwr_src          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.src]
$       defjob pwr_kit          pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.exp.kit]
$
$	defjob pwr_olb		pwr_lib:pwr_olb.olb
$	defjob pwr_olb_eln	pwr_lib:pwr_olb.olb_eln
$
$c_define_pwra_env:
$
$	@pwr_exe:pwra_env define	
$
$	goto exit_now
$
$c_undefine:
$
$	if f$trnlnm("PWRE_ENVIRONMENT") .nes. "" then goto exit_now
$
$       call undefjob pwr_exe          
$       call undefjob pwr_exe_en_us    
$       call undefjob pwr_exe_sv_se    
$       call undefjob pwr_lib          
$       call undefjob pwr_obj          
$       call undefjob pwr_load          
$       call undefjob pwr_cnf          
$       call undefjob pwr_db           
$       call undefjob pwr_doc          
$       call undefjob pwr_inc          
$       call undefjob pwr_lis          
$       call undefjob pwr_src          
$       call undefjob pwr_kit          
$	call undefjob pwr_olb
$	call undefjob pwr_olb_eln
$
$	goto exit_now
$
$c_create:
$
$	say "Create is obsolete"
$	goto exit_now
$
$exit
$
$undefjob: subroutine
$	  if f$trnlnm( p1 ) .nes. "" then deassign/job 'p1'
$	endsubroutine
$!
$!                             pwrb_root
$!                                 |
$!    +-------+-------+-------+----+--+-------+-------+-------+------+
$!    |       |       |       |       |       |       |       |      |
$!   db      doc     exe     inc     lib     lis     obj     src    ...
$!
