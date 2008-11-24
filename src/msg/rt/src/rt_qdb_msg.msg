! 
!  Proview   $Id: rt_qdb_msg.msg,v 1.3 2008-11-24 15:21:49 claes Exp $
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
!  rt_qdb_msg.msg -- <short description>
!
.facility	QDB,18 /prefix = QDB__

success		<successful completion>					/succ
lockcheck	<database locking bugckeck>				/fatal
mutexinit	<failed to init mutex>					/error
dupladd		<item is already added>					/error
insvirmem	<insufficient virtual memory>				/error
alradd		<item is already added>					/info
linked		<buffer is linked>					/error
alrcre		<database is already created>				/error
alrmap		<database is already mapped>				/error
net_send	<error sending to network>				/error
quotaexceeded	<queue quota exceeded>					/error


.end
