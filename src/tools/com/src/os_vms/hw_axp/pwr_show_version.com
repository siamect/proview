$!
$! File:
$!	pwr_tools:pwr_show_version.com
$!
$! Description:
$!
$ label = 0
$ pwr__status	= %x10000000
$ pwr__success	= pwr__status + %x0001
$ status	= pwr__success
$ on control_y then goto control_y
$ on warning then goto error
$ saved_message		= f$environment("MESSAGE")
$ saved_default		= f$environment("DEFAULT")
$ set message/facility/severity/identification/text
$ 
$ delete	= "delete/nolog"
$ purge		= "purge/nolog"
$ copy		= "copy/log"
$ set		= "set"
$ say		= "write sys$output"
$ signal	= "@pwrd_exe:pwrd_library signal pwr"
$
$ infile  = p1
$ outfile = p2
$
$ label = 1
$ open/write/error=error cnf 'outfile'
$ label = 2
$ write/error=error cnf "Version information"
$ write/error=error cnf ""
$ write/error=error cnf "File.......: ''infile'"
$ write/error=error cnf "BuildLabel.: ''pwre_blabel'"
$ write/error=error cnf "BuildTime..: ''f$cvtime()'"
$ write/error=error cnf "BuildId....: ''f$getjpi("","username")'"
$ label = 3
$ close/error=error cnf
$ label = 5
$ purge 'outfile'
$ label = 6
$ goto exit
$
$eof:
$ signal e unexpeof "unexpected end of file"
$ goto error
$
$control_y:
$ status = %x0004
$ goto exit
$
$error:
$ status = $status
$ signal e unexpeof "unexpected error: ''label'" 'status'
$ if f$trnlnm("cnf") .nes. "" then close cnf
$ goto exit
$
$exit:
$ set noon
$ set message 'saved_message
$ set default 'saved_default
$ exit status .or. %x10000000
