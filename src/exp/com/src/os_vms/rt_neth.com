$! rt_neth.com -- run the net handler
$!
$! PROVIEW/R
$! Copyright (C) 1996-98 by Mandator AB.
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R Net Handler.
$!
$ set noon
$!
$! Check where the log should go...
$!
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ prcnam = f$edit(f$getjpi("", "prcnam"), "lowercase")
$ output = f$trnlnm ("NETH_OUTPUT")
$ write sys$output "-- Redirecting output to ''output'"
$ if output .nes. ""
$ then
$   windownam = f$fao("!AS@!AS", prcnam, node)
$   iconnam = f$fao("!AS@!AS", prcnam, node)
$   crea/term-
	/display='output'/noprocess-
	/define=neth_tty/insert-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'",-
	font="-*-Terminal-*-*-*--*-100-*-*-*-*-*-*")
$   define/nolog sys$output neth_tty:
$   define/nolog sys$input neth_tty:
$ endif
$!
$! Shall we run the debugger?
$!
$ if f$trnlnm ("NETH_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ set proc/prio=8
$ run pwr_exe:rt_neth.exe 'debug'
$ exit
