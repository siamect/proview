$! pwra_env.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$!    <Description>.  */
$!
$! Commands
$!
$	wr := write sys$output
$
$	command = f$edit(p1, "trim,lowercase")
$
$	if command .eqs. "define" then goto c_define
$	if command .eqs. "symbols" then goto c_symbols
$	if command .eqs. "help" then goto c_help
$	if command .eqs. "?" then goto c_help
$	if command .eqs. "" then goto c_help
$	write sys$output "%PWRP_ENV-F-UNKNOWNCMD, Unknown command """, command, """"
$
$exit_now:
$	exit
$
$!                                                          
$! env help
$!
$c_help:
$	type sys$input:
pwra_env define

	Defines logical names to adm tree.

pwra_env symbols

	Defines PROVIEW/R adm symbols

pwra_env help

	shows this helptext.

$	goto exit_now
$
$! penv <directory> local/global
$!
$
$c_symbols:
$
$	pwr_au :== $pwr_exe:ds_login
$
$	goto exit_now
$
$c_define:
$
$       aroot = "pwr_disk:[adm]"
$	root = f$parse(aroot,,,"device")-":"-":"
$	if f$trnlnm(root,,,,,"concealed")
$	then
$	  devroot = f$trnlnm( root )
$	  root = f$parse(devroot,,,"device")
$	  dir = f$parse(devroot,aroot,,"directory") - "[" - "]" - "<" - ">" - "[" - "]" - "<" - ">"
$	else
$	  root = root + ":"
$	  dir =  f$parse(aroot,,,"directory") - "[" - "]" - "<" - ">" - "[" - "]" - "<" - ">"
$	endif
$	root = root + "[" + dir + "]"
$
$	len = f$length( root )
$	root = f$extract( 0, len-1, root) + "." + f$extract(len-1, 1, root)
$	defjob = "define/job/nolog"
$
$	defjob/trans=(conc,term) pwra_root	'root'
$
$
$	defjob pwra_db		pwra_root:[db]
$	defjob pwra_exe		pwra_root:[exe]
$	defjob pwra_lis		pwra_root:[lis]
$
$	goto exit_now
$
$exit
$!
$!         pwra_root
$!            |
$!    +-------+-------+
$!    |       |       |
$!    db     exe     lis     
$!
$!
