!  co_ccm_msg.msg -- class definition handler messages
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
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






