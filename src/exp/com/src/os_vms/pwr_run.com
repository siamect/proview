$! 
$! Proview   $Id: pwr_run.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! pwr_run.com -- run a system program
$!
$ set noon
$!
$! Check where the log should go...
$!
$ file	= p1
$ name	= p2
$ debug = p3
$ prio	= p4
$ arg	= p5
$
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ output = f$trnlnm ("PWR_OUTPUT")
$ if debug .and. output .nes. ""
$ then
$   windownam = f$fao("!AS@!AS", name, node)
$   iconnam = f$fao("!AS@!AS", name, node)
$   crea/term/display='output'/noprocess/define='name'_tty/insert-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'",-
	font="-*-Terminal-*-*-*--*-100-*-*-*-*-*-*")/little
$   define/nolog sys$output 'name'_tty:
$   define/nolog sys$input 'name'_tty:
$ endif
$
$ file = f$parse(file,,,"name","syntax_only")
$ efile = f$search("pwr_exe:''file'.exe")
$ if efile .nes. ""
$ then
$   if debug
$   then
$     mc pwr_exe:dbgset 'efile' 1
$   else 
$     mc pwr_exe:dbgset 'efile' 0
$   endif
$ else
$   efile = f$search("pwr_exe:''file'.com")
$   if efile .nes. ""
$   then
$     efile = "@" + efile
$   else
$     write sys$output "''name': no such file: ''file'"
$     exit
$   endif
$ endif
$	  
$ set proc/prio='prio'
$ mc 'efile' 'arg'
$ exit
