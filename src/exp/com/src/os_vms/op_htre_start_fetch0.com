$! 
$! Proview   $Id: op_htre_start_fetch0.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$!*************************************************************************
$!*
$!* Filnamn:	op_htre_start_fetch0.com
$!*
$!* Kompilator:	VMS C ver 3.0
$!* 
$!*		Datum	Pgm.	Kodl.	Kommentar
$!* Versioner:	920810  LE
$!*             940610  LE              Added settingsfile
$!*
$!* Beskrivning: Startar upp fetch process nummer 0.
$!*
$!*************************************************************************
$!*************************************************************************
$ define/process sys$scratch pwrp_lis
$! define/process RDMS$DEBUG_FLAGS "P"
$! define/process RDMS$DEBUG_FLAGS_OUTPUT pwrp_lis:fetch0debug.lis
$ define/process RDMS$BIND_WORK_FILE pwrp_lis
$
$ fetch == "$pwr_exe:op_htre_fetch"
$
$ fetch 0 pwrp_exe:op_htre_settings.dat
$
$ exit
$!*************************************************************************
$!*************************************************************************
