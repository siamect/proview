$!
$! Start the navigator
$!
$! Arguments	
$!		p1	Username
$!		p2	Password
$!		p3	Database Id
$!		p4	Volume
$!
$ on error then goto exit_now
$ on control_y then goto exit_now
$ beep = ""
$ say = "write sys$output"
$
$ if p3.nes.""
$ then
$   define/proc pwrp_db pwrp_root:[common.db.'p3']
$ endif
$
$ if ( f$search("pwrp_db:wb.rdb").eqs."")
$ then
$   say "** Database ''p3' does not exist ''beep'"
$   beep
$ else
$   mc pwr_exe:wb "''p1'" "''p2'" "''p4'"
$ endif
$
$exit_now:
$ if f$trnlnm( "pwrp_db", "lnm$process").nes."" then deassign/proc pwrp_db
