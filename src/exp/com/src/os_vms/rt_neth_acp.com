$! rt_neth_acp.com -- run the net handler ancillary control processor
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
$ output = f$trnlnm ("NACP_OUTPUT")
$ if output .nes. ""
$ then
$   write sys$output "-- ''prcnam': Redirecting output to ''output'"
$   windownam = f$fao("!AS@!AS", prcnam, node)
$   iconnam = f$fao("!AS@!AS", prcnam, node)
$   crea/term-
	/display='output'/noprocess-
	/define=nacp_tty/insert-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define/nolog sys$output nacp_tty:
$   define/nolog sys$input nacp_tty:
$ endif
$!
$! Shall we run the debugger?
$!
$ if f$trnlnm ("NACP_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ run pwr_exe:rt_neth_acp.exe 'debug'
$ exit
