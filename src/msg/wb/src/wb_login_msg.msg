! 
!  Proview   $Id: wb_login_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  wb_login_msg.msg -- <short description>
!

        .facility       LOGIN,320 /prefix = LOGIN__ ! Login

success		<successful completion>					/succ
usernotau	<user not authorized>					/error
userexist	<user already exixts>					/error
nomemory	<no dynamic memory>					/error
nouserlist	<userlist not opened>					/error
nouser		<no such user>						/error
userfile	<unable to oper userlist data file>			/error
nosave		<nothing written in user data file>			/info
nopriv		<no privilege for attempted operation>			/error
authfail	<user authorization failure>				/error
