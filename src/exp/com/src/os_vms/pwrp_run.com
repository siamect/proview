$! 
$! Proview   $Id: pwrp_run.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! pwrp_run.com -- run a user program
$!
$ set noon
$!
$! Check where the log should go...
$!
$ file	= p1
$ pname	= p2
$ debug = p3
$ prio	= p4
$ arg	= p5
$
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ output = f$trnlnm ("PWRP_OUTPUT")
$ if debug .and. output .nes. ""
$ then
$   windownam = f$fao("!AS@!AS", pname, node)
$   iconnam = f$fao("!AS@!AS", pname, node)
$   crea/term/display='output'/noprocess/define='pname'_tty/insert-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'",-
	font="-*-Terminal-*-*-*--*-100-*-*-*-*-*-*")/little
$   define/nolog sys$output 'pname'_tty:
$   define/nolog sys$input 'pname'_tty:
$ endif
$
$ set proc/prio='prio'
$ exe = ""
$ com = ""
$ device = f$parse(file,"pwrp_root:",,"device","syntax_only")
$ directory = f$parse(file,"pwrp_root:",,"directory","syntax_only")
$ name = f$parse(file,,,"name","syntax_only")
$ type = f$parse(file,,,"type","syntax_only")
$ cfile = "''device'''directory'''name'"
$ if type .eqs. "."
$ then
$   efile = f$search("''cfile'.exe")
$   if efile .eqs. "" then efile = f$search("''cfile'.com")
$ else
$   efile = f$search("''cfile'''type'")
$ endif
$ if efile .eqs. ""
$ then
$   write sys$output "''name': no such file: ''file'"
$   exit
$ endif
$ type = f$parse(efile,,,"type","syntax_only")
$ if type .nes. ".COM"
$ then
$   if debug
$   then
$     mc pwr_exe:dbgset 'efile' 1
$   else 
$     mc pwr_exe:dbgset 'efile' 0
$   endif
$   mc 'efile' 'arg'
$ else
$   @'efile' 'arg'
$ endif
$	  
$ exit
