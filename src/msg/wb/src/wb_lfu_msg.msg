! 
!  Proview   $Id: wb_lfu_msg.msg,v 1.3 2005-09-01 14:58:00 claes Exp $
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

        .facility       LFU,304 /prefix = LFU__ ! 

success 	<successful completion>				/succ
nobootfile	<filename does not match bootfile syntax>	/error
vollist		<volume list syntax error>			/error
nofile		<unable to open file>				/error
plcversion	<plc version file corrupt>			/error
syntax		<syntax error> 					/error
nodename	<node does not exist>				/error
filecrp		<file is corrupt>				/error
noloadfile	<file is no load file>				/error