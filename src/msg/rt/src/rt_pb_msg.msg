! 
!  Proview   $Id: rt_pb_msg.msg,v 1.6 2007-06-01 13:46:29 claes Exp $
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

success		<Normal, successful completion>				/succ
normal		<Normal operating state>				/succ
initfail	<Initialization failed>					/error
disabled	<Disabled>						/warning
noconn		<No connection>						/error
attrnoedit	<Attribute is no edit>					/error
noattrsel	<No attributes is selected>				/error
userprmdatalen  <Mismatch in UserPrmDataLen>				/error
moduleidx	<Module index out of range>				/error
modocc		<All modules are occupied>				/error
gsdfile		<Unable to open gsd file>				/error
nomodulename	<Module name is missing>				/error
duplmodulename	<Duplicate module name>					/error
nomoduleclass	<Module class is missing>				/error
stopped		<Stopped state>						/warning
cleared		<Cleared state>						/warning
notinit		<Not initialized>					/warning
configerr	<Configuration error>					/error
masterlock	<Slave is locked by other master>			/error
notready	<Not ready>						/warning
gsdattr		<Gsdfile attibute is empty>				/error
nyi		<Not yet implemented>					/error
syntax		<Conversion syntax error>				/error
gsdmlfile	<Unable to open GSDML file>				/error
nodevice	<No device selected>					/error
createchan	<Unable to create channel>				/error
gsdmlfilemismatch <Gsdmlfile doesn't match original filename>		/error
.end


