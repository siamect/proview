$! rt_hd_store_start.com
$!
$! PROVIEW/R
$! Copyright (C) 1997-99 by Mandator AB.
$!
$! Starts the store process the process will be the named
$! "* Store_groupnr *". It's running as a subprocess to adm
$!
$!
$! Check where the log should go...
$!
$ node = f$edit(f$getsyi("nodename"), "lowercase")
$ pwr_busid = "pwr_''node'_busid"
$ prcnam = "PWR_HDSTORE_''f$trnlnm(pwr_busid)'"
$ set process/name="''prcnam'"
$ prcnam = f$edit(f$getjpi("", "prcnam"), "lowercase")
$ output = f$trnlnm ("HDSTORE_OUTPUT")
$ if output .nes. ""
$ then
$   write sys$output "-- ''prcnam': redirecting output to ''output'"
$   windownam = f$fao("!AS@!AS", prcnam, node)
$   iconnam = f$fao("!AS@!AS", prcnam, node)
$   crea/term/display='output'/noprocess/define=hdstore_tty-
	/window_attributes=(rows=48,title="''windownam'",icon="''iconnam'",-
	font="-*-Terminal-*-*-*--*-100-*-*-*-*-*-*")
$   define/nolog sys$output hdstore_tty:
$   define/nolog sys$input hdstore_tty:
$ endif
$!
$ define/process sys$scratch pwrp_lis
$! define/process RDMS$DEBUG_FLAGS "T"
$! define/process RDMS$DEBUG_FLAGS_OUTPUT pwrp_lis:fetch1debug.lis
$ define/process RDMS$BIND_WORK_FILE pwrp_lis
$
$ set working_set/quota=8192/EXTENT=32063
$ store :== $pwr_exe:rt_hd_store.exe
$
$ set proc/prio=6
$ store "''p1'"
$
$ exit
