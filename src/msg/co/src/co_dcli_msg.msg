!  co_dcli_msg.msg -- class definition handler message
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  This module contains the defintion of the error messages used
!  in ccm modules.
!
.facility       DCLI,105 /prefix = DCLI__	

success		<successful completion>					/succ
nocommand	<no command found>					/error
com_ambig	<ambiguous command verb>				/error
com_nodef	<command not defined>					/error
qual_nodef	<qualifier not defined>					/error
qual_ambig	<ambiguous qualifier>					/error
qual_nofound	<qualifier not found>					/error
nofile		<unable to open file>					/error
nosymdefined	<no symbol is defined>					/error
nosymbol	<symbol not defined>					/error
symbol_ambig	<ambiguous symbol>					/error
symtblfull	<symbol table is full>					/error
symnametolong	<symbol name is to long>				/error
symnotfound	<symbol not found>					/error
symtype		<error in symbol type>					/error
syntax		<syntax error>						/error
symvaltolong	<symbol value to long>					/error
symdefined	<symbol defined>					/info
elemsyntax	<syntax error in array element>				/error
nomemory	<no dynamic memory available>				/error
strlong		<string is too long>					/error
endstructerr	<syntax error at end of struct>				/error
structsyntax	<syntax error in struct element>			/error
arraydim	<dimension of array not supported>			/error
structnotfound	<the struct was not found>				/error
eof		<end of file>						/error
undeferr	<undefined error>					/error
typeundef       <undefined type>                                        /error


