$! rt_ini_neth_stop.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! This command file stops the detached process
$! that runs the Proview/R Net Handler.
$!
$ set noon
$ grp = f$getjpi ("","GRP")
$ prcnam = f$fao ("* Neth_!3OL *", grp)
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnam'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnam
$ then
$ write sys$output "Net Handler process: ''prcnam' was not up."
$ else
$ write sys$output "Taking down the Net Handler: ''prcnam'"
$ stop "''prcnam'"
$ endif
$ if f$type(ctx) .eqs. "PROCESS_CONTEXT" then temp = f$context("process", ctx, "cancel")
$ exit
