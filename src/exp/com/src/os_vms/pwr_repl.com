$!
$!
$!	p1 : volym
$!	p2 : fil
$!
$!	RENAME
$!
$! 	File dutil_src:rename.com
$!
$!	Modified	CS	97 04 10 	Skapad
$!
$!	Ändra namn på objekt.
$!
$ on error then goto error_exit
$ say := write sys$output
$
$ cmdfile = "sys$login:tmp_rename.tpu"
$
$ filecnt = 0
$LC_NextFile:
$ open/write/error=L_WriteFileError cf 'cmdfile'
$ oldname = p2
$ newname = p3
$
$! say "rename of:  ''oldname' -> ''newname'"
$ write cf "POSITION(SEARCH(BUFFER_BEGIN,REVERSE));"
$ write cf -
  "eve_global_replace( ''''"+"''oldname'"+"'''',''''"+"''newname'"+"'''');"
$ write cf "eve_exit;"
$ close cf
$
$ edit/tpu/command='cmdfile'/nodisplay 'p1'
$! delete/nolog/noconfirm 'cmdfile'.*
$
$ exit
$
$L_WriteFileError:
$ say "Error, Unable to open file ''cmdfile'"
$ close rf
$ exit
$
$
$
$error_exit:
$ say "Exekveringen termineras pga fel"
$ exit
$
