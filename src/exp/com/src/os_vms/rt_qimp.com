$! rt_qimp.com -- run the Qcom monitor
$!
$! PROVIEW/R
$! Copyright (C) 1997 by Comator Process AB.
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R Qcom importer.
$!
$ set noon
$!
$! Check where the log should go...
$!
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ prcnam = f$edit(f$getjpi("", "prcnam"), "lowercase")
$ output = f$trnlnm ("QIMP_OUTPUT")
$ if output .nes. ""
$ then
$   write sys$output "-- ''prcnam': redirecting output to ''output'"
$   windownam = f$fao("!AS@!AS", prcnam, node)
$   iconnam = f$fao("!AS@!AS", prcnam, node)
$   crea/term/display='output'/noprocess/define=qimp_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define/nolog sys$output qimp_tty:
$   define/nolog sys$input qimp_tty:
$ endif
$!
$! Shall we run the debugger?
$!
$ if f$trnlnm ("QIMP_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ set proc/prio=9
$ run pwr_exe:rt_qimp.exe 'debug'
$ exit
