$! 
$! Proview   $Id: wb_rttsys_edit.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! Commandfile to start the rtt editor for system picture library
$!
$ define/job pwrp_rtt pwr_root:[src.lib.dtt.src]
$ define/job pwrp_rttbld pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.bld.lib.dtt]
$ mc pwr_exe:wb_rtt rttsys
$ deassign/job pwrp_rtt
$ deassign/job pwrp_rttbld
