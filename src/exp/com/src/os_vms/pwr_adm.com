$! pwr_adm.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! Note:
$!	This file requires that the logical name PWR_DISK is defined
$!
$!	define/system/trans=(concealed,terminal) pwr_disk dev:[pwr.]
$!
$	say = "write sys$output"
$	projdb = "pwr_disk:[adm.db]pwr_project.dat"
$!
$! commands
$!
$	command = f$edit(p1, "trim,lowercase")
$
$	if command .eqs. "setproj" then goto c_setproj
$	if command .eqs. "delete" then goto c_delete
$	if command .eqs. "proj_user" then goto c_proj_user
$	if command .eqs. "define" then goto c_define
$	if command .eqs. "show" then goto c_show
$	if command .eqs. "list" then goto c_list
$	if command .eqs. "init" then goto c_init
$	if command .eqs. "help" then goto c_help
$	if command .eqs. "?" then goto c_help
$	if command .eqs. "" then goto c_help
$	say "PWR_ADM-F-UNKNOWNCMD, Unknown command """, command, """"
$
$	goto exit_now
$
$check_basesystems:
$	  syms = ", %PWR_ENVIRONMENT, %PWR_LOCAL, %PWR_UPDATE, %PWR_GLOBAL,  "
$	  level = basesys - "%PWR_"
$	  if f$locate( ", "+basesys+",", syms) .eqs. f$length(syms)
$	  then
$	    syms = syms - ", " - ",  "
$	    say "%Invalid development base system name ''basesys' specificed"
$	    say "%Valid development base systems are;
$	    say "%      ", syms
$	    goto exit_now
$	  endif
$ return
$
$!
$! adm help
$!
$c_help:
$	type sys$input:
pwr_adm setproj <project> <base system root> <project root>

pwr_adm init <adm root>

pwr_adm show <project>

pwr_adm list

pwr_adm define <project> <local root>

pwr_adm delete <project>
$
$	goto exit_now
$
$c_help_proj_user:
$	type sys$input:
pwr_project show <project>

pwr_project list

pwr_project define <project> <local root>
$
$	goto exit_now
$
$c_proj_user:
$
$	sub_command = f$edit(p2, "trim,lowercase")
$
$	if sub_command .eqs. "define" then goto c_define_proj_user
$	if sub_command .eqs. "show" then goto c_show_proj_user
$	if sub_command .eqs. "list" then goto c_list_proj_user
$	if sub_command .eqs. "help" then goto c_help_proj_user
$	if sub_command .eqs. "?" then goto c_help_proj_user
$	if sub_command .eqs. "" then goto c_help_proj_user
$	say "PWR_PROJECT-F-UNKNOWNCMD, Unknown command """, command, """"
$
$	goto exit_now
$
$!
$! adm init 
$!
$c_init:
$
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
$	root = "''dev'" + ":" + "[" + "''dir'" + "]"
$	dir =  f$parse(root,,,"directory") - 'br' - 'br'
$	dir = dir - "000000."
$	root = "''dev'" + ":" + "[" + "''dir'" + ".]"
$
$	defjob = "define/job/nolog"
$	defjob/trans=(conc,term) pwra_root	'root'
$
$	defjob pwra_exe		pwra_root:[exe]
$	defjob pwra_db		pwra_root:[db]
$	defjob pwra_lis		pwra_root:[lis]
$
$	goto exit_now
$!
$! adm setproj 
$!
$c_setproj:
$
$	projid = f$edit(p2, "collapse,upcase")
$	basesys = f$edit(p3, "collapse,upcase")
$	levsys = f$edit(p4, "collapse,upcase")
$
$	if (projid .eqs. "") .or. (basesys .eqs "") .or (levsys .eqs. "")
$	then
$	  say "All parameters must be given"
$	  goto exit_now
$	endif
$
$	if f$extract(0,5,basesys) .eqs. "%PWR_" then gosub check_basesystems
$
$	if f$search("''projdb'") .eqs. ""
$	then
$	  create/fdl=sys$input 'projdb'
title	proview/r project databas
file
	organization		indexed
record
	size			512
key 0
	changes                 no
	name                    "ProjectId"
	prolog                  3
	length			32
	position		0
$	endif
$
$	keyprojid = "''f$fao("!32AS", projid )'"
$
$	open /share=read /read /write ff 'projdb'
$
$	line = f$fao("!32AS!128AS!128AS", projid, basesys, levsys)
$	read/delete /index=0 /key="''keyprojid'" /error=skip_err ff skip_read
$skip_err:
$	write/symbol ff line
$
$	close ff
$
$	goto exit_now
$
$c_show_line:
$	projid = "''f$extract(0, 32, line)'"
$	basesys = "''f$extract(32, 128, line)'"
$	levsys = "''f$extract(32+128, 128, line)'"
$
$	say "Project...........: ", f$edit(projid, "collapse")
$	say "  Base system root: ", f$edit(basesys, "collapse")
$	say "  Project root....: ", f$edit(levsys, "collapse")
$	say ""
$	return
$
$c_show_proj_user:
$	projid = f$edit(p3, "collapse,upcase")
$	localsys = f$edit(p4, "collapse,upcase")
$	goto c_show_begin
$
$c_show:
$	projid = f$edit(p2, "collapse,upcase")
$	localsys = f$edit(p3, "collapse,upcase")
$
$c_show_begin:
$	if f$search("''projdb'") .eqs. ""
$	then
$	  say "Project database not created"
$	  goto exit_now
$	endif
$
$	if projid .eqs. "*" then goto c_list
$
$	currproj = 0
$	if projid .eqs. ""
$	then
$	  projid = f$trnlnm("pwrp_project")
$	  if projid .eqs. ""
$	  then
$	    say "%No current project"
$	    goto exit_now
$	  else
$	    currproj = 1
$	    say "Current project"
$	    say ""
$	  endif
$	endif
$
$	keyprojid = "''f$fao("!32AS", projid )'"
$
$	open /share=read /read ff 'projdb'
$	read/error=notfound/end=notfound /index=0 /key="''keyprojid'" ff line
$	close ff
$
$	gosub c_show_line
$
$	if currproj
$	then
$	  pwrp_env show
$	endif
$
$	goto exit_now
$
$! show all/specific
$
$c_list_proj_user:
$c_list:
$	if f$search("''projdb'") .eqs. ""
$	then
$	  say "Project database not created"
$	  goto exit_now
$	endif
$
$	open /share=read /read ff 'projdb'
$next_line:
$	read/end=last_line ff line
$
$	gosub c_show_line
$
$	goto next_line
$
$last_line:
$	close ff
$
$	goto exit_now
$
$!
$! define ...
$!
$c_define_proj_user:
$	projid = f$edit(p3, "collapse,upcase")
$	localsys = f$edit(p4, "collapse,upcase")
$	goto c_define_begin
$
$c_define:
$	projid = f$edit(p2, "collapse,upcase")
$	localsys = f$edit(p3, "collapse,upcase")
$
$c_define_begin:
$
$	if (projid .eqs. "") .or. (localsys .eqs "")
$	then
$	  say "All parameters must be given"
$	  goto exit_now
$	endif
$
$	if f$search("''projdb'") .eqs. ""
$	then
$	  say "Project database not created"
$	  goto exit_now
$	endif
$
$	keyprojid = "''f$fao("!32AS", projid )'"
$
$	open /share=read /read ff 'projdb'
$	read/error=notfound/end=notfound /index=0 /key="''keyprojid'" ff line
$	close ff
$
$	projid = "''f$extract(0, 32, line)'"
$	basesys = f$edit("''f$extract(32, 128, line)'", "collapse")
$	levsys = "''f$extract(32+128, 128, line)'"
$
$	if f$trnlnm("PWRE_BROOT") .eqs. ""
$	then
$	  br := """[""" - """]""" - """<""" - """>"""
$	  dev	= basesys
$	  if f$locate(":",dev) .ge. f$length(dev)
$	  then
$	    dev = f$parse("sys$login",,,"device","no_conceal") - ":"
$	    dir = f$parse("sys$login",,,"directory","no_conceal") - 'br' - 'br'
$	    dir = "''dir'" + "''basesys'"  - 'br' - 'br'
$	  else
$	    dev = f$parse(basesys,,,"device","no_conceal") - ":"
$	    dir = f$parse(basesys,,,"directory","no_conceal") - 'br' - 'br'
$	  endif
$	  if .not. f$getdvi(dev,"exists")
$	  then
$	    say "Device does not exist: ''dev'"
$	    goto exit_now
$	  endif
$	  dev = f$getdvi("''dev'","devnam") - "_" - ":"
$	  root = "''dev'" + ":" + "[" + "''dir'" + "]"
$	  dir =  f$parse(root,,,"directory") - 'br' - 'br'
$	  dir = dir - "000000."
$	  root = "''dev'" + ":" + "[" + "''dir'" + ".]"
$
$	  defjob = "define/job/nolog"
$	  defjob/trans=(conc,term) pwrb_root	'root'
$
$	  @pwrb_root:[exe]pwrb_env define
$
$	endif
$
$	pwrp_env :== @pwr_exe:pwrp_env
$	pwrp_env setroot 'f$edit(levsys, "collapse")' global
$	pwrp_env setroot 'f$edit(localsys, "collapse")' local
$	pwrp_env define local
$	old = f$trnlnm("pwrp_project")
$	if old .nes. ""
$	then
$	  deassign/job pwrp_project
$	endif
$	define/job pwrp_project 'projid'
$
$	goto exit_now
$
$notfound:
$	say "Project ''projid' does not exist."
$	close ff
$	goto exit_now
$!
$!
$! adm delete
$!
$c_delete:
$	projid = f$edit(p2, "collapse,upcase")
$
$	if (projid .eqs. "")
$	then
$	  say "A project id must be given"
$	  goto exit_now
$	endif
$
$	if f$search("''projdb'") .eqs. ""
$	then
$	  say "Project database does not exist"
$	  goto exit_now
$	endif
$
$	keyprojid = "''f$fao("!32AS", projid )'"
$
$	open /share=read /read /write ff 'projdb'
$
$	read/delete /index=0 /key="''keyprojid'"/error=notfound ff skip_read
$
$	close ff
$
$	goto exit_now
$
$
$exit_now:
$ exit
