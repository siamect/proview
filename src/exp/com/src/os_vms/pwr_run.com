$! pwr_run.com -- run a system program
$!
$! PROVIEW/R
$! Copyright (C) 1999 by MandatorAB.
$!
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
