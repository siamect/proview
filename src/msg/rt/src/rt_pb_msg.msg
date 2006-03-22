! 
!  Proview   $Id: rt_pb_msg.msg,v 1.3 2006-03-22 14:43:45 claes Exp $
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
!  rt_pb_msg.msg -- <short description>
!
.facility       PB,141 /prefix = PB__		! Profibus I/O

success		<normal, successful completion>				/succ
normal		<normal operating state>				/succ
initfail	<failed to initialize slave or module>			/error
disabled	<slave is disabled>					/warning
noconn		<no connection to slave>				/error
attrnoedit	<Attribute is no edit>					/error
noattrsel	<No attributes is selected>				/error
userprmdatalen  <Mismatch in UserPrmDataLen>				/error
moduleidx	<Module index out of range>				/error
modocc		<All modules are occupied>				/error
gsdfile		<Unable to open gsd file>				/error
nomodulename	<Module name is missing>				/error
duplmodulename	<Duplicate module name>					/error
nomoduleclass	<Module class is missing>				/error
.end


