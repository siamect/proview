$! rs_mh_handler_run.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! This command file is to be executed by the detached process
$! that runs the Proview/R Event Monitor.
$!
$ set noon
$! @login
$ node = f$getsyi ("NODENAME")
$ grp = f$getjpi ("","GRP")
$ prcnam = f$fao ("* Msgh_!3OL *", grp)
$ set process/name="''prcnam'"
$!
$!
$! Check where the log should go...
$!
$ output = f$trnlnm ("MSGH_OUTPUT")
$ write sys$output "Redirecting output to ''output'"
$ if output .nes. ""
$ then
$   windownam = f$fao ("Message handler !3OL on !AS", grp, node)
$   iconnam = f$fao ("Msgh !3OL !AS", grp, node)
$   crea/nolog/term-
	/display='output'/noprocess-
	/define=msgh_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define sys$output msgh_tty:
$   define sys$input msgh_tty:
$ endif
$!
$!
$! Does he want it to run with the debugger?
$!
$ if f$trnlnm ("MSGH_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$ run pwr_exe:rt_eventmonitor.exe 'debug'
$ exit
