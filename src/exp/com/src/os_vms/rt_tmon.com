$! rt_tmon.com -- run the timer monitor
$!
$! PROVIEW/R
$! Copyright (C) 1997-99 by Mandator AB.
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R timer monitor.
$!
$ set noon
$!
$! Check where the log should go...
$!
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ prcnam = f$edit(f$getjpi("", "prcnam"), "lowercase")
$ output = f$trnlnm ("TMON_OUTPUT")
$ if output .nes. ""
$ then
$   write sys$output "-- ''prcnam': redirecting output to ''output'"
$   windownam = f$fao("!AS@!AS", prcnam, node)
$   iconnam = f$fao("!AS@!AS", prcnam, node)
$   crea/term/display='output'/noprocess/define=tmon_tty/insert-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'",-
	font="-*-Terminal-*-*-*--*-100-*-*-*-*-*-*")
$   define/nolog sys$output tmon_tty:
$   define/nolog sys$input tmon_tty:
$ endif
$!
$! Shall we run the debugger?
$!
$ if f$trnlnm ("TMON_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ run pwr_exe:rt_tmon.exe 'debug'
$ exit
