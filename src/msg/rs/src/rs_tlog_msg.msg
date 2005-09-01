! 
!  Proview   $Id: rs_tlog_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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

        .facility       TLOG,311 /prefix = TLOG__ ! 

success 	<successful completion>				/succ
nomemory	<nomemory>					/error
confobjnotfound	<tlog configure object not found>		/error
userobjnotfound	<tlog user object not found>			/error
maxmodify	<maximum amount of modify levels reached>	/error
notsel		<object not in selectlist>			/error
fileopen	<unable to open file '%s'>			/error
difftext	<difference in text>				/error
diffattr	<difference in attrbute>			/error
difftime	<difference in time>				/error
listend		<end of list>					/info
timesyntax	<syntax error in time>				/error
time_gt		<time greater than>				/info
time_lt		<time less than>				/info
time_eq		<time equal>					/info
beforequal	<syntax error in before time>			/error
sincequal	<syntax error in since time>			/error
