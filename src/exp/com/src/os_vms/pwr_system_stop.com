$! pwr_system_stop.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! Stops all Proview/R system processes.
$!	
$! !!! Nota bene !!!
$!
$! This commandfile must be executed at sys$login on
$! a Proview/R project account.
$!
$ set noon
$ !
$ node = f$getsyi ("NODENAME")
$ pwr_busid = "pwr_''node'_busid"
$ prcnam = "PWR_''f$trnlnm(pwr_busid)'"
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnam'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnam
$ then
$ write sys$output "-- PROVIEW/R System process: ''prcnam' was not up."
$ else
$ write sys$output "-- Taking down the PROVIEW/R System process: ''prcnam'"
$ stop "''prcnam'"
$ endif
$ if f$type(ctx) .eqs. "PROCESS_CONTEXT" then temp = f$context("process", ctx,"cancel") 
$!
$ wait 00:00:02
$ exit
