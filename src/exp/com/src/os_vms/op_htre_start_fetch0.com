$!*************************************************************************
$!**************************************************************************
$!*
$!*                 C O M A T O R   P R O C E S S 
$!*                 = = = = = = =   = = = = = = =
$!**************************************************************************
$!*
$!* Filnamn:	op_htre_start_fetch0.com
$!*
$!* Kompilator:	VMS C ver 3.0
$!* 
$!*		Datum	Pgm.	Kodl.	Kommentar
$!* Versioner:	920810  LE
$!*             940610  LE              Added settingsfile
$!*
$!* Beskrivning: Startar upp fetch process nummer 0.
$!*
$!*************************************************************************
$!*************************************************************************
$ define/process sys$scratch pwrp_lis
$! define/process RDMS$DEBUG_FLAGS "P"
$! define/process RDMS$DEBUG_FLAGS_OUTPUT pwrp_lis:fetch0debug.lis
$ define/process RDMS$BIND_WORK_FILE pwrp_lis
$
$ fetch == "$pwr_exe:op_htre_fetch"
$
$ fetch 0 pwrp_exe:op_htre_settings.dat
$
$ exit
$!*************************************************************************
$!*************************************************************************
