$! pwr_system_run.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! This command file starts rs_init which creates attached processes
$! that runs the 
$!   Proview/R Message Handler,
$!   Proview/R Net Handler,
$!
$ set noon
$ if f$trnlnm("sys$login") .eqs. ""
$ then
$   define/job sys$login 'f$environment("default")'
$ endif
$ @login
$!
$ sho log pwr*
$ node = f$getsyi ("NODENAME")
$ pwr_busid = "pwr_''node'_busid"
$ prcnam = "PWR_''f$trnlnm(pwr_busid)'"
$ set process/name="''prcnam'"
$ 
$!
$! Check where the log should go...
$!
$ output = f$trnlnm ("PWR_OUTPUT")
$ write sys$output "Redirecting output to ''output'"
$ if output .nes. ""
$ then
$   windownam = "PWR_''f$trnlnm(pwr_busid)'@''node'"
$   iconnam = windownam
$   crea/nolog/term-
	/display='output'/noprocess-
	/define=pwr_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'")
$   define sys$output pwr_tty:
$   define sys$input pwr_tty:
$ endif
$!
$!
$! Does he want it to run with the debugger?
$!
$ if f$trnlnm ("PWR_DEBUG")
$ then
$	debug = "/debug"
$ else
$	debug = "/nodebug"
$ endif
$!
$ write sys$output "Starting PROVIEW/R Init..."
$ run pwr_exe:rt_ini.exe 'debug'
$ exit
