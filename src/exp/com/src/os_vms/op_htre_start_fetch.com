$!*************************************************************************
$!**************************************************************************
$!*
$!*                 C O M A T O R   P R O C E S S 
$!*                 = = = = = = =   = = = = = = =
$!**************************************************************************
$!*
$!* Filnamn:	op_htre_start_fetch.com
$!*
$!* Kompilator:	VMS C ver 3.0
$!* 
$!*		Datum	Pgm.	Kodl.	Kommentar
$!* Versioner:	920810  LE
$!*             930502  LE              f$parse av input/output/error
$!*
$!* Beskrivning: Startar upp fetch processer. Processerna startas med 
$!*		 samma UIC som den anropande processen. Processerna får
$!*		 namn "FETCHX_gruppnr".
$!*
$!*************************************************************************
$!*************************************************************************
$!
$!
$! Get group number for this process
$!
$ uic = f$getjpi("","UIC")
$ grp = f$getjpi("","GRP")
$!
$ outfile = "pwrp_lis:" + ''f$fao("FETCH0_!3OL",grp)' + ".LOG"
$ def sys$output 'f$parse(outfile,,,,"no_conceal")'
$!
$ prcnamf0 = f$fao("PWR_FETCH0_!3OL",grp)
$!
$! Check if fetch 0 process is running
$!
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnamf0'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnamf0
$ then
$   write sys$output "Fetch process ''prcnamf0' was not up"
$ else
$   write sys$output "Taking down fetch process ''prcnamf0'"
$   stop "''prcnamf0'"
$ endif
$
$!
$! Start fetch 0 process again
$!
$ wait_fetch0:
$ ctx = ""
$ tmp = f$context("process", ctx, "prcnam", "''prcnamf0'", "eql")
$ if f$getjpi(f$pid(ctx), "prcnam") .nes. prcnamf0
$ then
$   write sys$output "Starting fetch process ''prcnamf0'"
$ else
$   write sys$output "Waiting for process ''prcnamf0' ... "
$   wait 00:00:01.00
$   goto wait_fetch0
$ endif
$!
$!
$ set process/name="''prcnamf0'"
$
$ @pwr_exe:op_htre_start_fetch0.com
$
$ exit
$!*************************************************************************
$!*************************************************************************
