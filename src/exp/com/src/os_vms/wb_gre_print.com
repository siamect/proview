$! 
$! Proview   $Id: wb_gre_print.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! wb_gre_print.com -- <short description>
$!
$! This command procedure convert and print a file.
$! It is called from gre_print_docobj in the DS_foe_gre.c module.
$!
$! arguments :
$! P1 is the name for the output file
$!
$! The print order is defined in the symbol ds_foe_gre_print.
$! ( ex : print/que=laser_post /form=laser_post )
$! If this symbol is undefined no print is done
$!
$
$ if  f$type ( pwr_foe_gre_print ) .eqs. "STRING" 
$ then
$     pwr_foe_gre_print 'P1'
$ endif
$ purge/nolog 'P1'
$ exit
