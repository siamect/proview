$! rt_neth.com -- run the net handler
$!
$! PROVIEW/R
$! Copyright (C) 1996,97 by Comator Process AB.
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
$   crea/nolog/term-
	/display='output'/noprocess-
	/define=neth_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define sys$output neth_tty:
$   define sys$input neth_tty:
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
$ run pwr_exe:rt_neth.exe 'debug'
$ exit
