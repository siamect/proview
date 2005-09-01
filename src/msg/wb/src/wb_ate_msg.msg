! 
!  Proview   $Id: wb_ate_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  wb_ate_msg.msg -- attribute editor messages
!
!  This module contains the defintion of the error messages used
!  in ate modules.
!
.facility       ATE,300 /prefix = ATE__	! Attribute editor

success		<successful completion>					/succ
converr		<error in converting string to value>			/error
internerr	<internal error>					/fatal
outofmemory	<out of memory>						/fatal
couldntalloc	<XtCalloc failed to allocate more memory>		/fatal

.end
