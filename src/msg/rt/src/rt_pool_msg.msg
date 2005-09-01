! 
!  Proview   $Id: rt_pool_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  rt_pool_msg.msg -- <short description>
!
.facility	POOL,13 /prefix = POOL__	! Pool package

success		<successful completion>					/succ
badoffs		<invalid pool reference>				/fatal
noslot		<pool directory full>					/fatal
badseg		<invalid pool reference>				/fatal
segsize		<invalid pool reference>				/fatal
nomark		<invalid pool reference>				/fatal
remapnyi	<remap needed, not implemented>				/fatal
bugcheck	<internal consistency error>				/fatal
insvirmem	<insufficient virtual memory>				/fatal
nulladdr	<a null address was passed>				/fatal
nullref		<a null reference was passed>				/fatal
incon		<pool inconsistency>					/fatal
toobig		<requested segment is too big>				/error
allrexist	<pool segment allready exist>				/error
allrmapped	<pool is allready mapped>				/error
nolist		<no free lookaside list>				/error
freenamed       <named segment cannot be freed>				/error
notcreated	<pool is not yet created>				/error

.end
