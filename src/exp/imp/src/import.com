$! import.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! This file exports imported files to 
$! locations in the pwr_root:[base.*] tree.
$!
$ pwrexp__status	= %x10000000
$ pwrexp__success	= pwrexp__status + %x0001
$ status		= pwrexp__success
$ on control_y then goto control_y
$ on warning then goto error
$ saved_message		= f$environment("MESSAGE")
$ saved_default		= f$environment("DEFAULT")
$ saved_protection	= f$environment("PROTECTION")
$ set message/facility/severity/identification/text
$ set prot=(s:rwed,o:rwed,g:rwed,w:rwed)/def
$ 
$ scope		= f$extract(0,1,f$trnlnm(""%PWRD_SCOPE""))
$ pwr_version	= f$trnlnm("%PWRD_''scope'VERSION")
$ build_id	= f$getjpi("","username")
$ state_none	= 0
$ state_id	= 1
$ state_backup	= 2
$ state		= state_none
$ announce	= 0
$ 
$ delete	= "delete/nolog"
$ purge		= "purge/nolog"
$ copy		= "copy/log"
$ backup	= "backup"
$ set		= "set"
$ say		= "write sys$output"
$ signal	= "@pwrd_exe:pwrd_library signal pwrimp"
$ verb_list	= "CLEAN,IMPORT"
$
$ verb = "''p1'"
$! tmp_dir = "''p2'"
$ tmp_dir = "disk$algebra:[pwrd.imp.tmp]"
$ infile  = "''p3'"
$ if f$search(infile) .eqs. "" then signal e fnf "file not found, \''infile'\"
$ outfile = "''p4'"
$ root    = "''p5'"
$ @pwrd_exe:pwrd_library lookup_keyword pwrd_ "''verb'" 'verb_list e verb "command verb"
$ verb = pwrd_
$ @pwrd_exe:pwrd_library get_root pwrd_ pwrd_msg_ 'root'
$ if pwre_ .eqs. ""
$ then
$  signal f errdefroot "root could not be defined, \PWR_ROOT\" "''pwrd_msg_'"
$ endif
$! else
$!  define/nolog/trans=(conc,term) pwr_root "''pwrd_'"
$! endif
$ open/read/error=error dat 'infile'
$l10:
$ read dat line /error=error/end_of_file=eof
$ if f$extract(0,1,line) .eqs. "!" then goto l10
$
$ if f$locate("<identification>", line) .eq. 0
$ then
$   state = state_id
$   goto l10
$ endif
$
$ if f$locate("<backup>", line) .eq. 0
$ then
$   state = state_backup
$   set default 'tmp_dir'
$   if verb .eqs. "IMPORT"
$   then
$     signal i import "Product: ''product', ''version', ''description'"
$     announce	= 1
$     if f$search("[]*.*;*") .nes. "" then delete []*.*;*
$     backup pwrd_disk:[imp]'product'.archive/sav []
$   else
$!     signal i clean "Product: ''product', ''version', ''description'"
$   endif
$   goto l10
$ endif
$
$ if f$locate("<end>", line) .eq. 0
$ then
$   close/error=error dat
$   if verb .eqs. "IMPORT"
$   then
$     open/write/error=error cnf 'outfile'
$     write/error=error cnf "Product....: ''product'"
$     write/error=error cnf "Version....: ''version'"
$     write/error=error cnf "Description: ''description'"
$     write/error=error cnf "PWRversion.: ''pwr_version'"
$     write/error=error cnf "BuildTime..: ''f$cvtime()'"
$     write/error=error cnf "BuildId....: ''build_id'"
$     close/error=error cnf
$     purge 'outfile'
$   else
$     if f$search("''outfile'") .nes. "" then delete 'outfile';*
$   endif
$   if f$search("[]*.*;*") .nes. "" then delete []*.*;*
$   goto exit
$ endif
$
$ if state .eq. state_none
$ then
$   signal w wrstate "Error parsing line: \''line'\"
$   goto l10
$ endif
$
$ if state .eq. state_id
$ then
$   'line
$   goto l10
$ endif
$
$ if state .eq. state_backup
$ then
$   line = f$edit(line,"compress")
$   index = f$locate(" ", line)
$   if index .ge. f$length(line) then signal e notrg -
	"target missing in line, \''line'\"
$   src = f$extract(0,index,line)
$   trg = f$extract(index+1,9999,line)
$   if f$search(trg) .nes. ""
$   then
$     if verb .eqs. "CLEAN" .and. .not. announce
$     then
$       signal i clean "Product: ''product', ''version', ''description'"
$       announce = 1
$     endif
$     if verb .eqs. "CLEAN" then signal i cleanf "remove file: ''trg'"
$     delete/nolog 'trg';*
$   else
$     trg1 = trg + f$parse(src,,,"name","syntax_only") + f$parse(src,,,"type","syntax_only")
$     if f$search(trg1) .nes. ""
$     then
$       if verb .eqs. "CLEAN" .and. .not. announce
$       then
$         signal i clean "Product: ''product', ''version', ''description'"
$         announce = 1
$       endif
$	trg1 = f$edit(trg1, "lowercase")
$       if verb .eqs. "CLEAN" then signal i cleanf "remove file: ''trg1'"
$       delete/nolog 'trg1';*
$     endif
$   endif
$   
$   if verb .eqs. "IMPORT"
$   then
$     logtrg = trg
$     if f$parse(logtrg,,,"name","syntax_only") .eqs. "" then logtrg = trg + src
$     signal i impf "import file: ''logtrg'"
$     set sec/acl/dele=all 'src
$     set file/prot=(s:rwed,o:rwed,g:rwed,w:rwed) 'src
$     copy/nolog 'src 'trg
$     set noon
$     set message/nofacility/noseverity/noidentification/notext
$     purge 'trg
$     set message/facility/severity/identification/text
$     set on
$   endif
$   goto l10
$ endif
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
$ if f$trnlnm("cnf") .nes. "" then close cnf
$ if f$trnlnm("dat") .nes. "" then close dat
$ goto exit
$
$exit:
$ set noon
$ set message 'saved_message
$ set protection=('saved_protection')/default
$ set default 'saved_default
$ exit status .or. %x10000000
