$!
$! Start functions in navigator.
$!
$! Arguments	
$!		p1	Label
$!
$ beep[0,8] = %x07
$ say = "write sys$output"
$ goto LABEL_'p1'
$ exit
$ 
$
$LABEL_EDITGRAPH:
$
$ if f$type(PWR_START_GRAPH).eqs.""
$ then
$   create/terminal/wait -
	/window_attributes=(rows=24,title="Proview/R Graphical Editor", -
	icon_name="Graphical Editor") -
	run gms$home:[bin]draw2x
$ else
$   create/terminal/wait -
	/window_attributes=(rows=24,title="Proview/R Graphical Editor", -
	icon_name="Graphical Editor") -
	'pwr_start_draw2'
$ endif
$ exit
$
$LABEL_EDITRTT:
$
$ if f$search("ssab_exe:ds_rtt.exe").eqs.""
$ then
$   say "** Rtt Editor is not installed in this system, please contact your system manager"
$   exit
$ endif
$ if f$trnlnm( "pwrp_rtt").eqs.""
$ then
$   say "** Rtt directory is not created in your project"
$   exit
$ endif
$ create/terminal/wait -
	/window_attributes=(rows=24,title="Proview/R Rtt Editor", -
	icon_name="Rtt Editor") -
	run ssab_exe:ds_rtt
$ exit
$
