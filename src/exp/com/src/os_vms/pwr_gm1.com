$! pwr_gm1.com -- <short description>
$!
$! PROVIEW/R
$! Copyright (C) 1996 by Comator Process AB.
$!
$! <Description>.  */
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
