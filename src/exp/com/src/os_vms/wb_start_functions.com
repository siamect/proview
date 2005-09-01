$! 
$! Proview   $Id: wb_start_functions.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! Start functions in navigator.
$!
$! Arguments	
$!		p1	Label
$!
$ beep[0,8] = %x07
$ say = "write sys$output"
$ goto LABEL_'p1'
$ exit
$ 
$
$LABEL_EDITGRAPH:
$
$ if f$type(PWR_START_GRAPH).eqs.""
$ then
$   create/terminal/wait -
	/window_attributes=(rows=24,title="Proview/R Graphical Editor", -
	icon_name="Graphical Editor") -
	run gms$home:[bin]draw2x
$ else
$   create/terminal/wait -
	/window_attributes=(rows=24,title="Proview/R Graphical Editor", -
	icon_name="Graphical Editor") -
	'pwr_start_draw2'
$ endif
$ exit
$
$LABEL_EDITRTT:
$
$ if f$search("ssab_exe:ds_rtt.exe").eqs.""
$ then
$   say "** Rtt Editor is not installed in this system, please contact your system manager"
$   exit
$ endif
$ if f$trnlnm( "pwrp_rtt").eqs.""
$ then
$   say "** Rtt directory is not created in your project"
$   exit
$ endif
$ create/terminal/wait -
	/window_attributes=(rows=24,title="Proview/R Rtt Editor", -
	icon_name="Rtt Editor") -
	run ssab_exe:ds_rtt
$ exit
$
