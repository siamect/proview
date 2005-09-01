$! 
$! Proview   $Id: pwrp_env.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! pwrp_env.com -- <short description>
$!
$	say := write sys$output
$       platform_list   = "AXP_VMS,HOST_VMS,VAX_ELN,VAX_VMS"
$!
$! Commands
$!
$	command = f$edit(p1, "trim,lowercase")
$
$	if command .eqs. "setroot" then goto c_setroot
$	if command .eqs. "setdb" then goto c_setdb
$	if command .eqs. "create" then goto c_create
$	if command .eqs. "define" then goto c_define
$	if command .eqs. "init" then goto c_init
$	if command .eqs. "show" then goto c_show
$	if command .eqs. "symbols" then goto c_symbols
$	if command .eqs. "help" then goto c_help
$	if command .eqs. "?" then goto c_help
$	if command .eqs. "" then goto c_help
$	say "%PWRP_ENV-F-UNKNOWNCMD, Unknown command """, command, """"
$
$exit_now:
$	exit
$
$!
$! env help
$!
$c_help:
$	type sys$input:
pwrp_env setroot <directory> [local, global]

	Sets root to local or global project tree.

pwrp_env setdb [db-id]

	Set database. If id is not supplied the project database
	will be set.

pwrp_env create  [local, global]

	Creates local or global project tree directories.

pwrp_env define  [local, global]

	Defines logical names to local or global project tree.

pwrp_env init [local, global]

	Copies a database to the global directory and a new .olb
	to the local and the global directory.

pwrp_env show

	Shows current definitions of project roots.

pwrp_env symbols

	Defines PROVIEW/R symbols

pwrp_env help

	shows this helptext.

$	goto exit_now
$
$! penv <directory> local/global
$!
$c_setroot:
$	platform = "VAX_VMS"
$	hardware = f$extract(0,3,f$getsyi("HW_NAME"))
$	if hardware .nes. "VAX" then platform = "AXP_VMS"
$	  
$	br := """[""" - """]""" - """<""" - """>"""
$	dev	= "''p2'"
$	if f$locate(":",dev) .ge. f$length(dev)
$	then
$	  dev = f$parse("sys$login",,,"device","no_conceal") - ":"
$	  dir = f$parse("sys$login",,,"directory","no_conceal") - 'br' - 'br'
$	  dir = "''dir'" + "''p2'"  - 'br' - 'br'
$	else
$	  dev = f$parse("''p2'",,,"device","no_conceal") - ":"
$	  dir =  f$parse("''p2'",,,"directory","no_conceal") - 'br' - 'br'
$	endif
$	if .not. f$getdvi("''dev'","exists")
$	then
$	  say "device does not exist: ''dev'"
$	  goto exit_now
$	endif

$	dev = f$getdvi("''dev'","devnam") - "_" - ":"
$	root = "''dev'" + ":" + "[" + "''dir'" + "]"
$	dir =  f$parse(root,,,"directory") - 'br' - 'br'
$	dir = dir - "000000."
$	root = "''dev'" + ":" + "[" + "''dir'" + ".]"
$	eroot = "''dev'" + ":" + "[" + "''dir'" + ".''platform'.]"
$
$	senv = f$extract(0,1,f$edit(p3, "collapse,trim,lowercase"))
$	if senv .eqs. "g"
$	then
$	  p3 = "g"
$	else
$	  p3 = "l"
$	endif
$	defjob = "define/job/nolog"
$	defjob/trans=(conc,term) pwrp_'p3'root	'root'
$	defjob/trans=(conc,term) pwrpe_'p3'root	'eroot'
$
$	goto exit_now
$
$c_setdb:
$	if p2.eqs.""
$	then
$	  define/job pwrp_db pwrp_root:[common.db]
$	else
$	  db = "pwrp_root:[common.db.''p2']wb.rdb"
$ 	  if f$search("''db'").eqs.""
$	  then
$	    say "%pwrp_env-e-db, no valid database ''p2'"
$	    goto exit_now
$	  endif
$	  define/job pwrp_db pwrp_root:[common.db.'p2']
$	endif
$	if f$trnlnm( "pwrp_db","lnm$process").nes.""
$	then
$	  deassign/process pwrp_db
$	endif
$	goto exit_now
$
$c_init:
$	senv = f$extract(0,1,f$edit(p2, "collapse,trim,lowercase"))
$	if senv .eqs. "g"
$	then
$	  copy/nolog pwr_db:wb.*; pwrp_gdb:
$	  libr/create pwrp_glib:ra_proj.olb
$	else
$	  copy pwrp_glib:ra_proj.olb; pwrp_llib:
$	endif
$
$	goto exit_now
$
$c_symbols:
$
$	pwr_pro*ject		:== @pwr_exe:pwr_adm proj_user
$	pwr_plc			:== $pwr_exe:wb_cmd
$	pwr_cmd			:== $pwr_exe:wb_cmd
$	pwr_dev			:== @pwr_exe:wb_start
$	pwr_ld*list		:== $pwr_exe:wb_ldlist
$	pwr_wbl*oad		:== $pwr_exe:wb_load
$	pwr_wbd*ump		:== $pwr_exe:wb_dump
$	pwr_gen*struct		:== $pwr_exe:wb_genstruct
$	pwr_s*tart		:== @pwr_exe:pwr_system_start
$	pwr_stop		:== @pwr_exe:pwr_system_stop
$	pwr_pcm			:== $pwr_exe:rt_pcm
$	pwr_w*atch		:== run/nodeb pwr_exe:rt_watch
$	pwr_pop			:== $pwr_exe:op
$	pwr_pop_stop		:== $pwr_exe:op_stop
$	pwr_rnav		:== $pwr_exe:op_rnav -name """standAlone"""
$	pwr_ev*entprinter	:== $pwr_exe:rt_eventprinter
$	pwr_estart		:== "write sys$output ""pwr_estart not yet defined!""
$	pwr_ewatch		:== set host
$	pwr_help		:== help/library=pwr_lib:wb_foe help
$	pwr_eh*elp		:== help/library=pwr_lib:wb_errorhelp errors
$	pwr_user		:== $pwr_exe:pwr_user
$	pwr_xtt			:== $pwr_exe:rt_xtt
$
$	goto exit_now
$
$c_show:
$	lr = f$trnlnm("pwrp_lroot")
$	if lr .eqs. "" then lr = "not defined."
$	gr = f$trnlnm("pwrp_groot")
$	if gr .eqs. "" then gr = "not defined."
$	say "pwrp_lroot is ", lr
$	say "pwrp_groot is ", gr
$	say ""
$	def = f$trnlnm("pwrp_define")
$	if def .eqs. ""
$	then
$	  say "PWRP environment is not defined."
$	else
$	  say "PWRP environment is: ", def
$	endif
$
$	goto exit_now
$
$c_define:
$
$	platform = "VAX_VMS"
$	hardware = f$extract(0,3,f$getsyi("hw_name"))
$	if hardware .nes. "VAX" then platform = "AXP_VMS"
$	  
$	defjob = "define/job/nolog"
$
$	senv = f$extract(0,1,f$edit(p2, "collapse,trim,lowercase"))
$	if senv .eqs. "g"
$	then
$         rout = "g"
$	  defjob pwrp_define	global
$	else
$         rout = "lg"
$	  defjob pwrp_define	local
$	endif
$
$       defjob pwrp_db		pwrp_groot:[common.db]
$	call def'rout'	pwrp	inc	"common.inc"
$	call def'rout'	pwrp	load	"common.load"
$	call def'rout'	pwrp	log	"common.log"
$	call def'rout'	pwrp	src	"common.src"
$	call def'rout'	pwrp	doc	"common.doc"
$	call def'rout'	pwrp	tmp	"common.tmp"
$	call def'rout'	pwrp	exe	"''platform'.exe"
$	call def'rout'	pwrp	lib	"''platform'.lib"
$	call def'rout'	pwrp	lis	"''platform'.lis"
$	call def'rout'	pwrp	obj	"''platform'.obj"
$
$	defjob pwrp_olb		pwrp_lib:pwrp_olb.olb
$	defjob pwrp_lolb	pwrp_llib:pwrp_olb.olb
$	defjob pwrp_golb	pwrp_glib:pwrp_olb.olb
$
$	defjob pwrp_olb_eln	pwrp_lib:pwrp_olb.olb_eln
$	defjob pwrp_lolb_eln	pwrp_llib:pwrp_olb.olb_eln
$	defjob pwrp_golb_eln	pwrp_glib:pwrp_olb.olb_eln
$
$	goto exit_now
$
$c_create:
$	senv = f$extract(0,1,f$edit(p2, "collapse,trim,lowercase"))
$	if senv .eqs. "g"
$	then
$         r = "g"
$	else
$         r = "l"
$	endif
$
$	create/dir/nolog pwrp_'r'root:[common.inc]
$	create/dir/nolog pwrp_'r'root:[common.doc]
$	create/dir/nolog pwrp_'r'root:[common.load]
$	create/dir/nolog pwrp_'r'root:[common.log]
$	create/dir/nolog pwrp_'r'root:[common.src]
$	create/dir/nolog pwrp_'r'root:[common.tmp]
$	if r .eqs. "g"
$	then
$         create/dir/nolog pwrp_'r'root:[common.db]
$	endif
$
$	i = 0
$l1:
$	dplatform = f$element(i, ",", platform_list)
$       i = i + 1
$       if dplatform .eqs. "," then goto l2
$       if dplatform .eqs. "HOST_VMS" then goto l1
$
$	create/dir/nolog pwrp_'r'root:['dplatform'.exe]
$	create/dir/nolog pwrp_'r'root:['dplatform'.lib]
$	create/dir/nolog pwrp_'r'root:['dplatform'.lis]
$	create/dir/nolog pwrp_'r'root:['dplatform'.obj]
$	goto l1
$l2:
$
$	goto exit_now
$
$!
$!
$!	call deflg	a	b	c	local/global
$!	generates	a_lb	pwrp_lroot:[c]
$!	 		a_gb	pwrp_groot:[c]
$!			a_b	a_lb:, a_gb:
$! example
$!	call deflg	utl	src	utl
$!
$!	generates 	utl_lsrc	pwrp_root:[utl]
$!	 		utl_gsrc	pwrp_root:[utl]
$!			utl_src		utl_lsrc:, utl_gsrc:
$!
$deflg: subroutine
$	  define/job/nolog 'p1'_l'p2'	pwrp_lroot:['p3']
$	  define/job/nolog 'p1'_g'p2'	pwrp_groot:['p3']
$	  define/job/nolog 'p1'_'p2'	'p1'_l'p2':, 'p1'_g'p2':
$	  define/job/nolog pwrp_root	pwrp_lroot:, pwrp_groot:
$	  define/job/nolog pwrpe_root	pwrpe_lroot:, pwrpe_groot:
$	endsubroutine
$
$!
$!
$!	call defg	a	b	c
$!	generates	a_gb	pwrp_root:[c]
$!			a_b	a_gb
$! example
$!	call deflg	utl	src	utl
$!
$!	generates 	utl_gsrc	pwrp_root:[utl]
$!			utl_src		utl_gsrc
$!
$defg: subroutine
$	  define/job/nolog 'p1'_g'p2'	pwrp_groot:['p3']
$	  define/job/nolog 'p1'_'p2'	'p1'_g'p2':
$	  define/job/nolog pwrp_root	pwrp_groot:
$	  define/job/nolog pwrpe_root	pwrpe_groot:
$	endsubroutine
$
$exit
$!
$!                             pwrp_root
$!                                 |
$!    +-------+-------+-------+----+--+-------+-------+-------+------+
$!    |       |       |       |       |       |       |       |      |
$!   load    exe     inc     lib     lis     src     obj     db     ...  
$!
$!
