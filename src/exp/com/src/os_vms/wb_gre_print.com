$! wb_gre_print.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
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
