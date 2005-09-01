! 
!  Proview   $Id: rt_utl_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  rt_utl_msg.msg -- <short description>
!
.facility       UTL,12 /prefix = UTL__	! Utility packages

success		<successful completion>					/succ
noslot		<pool directory full>					/fatal
badseg		<invalid pool reference>				/fatal
segsize		<invalid pool reference>				/fatal
nomark		<invalid pool reference>				/fatal
remapnyi	<remap needed, not implemented>				/fatal
bugcheck	<internal consistency error>				/fatal
malloc		<malloc error>						/fatal

.end
