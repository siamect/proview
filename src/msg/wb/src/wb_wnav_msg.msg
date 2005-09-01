! 
!  Proview   $Id: wb_wnav_msg.msg,v 1.4 2005-09-01 14:58:00 claes Exp $
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

        .facility       WNAV,308 /prefix = WNAV__ ! 

success 	<successful completion>				/succ
holdcommand	<keep command mode>				/info
noelement	<element is missing>				/error
privattr	<attribute is private or virtual>		/error
eof		<end of file>					/error
nofile		<unable to open file>				/error
objectnotfound	<Object is not found>				/error
nocrossref	<No crossreferenses for this object>		/info
entryalreadyexist <Entry already exist>				/error
ismountobject	<Object is mountobject>				/error
nomatch		<No match in search expression>			/error
nosearch	<Search not started>				/error
searchcompile	<Can't compile search string>			/error
noedit		<Not in edit mode>				/error
flagnoedit	<Noedit attribute>				/error
flagstate	<State attribute>				/error
input_syntax	<Input syntax error>				/error
objnotfound	<Object not found>				/error
stringtolong	<String is to long>				/error
nochild		<Object has no children>			/info
waitconfirm	<Waiting for confirm>				/info
novolattached	<Not attatch to a volume>			/error 
disappeard	<Item has disappeard>				/error
noname		<No name to change on this object>		/error
volattached	<Volume is already attached>			/error
syntax		<Syntax error>					/error
qual		<Qualifier required>				/error
notsaved	<Session is not saved>				/error
nyi		<Not yet implemented in this environment>	/error
noobject	<Selected item is not an object>		/error
noselect	<No item is selected>				/error
notfound	<Item not found>				/error
nochildren	<Object has no children>			/error
cmdmode		<Session is in command mode>			/error
dbopen		<Database is already open>			/error
co_convert      <Error from co_convert>                         /error
seltomany	<Too many objects is selected>			/error
noplc		<Object is not a plc object>			/error




