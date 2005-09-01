$! 
$! Proview   $Id: op_htre_start.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! File: ps_htre_start.com
$! 
$! Setups logical names for historical trend and data storage, starts the fetch 
$! and print processes.
$!
$!
$! 
$   file = f$search("pwrp_exe:op_htre_define_logname")
$   if file .eqs "" then file = f$search("pwr_exe:op_htre_define_logname")  
$
$   if file .eqs "" 
$   then
$	write sys$output "Start HTrend: Can't define logical names"
$   else
$	@ 'file'
$   endif
$!
$   @pwr_exe:op_htre_start_fetch
$   @pwr_exe:op_htre_start_print
$!
$ exit:
