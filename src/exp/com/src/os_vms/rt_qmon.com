$! rt_qmon.com -- run the Qcom monitor
$!
$! PROVIEW/R
$! Copyright (C) 1997-99 by MandatorAB.
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R Qcom monitor.
$!
$ set noon
$!
$! Check where the log should go...
$!
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ prcnam = f$edit(f$getjpi("", "prcnam"), "lowercase")
$ output = f$trnlnm ("QMON_OUTPUT")
$ if output .nes. ""
$ then
$   write sys$output "-- ''prcnam': redirecting output to ''output'"
$   windownam = f$fao("!AS@!AS", prcnam, node)
$   iconnam = f$fao("!AS@!AS", prcnam, node)
$   crea/term/display='output'/noprocess/define=qmon_tty/insert-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'",-
	font="-*-Terminal-*-*-*--*-100-*-*-*-*-*-*")
$   define/nolog sys$output qmon_tty:
$   define/nolog sys$input qmon_tty:
$ endif
$!
$! Shall we run the debugger?
$!
$ if f$trnlnm ("QMON_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ set proc/prio=9
$ run pwr_exe:rt_qmon.exe 'debug'
$ exit
