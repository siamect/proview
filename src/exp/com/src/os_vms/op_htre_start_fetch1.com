$!*************************************************************************
$!**************************************************************************
$!*
$!*                 C O M A T O R   P R O C E S S 
$!*                 = = = = = = =   = = = = = = =
$!**************************************************************************
$!*
$!* Filnamn:	op_htre_start_fetch1.com
$!*
$!* Kompilator:	VMS C ver 3.0
$!* 
$!*		Datum	Pgm.	Kodl.	Kommentar
$!* Versioner:	920810  LE
$!*             940610  LE              Added settingsfile
$!*
$!* Beskrivning: Startar upp fetch process nummer 1.
$!*
$!*************************************************************************
$!*************************************************************************
$
$ define/process sys$scratch pwrp_lis
$! define/process RDMS$DEBUG_FLAGS "T"
$! define/process RDMS$DEBUG_FLAGS_OUTPUT pwrp_lis:fetch1debug.lis
$ define/process RDMS$BIND_WORK_FILE pwrp_lis
$
$ fetch == "$pwr_exe:op_htre_fetch"
$
$ fetch 1 pwrp_exe:op_htre_settings.txt
$
$ exit
$!*************************************************************************
$!*************************************************************************
