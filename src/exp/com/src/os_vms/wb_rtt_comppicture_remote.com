$! 
$! Proview   $Id: wb_rtt_comppicture_remote.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$ p_rttbld = p5
$ p_root = p6
$
$ platform = "VAX_VMS"
$ hardware = f$extract(0,3,f$getsyi("HW_NAME"))
$ if hardware .nes. "VAX" then platform = "AXP_VMS"
$
$ set ver
$ define /job pwrp_rttbld 'p_rttbld'
$ define /job/trans=conc pwrp_root 'p_root'
$ define /job pwrp_obj pwrp_root:['platform'.obj]
$ define /job pwrp_lib pwrp_root:['platform'.lib]
$ define /job pwrp_exe pwrp_root:['platform'.exe]
$
$ @pwr_exe:wb_rtt_comppicture 'p1' 'p2' 'p3' 'p4'
