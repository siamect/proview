! 
!  Proview   $Id: co_user_msg.msg,v 1.3 2005-09-01 14:58:00 claes Exp $
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
!  co_user_msg.msg -- class definition handler messages
!
!  This module contains the defintion of the error messages used
!  in user modules.
!
.facility       USER,104 /prefix = USER__	

success		<successful completion>					/succ
invsysname	<invalid system name>					/error
nosuchsystem	<no such system>					/error
nosuchuser	<no such user>						/error
notauthorized	<user is not authorized>				/error
useralrexist	<user already exist>					/error
systemalrexist	<system already exist>					/error
fileopen	<unable to open user database>				/error
systemnotempty	<system is not empty>					/error
