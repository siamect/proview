$!
$! File: ps_htre_start.com
$! 
$! Setups logical names for historical trend and data storage, starts the fetch 
$! and print processes.
$!
$!
$! 
$   file = f$search("pwrp_exe:op_htre_define_logname")
$   if file .eqs "" then file = f$search("pwr_exe:op_htre_define_logname")  
$
$   if file .eqs "" 
$   then
$	write sys$output "Start HTrend: Can't define logical names"
$   else
$	@ 'file'
$   endif
$!
$   @pwr_exe:op_htre_start_fetch
$   @pwr_exe:op_htre_start_print
$!
$ exit:
