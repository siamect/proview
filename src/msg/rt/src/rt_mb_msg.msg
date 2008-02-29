! 
!  Proview   $Id: rt_mb_msg.msg,v 1.1 2008-02-29 13:07:16 claes Exp $
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
!  rt_mb_msg.msg -- <short description>
!
.facility       MB,142 /prefix = MB__		! Modbus I/O

success		<Normal, successful completion>				/succ
normal		<Normal operating state>				/succ
initfail	<Initialization failed>					/error
disabled	<Disabled>						/warning
conndown	<Disabled>						/warning
noconn		<No connection>						/error
connlost	<Connection lost>					/error
.end


