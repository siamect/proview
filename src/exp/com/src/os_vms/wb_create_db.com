$!
$!	Create the database directory and copy en empty database to the
$!	directory.
$!
$!	Wb will be copied from PWR_WB_DIRECTORY if this is defined as a
$!	logical name. Otherwise it will be copied from pwr_db.
$!
$!	Parameters	p1	database id
$!
$ db_dir = "pwrp_root:[common.db.''p1']"
$ if f$search( "pwrp_root:[common.db]''p1'.dir").eqs.""
$ then
$   create/directory/log 'db_dir'
$ endif
$ if f$search( "''db_dir'wb.rdb").eqs.""
$ then
$   write sys$output "-- Creating database ''db_dir'wb..."
$   if f$trnlnm("PWR_WB_DIRECTORY").nes.""
$   then
$     rmu /copy/online/directory='db_dir' pwr_wb_directory:wb
$   else
$     rmu /copy/online/directory='db_dir' pwr_db:wb
$   endif
$   if $status.eq.%X10000001.or.$status.eq.%X12C88700
$   then
$     write sys$output "-- Database ''db_dir'wb created"
$   else
$     write sys$output "** Error when creating database"
$   endif
$   set file 'db_dir'wb*.*.0 /prot=(s:rwed,o:rwed,g:re,w:re)
$ endif
