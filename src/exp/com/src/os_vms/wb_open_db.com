$!
$! Open a new terminal and start the navigator
$! This procedure is started from the navigator 'Open db' method for 
$! DbConfig and DbTrace objects.
$!
$! Arguments	
$!		p1	Database Id
$!		p2	Username
$!		p3	Password
$!		p4	Volume
$!		p5	Title
$!
$
$ beep = ""
$ say = "write sys$output"
$
$ if p1.eqs.""
$ then
$   write sys$output "** Database Id is missing ''beep'"
$   exit
$ endif
$ define/proc pwrp_db pwrp_root:[common.db.'p1']
$
$ if ( f$search("pwrp_db:wb.rdb").eqs."")
$ then
$   write sys$output "** Database ''p1' does not exist ''beep'"
$   exit
$ endif
$ if p4.eqs.""
$ then
$   write sys$output "-- Opening database ''p1'"
$ else
$   write sys$output "-- Opening database ''p1' volume ''p4'"
$ endif
$ create/terminal/wait -
	/window_attributes=(rows=24,title="PWR_DEV on WorkBench ''p5'",icon_name="PWR_DEV") -
	mc pwr_exe:wb "''p2'" "''p3'" "''p4'"
$
$ deassign/proc pwrp_db
