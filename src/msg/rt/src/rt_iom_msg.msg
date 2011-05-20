! 
!  Proview   $Id$
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
!  rt_iom_msg.msg -- <short description>
!
.facility       IOM,143 /prefix = IOM__		! IO methods

success		<Normal, successful completion>				/succ
udp_up		<UDP link is up>					/succ
udp_init	<UDP initalizing>					/info
udp_down	<UDP link is down>					/error
udp_socket	<UDP create socket error>				/error
udp_bind	<UDP bind socket error>					/error
udp_remote	<UDP unknown remote host>				/error
.end


