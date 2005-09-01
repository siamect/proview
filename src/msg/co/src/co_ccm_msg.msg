! 
!  Proview   $Id: co_ccm_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  co_ccm_msg.msg -- class definition handler messages
!
!  This module contains the defintion of the error messages used
!  in ccm modules.
!
.facility       CCM,103 /prefix = CCM__	

success		<successful completion>					/succ
exitfunc	<exit function called>					/info
expression	<syntax error in expression>				/error
vartype		<variable type mismatch>				/error
varalrexist	<variable already declared>				/error
varnotfound	<variable not found>					/error
openfile	<unable to open file>					/error
syntax		<syntax error>						/error
funcnotfound	<function not declared>					/error
unknvartype	<unknown varable type>					/error
funcmism	<function-endfunction mismatch>				/error
mainmism	<main-endmain mismatch>					/error
ifmism		<if-else-endif mismatch>				/error
whilemism	<while-endwhile mismatch>				/error
formism		<for-endfor mismatch>					/error
argmism		<argument mismatch>					/error
contmism	<continue-for/while mismatch>				/error
breakmism	<break-if/for/while mismatch>				/error
noextvar	<no external variables defined>				/error
gotomism	<label in goto expression not found mismatch>		/error
sysfuncexceed	<max number of function exceeded>			/error
externfunc	<external function found>				/info
longline	<maximum line size exceeded>				/error
stringexceed	<maximum size of string exceeded>			/error
argexceed	<max number of arguments exceeded>			/error
arraybounds	<array index out of bounds>				/error
alreadyreg      <function is already registred>                         /info






