$! rt_ini_appl_start.com -- start an application
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! This procedure is responsible for starting a VMS application
$! defined by a $Appl object. The procedure is invoked with
$! a number of arguments from the body of the object. RS_INIT
$! waits for this procedure to complete before starting the next
$! application.
$!
$!  Arg  $Appl field    Description
$!   p1  FileName       Name of the application file. Must
$!                      be .COM or .EXE.
$!                      If blank filetype, this is assumed
$!                      to be a DCL command.
$!   p2  ProgramName    Process name (max 12 characters). May
$!                      be blank.
$!   p3  StartWithDebug Decimal value, nonzero implies debug.
$!                      Some restrictions apply...
$!   p4  JobPriority    VMS process priority
$!   p5  Arg            Argument string. May be blank.
$!
$ set noon
$ typetab = ".COM/.EXE/./"
$!
$! Work on input parameters
$!
$ node = f$getsyi ("NODENAME")
$ pid = f$getjpi ("", "PID")
$ name = f$parse (p1,,,"NAME")
$ type = f$parse (p1,,,"TYPE")
$!
$ if p2 .eqs. ""
$   then
$     programname = name
$   else
$     programname = p2
$   endif
$!
$! Tell him what we are trying to achieve...
$!
$ write sys$output -
	"%INIAPPL-I-START, Starting ""''p1'"" in process ""''programname'"""
$!
$! Make sure the file type is understood...
$!
$ if f$locate (type+"/", typetab) .eq. f$length (typetab)
$   then
$     write sys$output "%INIAPPL-F-LOADERROR, Unhandled file type ""''type'"""
$     exit
$   endif
$!
$ if type .nes. "." then expname = f$parse(p1,,,,"no_conceal")
$!
$ set process/name="''programname'"
$ set process/priority='p4'
$!
$ if type .eqs. ".COM"
$   then
$     @ 'expname' 'p5'/out='f$trnlnm("sys$output")'
$   endif
$!
$ if type .eqs. ".EXE"
$   then
$     if p3 .ne. 0
$       then
$         if p5 .nes. "" then -
$	       write sys$output -
		"%INIAPPL-W-DEBUG, Argument string not passed to application"
$	  run/debug 'expname'
$       else
$	  'name' := $'expname'
$	  'name' 'p5' /out='f$trnlnm("sys$output")'
$       endif
$   endif
$!
$ if type .eqs. "."
$   then
$     'name' 'p5'
$   endif
$!
$ exit
