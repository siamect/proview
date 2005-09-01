$! 
$! Proview   $Id: pwr_repl.com,v 1.2 2005-09-01 14:57:49 claes Exp $
$! Copyright (C) 2005 SSAB Oxelösund AB.
$!
$! This program is free software; you can redistribute it and/or 
$! modify it under the terms of the GNU General Public License as 
$! published by the Free Software Foundation, either version 2 of 
$! the License, or (at your option) any later version.
$!
$! This program is distributed in the hope that it will be useful 
$! but WITHOUT ANY WARRANTY; without even the implied warranty of 
$! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
$! GNU General Public License for more details.
$!
$! You should have received a copy of the GNU General Public License 
$! along with the program, if not, write to the Free Software 
$! Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
$!
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
