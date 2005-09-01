$! 
$! Proview   $Id: op_htre_define_logname.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$!	Logical names for trend and data storage
$!
$ defg := define/group/nolog
$!
$ defg	op_htre_hd_db			"pwrp_db:rt_hd"
$!
$!
$ defg 	op_htre_realtime_count		30
$ defg	op_htre_trend_timeout		60
$ defg	op_htre_color_print		"no"
$ defg	op_htre_default_directory  	"pwrp_lis:"
$ defg	op_htre_default_extension	"ps"
$ defg	op_htre_default_destination 	"file"
$ defg	op_htre_default_printer		"lps17_ps"
$ defg  op_htre_groups_db		"pwrp_db:op_htre_groups"
$!
$!
$!
$ exit
