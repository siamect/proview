$! rt_ini_neth_start.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! This command file creates a detached process
$! that runs the Proview/R Net Handler.
$!
$ set noon
$ required_priv = "ALTPRI,DETACH"
$ set proc/priv = ('required_priv')
$ if .not.f$privilege ("''required_priv'")
$ then
$  write sys$output "You need the following privileges: ''required_priv'"
$  exit
$ endif
$ message = f$environment("message")
$ set message/nofacility/noseverity/noidentification/notext
$ purge/nolog 'f$trnlnm("sys$login")'neth_vms.log/keep=3
$ set message'message'
$ cmd = f$search ("pwr_exe:rs_ini_neth_run.com")
$ write sys$output "Creating process for Net Handler..."
$ run	/detach/dump-
	/output='f$parse("sys$login:neth_vms.log",,,,"no_conceal")'-
	/input='f$parse(cmd,,,,"no_conceal")'-
	/process_name="Neth_start"-
	/prio=6-
	/job=0-
	sys$system:loginout.exe
$ exit
