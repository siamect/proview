!  wb_cmd_msg.msg -- command file processing
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  <Description>.

.facility       CMD,301 /prefix = CMD__ ! 

success 	<successful completion>				/succ
com_ambig	<ambigous command>				/error
com_nodef	<command not defined>				/error
qual_nodef	<qualifier not defined>				/error
qual_ambig	<ambigous qualifier>				/error
qual_nofound	<qualifier not found>				/error
nocommand	<no command given>				/info
nomemory	<unable to allocate dynamic memory>		/error
nofile		<unable to open file>				/error
eoc		<end of command array>				/error
interact	<not supported in interactive mode>		/error
symtblfull	<symbol table is full>				/error
symlong		<symbol name is to long>			/error
symbol		<symbol not found>				/error
symtype		<error in symbol type>				/error
objnotfound	<object not found>				/error
syntax		<syntax error>					/error
symval		<symbol value is to long>			/error
symdef		<symbol is defined>				/info
label		<label name is missing>				/error
labelstx	<label syntax error>				/error
nolabel		<label is not found>				/error
endwhile	<error in while-endwhile structure>		/error
ifendif		<error in if-endif structure>			/error
ifelse		<error in if-else-endif structure>		/error
filalron	<file is already on>				/error
name		<name is missing>				/error
cmdarg		<to many arguments in commandfile call>		/error
novolattached	<no volume is attached>				/error
nowbattached	<no database is attached>			/error
.end
