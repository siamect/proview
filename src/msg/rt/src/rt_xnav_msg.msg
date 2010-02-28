! 
!  Proview   $Id: rt_xnav_msg.msg,v 1.6 2006-03-31 14:46:26 claes Exp $
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

        .facility       XNAV,307 /prefix = XNAV__ ! 

success 	<successful completion>				/succ
holdcommand	<keep command mode>				/info
noelement	<element is missing>				/error
privattr	<attribute is private or virtual>		/error
eof		<end of file>					/error
nofile		<unable to open file>				/error
objectnotfound	<Object is not found>				/error
nocrossref	<No crossreferenses for this object>		/info
entryalreadyexist <Entry already exist>				/error
nouser		<Invalid user object>				/error
notconnected	<Not connected to jop process>			/error
nochild		<Object has no children>			/info
nomemory        <No virtual memory>                             /error
nochange        <Attribute can't be changed>                    /error
noselect        <Nothing is selected>                           /error
ismountobject	<Object is mountobject>				/error
nomatch		<No match in search expression>			/error
nosearch	<Search not started>				/error
searchcompile	<Can't compile search string>			/error
syntax          <Syntax error>                                  /error
trendconfig	<Error in trend configuration>			/error
nomethod	<Method not found>				/error
invisible	<Menu button invisible>				/error
insensitive	<Menu button not sensitive>			/info
notraceattr     <No trace attribute for this class>             /error
input_syntax	<Input syntax>					/error
objnotfound	<Object is not found>				/error
stringtolong	<String is too long>				/error
fastconfig	<Error in fast configuration>			/error
nochildren	<Item has no children>				/error
queued		<Request queued>				/info
queuefull	<Queue is full>					/error
waveformat	<Unsupported wavefile format>			/error
filecorrupt	<File is corrupt>				/error
notcollectw	<Not collect window>				/error
noclassgraph	<No classgraph found>				/error


