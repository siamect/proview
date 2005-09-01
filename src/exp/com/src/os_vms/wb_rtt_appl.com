$! 
$! Proview   $Id: wb_rtt_appl.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$!**************************************************************************
$!	Filename: ds_rt_appl.com
$!
$!	Description: 	compile and link of rtt modules
$!	Parameters:	P1	program name
$!			P2	action (link or compile)
$!			P3	operating system
$!			P4	debug
$!
$!**************************************************************************
$
$ pgmname = p1
$ action = f$integer( p2)
$ opsys = f$integer( p3)
$ debug = f$integer( p4)
$
$ action_link = 1
$ action_compile = 2
$
$ opsys_VAX_ELN = 1
$ opsys_VAX_VMS = 2
$ opsys_AXP_VMS = 4
$ opsys_PPC_LYNX = 8
$ opsys_X86_LYNX = 16
$
$ current_opsys = opsys_VAX_VMS
$ hardware = f$extract(0,3,f$getsyi("HW_NAME"))
$ if hardware .nes. "VAX" then current_opsys = opsys_AXP_VMS
$
$ if current_opsys.ne.opsys
$ then
$     p_rttbld = f$trnlnm("pwrp_rttbld")
$     br := """[""" - """]""" - """<""" - """>"""
$     dev = f$parse("''p_rttbld'",,,"DEVICE","NO_CONCEAL") - ":"
$     dir =  f$parse("''p_rttbld'",,,"DIRECTORY","NO_CONCEAL") - 'br' - 'br'
$     dev = f$getdvi("''dev'","devnam") - "_" - ":"
$     rttbld = "''dev'" + ":" + "[" + "''dir'" + "]"
$     p_root = f$trnlnm("pwrp_root")
$     p_root = p_root - ":"
$     p_root2 = f$trnlnm("''p_root'")
$     if p_root2.nes."" then p_root=p_root2
$ endif
$
$ if action.eq.action_link
$ then
$   if current_opsys.eq.opsys_VAX_VMS
$   then
$     if opsys.eq.opsys_VAX_VMS
$     then
$       if f$search("pwrp_exe:rt_rtt_appl.opt").nes.""
$       then
$         optfile = "pwrp_exe:rt_rtt_appl.opt"
$       else
$         optfile = "pwr_exe:rt_rtt_appl.opt"
$       endif
$       if debug.eq.1
$	then
$         linkflags = "/nosysshr/debug"
$         cflags = "/decc/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/noopt/debug/warn=disable=trailcomma"
$       else
$         linkflags = "/nodebug"
$         cflags = "/decc/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/warn=disable=trailcomma"
$       endif
$       cinc = "/incl=(pwr_inc,pwrp_rttbld)"
$	source = "pwrp_rttbld:dtt_appl_''pgmname'_m.c"
$	obj = "pwrp_obj:dtt_appl_''pgmname'_m.obj"
$       target = "pwrp_exe:rt_rtt_''pgmname'.exe"
$	rttlib = "pwrp_lib:ra_rtt_''pgmname'.olb"
$	rttlib_pict = "pwrp_lib:ra_rtt_''pgmname'_pict.olb"
$       if f$search("''rttlib'").eqs."" then libr/create 'rttlib'
$       if f$search("''rttlib_pict'").eqs."" then libr/create 'rttlib_pict'
$
$       cc 'cflags' 'cinc' /obj='obj' 'source'
$       purge/nolog 'source'
$       purge/nolog pwrp_rttbld:dtt_appl_'pgmname'_m.rh
$
$       link 'linkflags' /exe='target' -
		'obj', -
		'rttlib'/lib, -
		'rttlib_pict'/lib, -
		'optfile'/opt
$       delete/nolog/noconfirm 'obj';*
$
$     endif
$     if opsys.eq.opsys_VAX_ELN
$     then
$       if f$search("pwrp_root:[vax_eln.exe]rt_rtt_appl.opt").nes.""
$       then
$         optfile = "pwrp_root:[vax_eln.exe]rt_rtt_appl.opt"
$       else
$         optfile = "pwrb_root:[os_eln.hw_vax.exp.exe]rt_rtt_appl.opt"
$       endif
$       if debug.eq.1
$	then
$         linkflags = "/nosysshr/debug"
$         cflags = "/noopt/debug/define=(os_eln=1,os=""eln"")"
$       else
$         linkflags = "/nosysshr/nodebug"
$         cflags = "/define=(os_eln=1,os=""eln"")"
$       endif
$       cinc = "/incl=(pwr_inc,pwrp_rttbld)"
$	source = "pwrp_rttbld:dtt_appl_''pgmname'_m.c"
$	obj = "pwrp_root:[vax_eln.obj]dtt_appl_''pgmname'_m.obj"
$       target = "pwrp_root:[vax_eln.exe]rt_rtt_''pgmname'.exe"
$	rttlib = "pwrp_root:[vax_eln.lib]ra_rtt_''pgmname'.olb"
$	rttlib_pict = "pwrp_root:[vax_eln.lib]ra_rtt_''pgmname'_pict.olb"
$       if f$search("''rttlib'").eqs."" then libr/create 'rttlib'
$       if f$search("''rttlib_pict'").eqs."" then libr/create 'rttlib_pict'
$
$	define/user rpc eln$rpc_inc
$	define/user sys eln$
$	define/user vaxc$include eln$
$	define/user pwr_inc pwrb_root:[os_eln.hw_vax.exp.inc]
$       cc 'cflags' 'cinc' /obj='obj' 'source' +eln$:vaxelnc/lib
$       purge/nolog 'source'
$       purge/nolog pwrp_rttbld:dtt_appl_'pgmname'_m.rh
$
$!       define/user dcio eln$:dcio
$!       define/user cmsc eln$:cmsc
$!       define/user ipcshr eln$:ipcshr
$!       define/user langmsc eln$:langmsc
$!       define/user pascalmsc eln$:pascalmsc
$!       define/user libcommon eln$:libcommon
$!       define/user dap eln$:dap
$!       define/user elnaccess eln$:elnaccess
$!       define/user getmsgshr eln$:getmsgshr
$!       define/user fileutil eln$:fileutil
$!       define/user dmath eln$:dmath
$!       define/user dcmath eln$:dcmath
$!       define/user dda eln$:dda
$!       define/user prgloader eln$:prgloader
$!       define/user auxcshr eln$:auxcshr
$!       define/user libmsc eln$:libmsc
$!       define/user libvm eln$:libvm
$!       define/user dpascalio eln$:dpascalio
$!       define/user icsshr eln$:icsshr
$!       define/user latshr eln$:latshr
$!       define/user eln$pshr eln$:eln$pshr
$
$       link 'linkflags' /exe='target' -
		'obj', -
		'rttlib'/lib, -
		'rttlib_pict'/lib, -
		'optfile'/opt
$       delete/nolog/noconfirm 'obj';*
$     endif
$     if opsys.eq.opsys_AXP_VMS
$     then
$       rsh 'pwr_axp_vms_host' @pwr_exe:wb_rtt_appl_remote -
		"''pgmname'" 'action' 'opsys' 'debug' 'rttbld' 'p_root'
$     endif
$     if opsys.eq.opsys_X86_LYNX
$     then
$       rsh 'pwr_x86_lynx_host' $pwr_exe/wb_rtt_appl_remote.sh -
		"''pgmname'" 'action' 'opsys' 'debug' "'"'rttbld'"'" -
		"'"'p_root'"'"
$     endif
$   endif
$   if current_opsys.eq.opsys_AXP_VMS
$   then
$     if opsys.eq.opsys_AXP_VMS
$     then
$       if f$search("pwrp_exe:rt_rtt_appl.opt").nes.""
$       then
$         optfile = "pwrp_exe:rt_rtt_appl.opt"
$       else
$         optfile = "pwr_exe:rt_rtt_appl.opt"
$       endif
$       if debug.eq.1
$	then
$         linkflags = "/nosysshr/debug"
$         cflags = "/decc/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/noopt/debug/warn=disable=trailcomma"
$       else
$         linkflags = "/nodebug"
$         cflags = "/decc/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/noopt/debug/warn=disable=trailcomma"
$       endif
$       cinc = "/incl=(pwr_inc,pwrp_rttbld)"
$	source = "pwrp_rttbld:dtt_appl_''pgmname'_m.c"
$	obj = "pwrp_obj:dtt_appl_''pgmname'_m.obj"
$       target = "pwrp_exe:rt_rtt_''pgmname'.exe"
$	rttlib = "pwrp_lib:ra_rtt_''pgmname'.olb"
$	rttlib_pict = "pwrp_lib:ra_rtt_''pgmname'_pict.olb"
$       if f$search("''rttlib'").eqs."" then libr/create 'rttlib'
$       if f$search("''rttlib_pict'").eqs."" then libr/create 'rttlib_pict'
$
$       cc 'cflags' 'cinc' /obj='obj' 'source'
$       purge/nolog 'source'
$       purge/nolog pwrp_rttbld:dtt_appl_'pgmname'_m.rh
$
$       link 'linkflags' /exe='target' -
		'obj', -
		'rttlib'/lib, -
		'rttlib_pict'/lib, -
		'optfile'/opt
$       delete/nolog/noconfirm 'obj';*
$
$     endif
$     if opsys.eq.opsys_VAX_VMS.or.opsys.eq.opsys_VAX_ELN
$     then
$       rsh 'pwr_vax_vms_host' @pwr_exe:wb_rtt_appl_remote -
		"''pgmname'" 'action' 'opsys' 'debug' 'rttbld' 'p_root'
$     endif
$     if opsys.eq.opsys_X86_LYNX
$     then
$       rsh 'pwr_x86_lynx_host' "$pwr_exe/wb_rtt_appl_remote.sh" -
		"''pgmname'" 'action' 'opsys' 'debug' "'"'rttbld'"'" -
		"'"'p_root'"'"
$     endif
$   endif
$ endif
$
$ if action.eq.action_compile
$ then
$   if current_opsys.eq.opsys_VAX_VMS
$   then
$     if opsys.eq.opsys_VAX_VMS
$     then
$       if debug.eq.1
$	then
$         cflags = "/decc/noopt/debug/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/warn=disable=trailcomma"
$       else
$         cflags = "/decc/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/warn=disable=trailcomma"
$       endif
$       cinc = "/incl=(pwr_inc,pwrp_rttbld)"
$	source = "pwrp_rttbld:ra_rtt_''pgmname'.c"
$	obj = "pwrp_obj:ra_rtt_''pgmname'.obj"
$	rttlib = "pwrp_lib:ra_rtt_''pgmname'.olb"
$       if f$search("''rttlib'").eqs."" then libr/create 'rttlib'
$
$       cc 'cflags' 'cinc' /obj='obj' 'source'
$	library 'rttlib' 'obj'
$       delete/nolog/noconfirm 'obj';*
$
$     endif
$     if opsys.eq.opsys_VAX_ELN
$     then
$       if debug.eq.1
$	then
$         cflags = "/noopt/debug/define=(os_eln=1,os=""eln"")"
$       else
$         cflags = "/define=(os_eln=1,os=""eln"")"
$       endif
$       cinc = "/incl=(pwr_inc,pwrp_rttbld)"
$	source = "pwrp_rttbld:ra_rtt_''pgmname'.c"
$	obj = "pwrp_root:[vax_eln.obj]ra_rtt_''pgmname'.obj"
$	rttlib = "pwrp_root:[vax_eln.lib]ra_rtt_''pgmname'.olb"
$       if f$search("''rttlib'").eqs."" then libr/create 'rttlib'
$
$       cc 'cflags' 'cinc' /obj='obj' 'source'
$	library 'rttlib' 'obj'
$       delete/nolog/noconfirm 'obj';*
$
$     endif
$     if opsys.eq.opsys_AXP_VMS
$     then
$       rsh 'pwr_axp_vms_host' @pwr_exe:wb_rtt_appl_remote -
		"''pgmname'" 'action' 'opsys' 'debug' 'rttbld' 'p_root'
$     endif
$     if opsys.eq.opsys_X86_LYNX
$     then
$       rsh 'pwr_x86_lynx_host' "$pwr_exe/wb_rtt_appl_remote.sh" -
		"''pgmname'" 'action' 'opsys' 'debug' "'"'rttbld'"'" -
		"'"'p_root'"'"
$     endif
$   endif
$   if current_opsys.eq.opsys_AXP_VMS
$   then
$     if opsys.eq.opsys_AXP_VMS
$     then
$       if debug.eq.1
$	then
$         cflags = "/decc/noopt/debug/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/warn=disable=trailcomma"
$       else
$         cflags = "/decc/float=d_float/exte=str/stan=rel/pref=all" + -
	  "  /define=(os_vms=1,os=""vms"")/warn=disable=trailcomma"
$       endif
$       cinc = "/incl=(pwr_inc,pwrp_rttbld)"
$	source = "pwrp_rttbld:ra_rtt_''pgmname'.c"
$	obj = "pwrp_obj:ra_rtt_''pgmname'.obj"
$	rttlib = "pwrp_lib:ra_rtt_''pgmname'.olb"
$       if f$search("''rttlib'").eqs."" then libr/create 'rttlib'
$
$       cc 'cflags' 'cinc' /obj='obj' 'source'
$	library 'rttlib' 'obj'
$       delete/nolog/noconfirm 'obj';*
$
$     endif
$     if opsys.eq.opsys_VAX_VMS.or.opsys.eq.opsys_VAX_ELN
$     then
$       rsh 'pwr_vax_vms_host' @pwr_exe:wb_rtt_appl_remote -
		"''pgmname'" 'action' 'opsys' 'debug' 'rttbld' 'p_root'
$     endif
$     if opsys.eq.opsys_X86_LYNX
$     then
$       rsh 'pwr_x86_lynx_host' "$pwr_exe:wb_rtt_appl_remote.sh" -
		"''pgmname'" 'action' 'opsys' 'debug' "'"'rttbld'"'" -
		"'"'p_root'"'"
$     endif
$   endif
$ endif
$
$ exit
