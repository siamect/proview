$! rt_ini_neth_run.com -- run the net handler
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R Net Handler.
$!
$ set noon
$!@login
$ node = f$getsyi ("NODENAME")
$ grp = f$getjpi ("","GRP")
$ prcnam = f$fao ("* Neth_!3OL *", grp)
$ acpprcnam = f$fao ("* NethACP_!3OL *", grp)
$ set process/name="''prcnam'"
$!
$!
$! Check where the log should go...
$!
$ output = f$trnlnm ("NETH_OUTPUT")
$ write sys$output "Redirecting output to ''output'"
$ if output .nes. ""
$ then
$   windownam = f$fao ("Nethandler !3OL on !AS", grp, node)
$   iconnam = f$fao ("Neth !3OL !AS", grp, node)
$   crea/nolog/term-
	/display='output'/noprocess-
	/define=neth_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define sys$output neth_tty:
$   define sys$input neth_tty:
$ endif
$!
$! Start the Nethandler ACP
$!
$ spawn/nowait/process="''acpprcnam'" run/nodebug pwr_exe:rt_neth_acp
$!
$! Does he want it to run with the debugger?
$!
$ if f$trnlnm ("NETH_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ run pwr_exe:rt_neth.exe 'debug'
$ exit
