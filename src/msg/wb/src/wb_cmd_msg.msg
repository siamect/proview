! 
!  Proview   $Id: wb_cmd_msg.msg,v 1.3 2006-02-23 14:49:00 claes Exp $
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
!  wb_cmd_msg.msg -- command file processing

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
attribute	<attribute is missing>				/error
cmdarg		<to many arguments in commandfile call>		/error
novolattached	<no volume is attached>				/error
nowbattached	<no database is attached>			/error
.end
