$! op_htre_start_print.com -- Start Historical Trending
$!
$! PROVIEW/R
$! Copyright (C) 1998 by Mandator AB.
$!
$! Start the print and fetch processes of Historical Trend
$!
$ uic = f$getjpi("","UIC")
$ grp = f$getjpi("","GRP")
$
$ outfile = "pwrp_lis:" + ''f$fao("PRINT1_!3OL",grp)' + ".LOG"
$ def sys$output 'f$parse(outfile,,,,"no_conceal")'
$ prcnamp1 = f$fao("PWR_PRINT1_!3OL",grp)
$ prcnamf1 = f$fao("PWR_FETCH1_!3OL",grp)
$!
$! Check if print 1 process is running
$!
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnamp1'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnamp1
$ then
$   write sys$output "Print process ''prcnamp1' was not up"
$ else
$   write sys$output "Taking down print process ''prcnamp1'"
$   stop "''prcnamp1'"
$ endif
$!
$! Check if fetch 1 process is running
$!
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnamf1'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnamf1
$ then
$   write sys$output "Fetch process ''prcnamf1' was not up"
$ else
$   write sys$output "Taking down fetch process ''prcnamf1'"
$   stop "''prcnamf1'"
$ endif
$!
$! Start fetch process again
$!
$ wait_fetch1:
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnamf1'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnamf1
$ then
$   write sys$output "Starting fetch process ''prcnamf1'"
$ else
$   write sys$output "Waiting for process ''prcnamf1' ... "
$   wait 00:00:01.00
$   goto wait_fetch1
$ endif
$!
$ outfile = "pwrp_lis:" + ''f$fao("FETCH1_!3OL",grp)' + ".LOG"
$ spawn	/nowait -
        /process =      "''prcnamf1'" -
        /output =       'f$parse(outfile,,,,"no_conceal")' -
	@pwr_exe:op_htre_start_fetch1.com
$!
$! Start print process again
$!
$ wait_print1:
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnamp1'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnamp1
$ then
$   write sys$output "Starting print process ''prcnamp1'"
$ else
$   write sys$output "Waiting for process ''prcnamp1' ... "
$   wait 00:00:01.00
$   goto wait_print1
$ endif
$!
$ set process/name="''prcnamp1'"
$
$ @pwr_exe:op_htre_start_print1.com
$
$ exit
