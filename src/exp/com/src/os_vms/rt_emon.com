$! rt_emon.com -- run the event monitor 
$!
$! PROVIEW/R
$! Copyright (C) 1996-99 by Mandator AB.
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R event monitor.
$!
$ set noon
$!
$! Check where the log should go...
$!
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ prcnam = f$edit(f$getjpi("", "prcnam"), "lowercase")
$ output = f$trnlnm ("EMON_OUTPUT")
$ if output .nes. ""
$ then
$   write sys$output "-- ''prcnam': redirecting output to ''output'"
$   windownam = f$fao("!AS@!AS", prcnam, node)
$   iconnam = f$fao("!AS@!AS", prcnam, node)
$   crea/term-
	/display='output'/noprocess-
	/define=emon_tty/insert-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'",-
	font="-*-Terminal-*-*-*--*-100-*-*-*-*-*-*")
$   define/nolog sys$output emon_tty:
$   define/nolog sys$input emon_tty:
$ endif
$!
$! Shall we run the debugger?
$!
$ if f$trnlnm ("EMON_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ run pwr_exe:rt_emon.exe 'debug'
$ exit
