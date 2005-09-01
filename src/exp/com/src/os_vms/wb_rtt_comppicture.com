$! 
$! Proview   $Id: wb_rtt_comppicture.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$!
$! Compile datastruct of a picture with update items
$!
$ name = p1
$ pgmname = p2
$ is_rttsys = f$integer( p3)
$ opsys = f$integer( p4)
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
$!
$ if is_rttsys.eq.1
$ then
$   if current_opsys.eq.opsys_VAX_VMS
$   then
$     if opsys.eq.opsys_VAX_VMS
$     then
$       source = "pwrb_bldroot:[lib.dtt]''name'.c"
$       target = "pwrb_bldroot:[lib.dtt]''name'.obj"
$       dttlib = "pwr_lib:libpwr_dtt.olb"
$       cinc = "/include=(pwr_inc,pwrb_bldroot:[lib.dtt])"
$
$       cc /decc/float=d_float/inc=pwr_inc/exte=str/stan=rel/pref=all -
	  /define=(os_vms=1,os="vms")/noopt/debug/warn=disable=trailcomma -
	  'cinc' /obj='target' 'source'
$       libr 'dttlib' 'target'
$     else
$       source = "pwrb_bldroot:[lib.dtt]''name'.c"
$       target = "pwrb_bldroot:[lib.dtt]''name'.obj"
$       dttlib = "pwr_lib:libpwr_dtt.olb"
$       cinc = "/include=(pwr_inc,pwrb_bldroot:[lib.dtt])"
$
$       cc /vaxc/inc=pwr_inc -
	  /define=(os_eln=1,os="eln")/noopt/debug -
	  'cinc' /obj='target' 'source' + eln$:vaxelnc/lib
$       libr 'dttlib' 'target'
$     endif
$   else
$     source = "pwrb_bldroot:[lib.dtt]''name'.c"
$     target = "pwrb_bldroot:[lib.dtt]''name'.obj"
$     dttlib = "pwr_lib:libpwr_dtt.olb"
$     cinc = "/include=(pwr_inc,pwrb_bldroot:[lib.dtt])"
$
$     cc /decc/noop/debug/inc=pwr_inc/define=(os_vms=1,os=""vms"") -
     	/flo=g_flo/noansi/ass=noali/ext=str/nomem/stan=rel/pref=all/ree=mul -
	/warn=disable=trailcomma -
	'cinc' /obj='target' 'source'
$     libr 'dttlib' 'target'
$   endif
$   delete/nolog/noconf 'target'.*
$   delete/nolog/noconf 'source'.*
$ else
$!  Is not rttsys
$   if current_opsys.eq.opsys_VAX_VMS
$   then
$     if opsys.eq.opsys_VAX_VMS
$     then
$       source = "pwrp_rttbld:''name'.c"
$       target = "pwrp_rttbld:''name'.obj"
$	dttlib = "pwrp_lib:ra_rtt_''pgmname'_pict.olb"
$
$       cc /decc/float=d_float/exte=str/stan=rel/pref=all -
	  /define=(os_vms=1,os="vms")/noopt/debug/warn=disable=trailcomma -
	  /include=(pwr_inc,pwrp_rttbld) -
	  /obj='target' 'source'
$       if f$search("''dttlib'").eqs."" then libr/create 'dttlib'
$       libr 'dttlib' 'target'
$       delete/nolog/noconf 'target'.*
$       purge/nolog/noconf 'source'
$     endif
$     if opsys.eq.opsys_VAX_ELN
$     then
$       source = "pwrp_rttbld:''name'.c"
$       target = "pwrp_rttbld:''name'.obj"
$       dttlib = "pwrp_root:[vax_eln.lib]ra_rtt_''pgmname'_pict.olb"
$
$       define/user rpc eln$rpc_inc
$       define/user sys eln$
$       define/user vaxc$include eln$
$       define/user pwr_inc pwrb_root:[os_eln.hw_vax.exp.inc]
$       cc /vaxc /define=(os_eln=1,os="eln")/noopt/debug -
	    /include=(pwr_inc,pwrp_rttbld) -
	    /obj='target' 'source' + eln$:vaxelnc/lib
$       if f$search("''dttlib'").eqs."" then libr/create 'dttlib'
$       libr 'dttlib' 'target'
$       delete/nolog/noconf 'target'.*
$       purge/nolog/noconf 'source'
$     endif
$     if opsys.eq.opsys_AXP_VMS
$     then
$       p_rttbld = f$trnlnm("pwrp_rttbld")
$       br := """[""" - """]""" - """<""" - """>"""
$       dev = f$parse("''p_rttbld'",,,"DEVICE","NO_CONCEAL") - ":"
$       dir =  f$parse("''p_rttbld'",,,"DIRECTORY","NO_CONCEAL") - 'br' - 'br'
$       dev = f$getdvi("''dev'","devnam") - "_" - ":"
$       rttbld = "''dev'" + ":" + "[" + "''dir'" + "]"
$       p_root = f$trnlnm("pwrp_root")
$!      write sys$output "''rttbld'"
$!      write sys$output "''p_root'"
$       rsh 'pwr_axp_vms_host' @pwr_exe:wb_rtt_comppicture_remote -
		 "''name'" "''pgmname'" 'is_rttsys' 'opsys' 'rttbld' 'p_root'
$     endif
$     if opsys.eq.opsys_X86_LYNX
$     then
$       p_rttbld = f$trnlnm("pwrp_rttbld")
$       br := """[""" - """]""" - """<""" - """>"""
$       dev = f$parse("''p_rttbld'",,,"DEVICE","NO_CONCEAL") - ":"
$       dir =  f$parse("''p_rttbld'",,,"DIRECTORY","NO_CONCEAL") - 'br' - 'br'
$       dev = f$getdvi("''dev'","devnam") - "_" - ":"
$       rttbld = "''dev'" + ":" + "[" + "''dir'" + "]"
$       p_root = f$trnlnm("pwrp_root")
$!      write sys$output "''rttbld'"
$!      write sys$output "''p_root'"
$       rsh 'pwr_x86_lynx_host' "$pwr_exe/wb_rtt_comppicture_remote.sh" -
		 "''name'" "''pgmname'" 'is_rttsys' 'opsys' "'"'rttbld'"'" -
		 "'"'p_root'"'"
$     endif
$   endif
$   if current_opsys.eq.opsys_AXP_VMS
$   then
$     if opsys.eq.opsys_AXP_VMS
$     then
$       source = "pwrp_rttbld:''name'.c"
$       target = "pwrp_rttbld:''name'.obj"
$	dttlib = "pwrp_lib:ra_rtt_''pgmname'_pict.olb"
$
$       cc /decc/noop/debug/define=(os_vms=1,os=""vms"") -
     	  /flo=g_flo/noansi/ass=noali/ext=str/nomem/stan=rel/pref=all/ree=mul -
	  /warn=disable=trailcomma -
	/include=(pwr_inc,pwrp_rttbld) -
	/obj='target' 'source'
$       if f$search("''dttlib'").eqs."" then libr/create 'dttlib'
$       libr 'dttlib' 'target'
$       delete/nolog/noconf 'target'.*
$       delete/nolog/noconf 'source'.*
$     endif
$     if opsys.eq.opsys_VAX_VMS.or.opsys.eq.opsys_VAX_ELN
$     then
$       p_rttbld = f$trnlnm("pwrp_rttbld")
$       br := """[""" - """]""" - """<""" - """>"""
$       dev = f$parse("''p_rttbld'",,,"DEVICE","NO_CONCEAL") - ":"
$       dir =  f$parse("''p_rttbld'",,,"DIRECTORY","NO_CONCEAL") - 'br' - 'br'
$       dev = f$getdvi("''dev'","devnam") - "_" - ":"
$       rttbld = "''dev'" + ":" + "[" + "''dir'" + "]"
$       p_root = f$trnlnm("pwrp_root")
$	p_root = p_root - ":"
$       p_root2 = f$trnlnm("''p_root'")
$       if p_root2.nes."" then p_root=p_root2
$!      write sys$output "''rttbld'"
$!      write sys$output "''p_root'"
$       rsh 'pwr_vax_vms_host' @pwr_exe:wb_rtt_comppicture_remote -
		 "''name'" "''pgmname'" 'is_rttsys' 'opsys' 'rttbld' 'p_root'
$     endif
$     if opsys.eq.opsys_X86_LYNX
$     then
$       p_rttbld = f$trnlnm("pwrp_rttbld")
$       br := """[""" - """]""" - """<""" - """>"""
$       dev = f$parse("''p_rttbld'",,,"DEVICE","NO_CONCEAL") - ":"
$       dir =  f$parse("''p_rttbld'",,,"DIRECTORY","NO_CONCEAL") - 'br' - 'br'
$       dev = f$getdvi("''dev'","devnam") - "_" - ":"
$       rttbld = "''dev'" + ":" + "[" + "''dir'" + "]"
$       p_root = f$trnlnm("pwrp_root")
$	p_root = p_root - ":"
$       p_root2 = f$trnlnm("''p_root'")
$       if p_root2.nes."" then p_root=p_root2
$!      write sys$output "''rttbld'"
$!      write sys$output "''p_root'"
$       rsh 'pwr_x86_lynx_host' "$pwr_exe/wb_rtt_comppicture_remote.sh" -
		 "''name'" "''pgmname'" 'is_rttsys' 'opsys' "'"'rttbld'"'" -
		 "'"'p_root'"'"
$     endif
$   endif
$ endif
$ exit
