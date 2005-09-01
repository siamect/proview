$! 
$! Proview   $Id: pwr_gm1.com,v 1.2 2005-09-01 14:57:49 claes Exp $
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
$! pwr_gm1.com -- <short description>
$!
$!
$!
$!
$!	p1	target
$!	p2	source
$!
$	on warning then goto error
$	default = f$environment("default")
$	say = "write sys$output"
$	status = 1
$
$	say "pwr_gm1 ''p1' <- ''p2'"
$	target = f$parse(p1,,,"device","syntax_only") -
		+ f$parse(p1,,,"directory","syntax_only")
$	source = f$parse(p2,,,"device","syntax_only") -
		+ f$parse(p2,,,"directory","syntax_only")
$	target_file = f$parse(p1,,,"name","syntax_only") -
		+ f$parse(p2,,,"type","syntax_only")
$	target_name = f$parse(p1,,,"name","syntax_only")
$	source_name = f$parse(p2,,,"name","syntax_only")
$	set def 'target
$	if target_name .nes. source_name
$	then
$	  model_lang = f$parse(p2,,,"name","syntax_only")
$	  model_lang = f$edit(model_lang,"lowercase") + ": model"
$	  model = f$parse(p1,,,"name","syntax_only")
$	  model = f$edit(model,"lowercase") + ": model"
$	  open/write edt tmp.tmp
$	  write edt "type ""''model_lang'"""
$	  write edt "s/''model_lang'/''model'/"
$	  write edt "wr ''target_file'"
$	  write edt "quit"
$	  close edt
$	  define/user sys$output nl:
$	  edit/edt/command=tmp.tmp 'p2
$	  delete/nolog tmp.tmp;*
$	else
$	  copy 'p2' 'target_file'
$	endif
$	@gms$home:[bin]gm1 'target_file
$	status = $status
$	delete/nolog 'target_file';*
$	goto exit
$error:
$	status = $status
$	goto exit
$exit:
$	set def 'default	
$	exit status
