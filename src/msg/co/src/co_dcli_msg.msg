! 
!  Proview   $Id: co_dcli_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
!  Copyright (C) 2005 SSAB Oxelösund AB.
!
!  This program is free software; you can redistribute it and/or 
!  modify it under the terms of the GNU General Public License as 
!  published by the Free Software Foundation, either version 2 of 
!  the License, or (at your option) any later version.
!
!  This program is distributed in the hope that it will be useful 
!  but WITHOUT ANY WARRANTY; without even the implied warranty of 
!  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
!  GNU General Public License for more details.
!
!  You should have received a copy of the GNU General Public License 
!  along with the program, if not, write to the Free Software 
!  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
!
!  co_dcli_msg.msg -- class definition handler message
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


