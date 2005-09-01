! 
!  Proview   $Id: rt_pwr_msg.msg,v 1.3 2005-09-01 14:58:00 claes Exp $
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
!  rt_pwr_msg.msg -- <short description>
!
! NOTE !!! Put new message last, msg numbers are hardcoded in Errh.java
!
.facility	PWR,4 /prefix = PWR__		! Proview messages

success		<Successful completion>					/succ
startup		<Proview starting up>					/info
running		<Proview running>					/succ
swap            <Proview soft restart>					/warning
nettimeout      <Network timeout>					/fatal
srverror	<Server reported error>					/error
srvfatal	<Server reported fatal error>				/fatal
srvwarning	<Server reported warning>				/warning
srvinfo		<Server reported info>					/info
srvstartup      <Server starting up>					/warning
srun		<Server running>					/info
srvrestart	<Server restarting>					/warning
srvterm		<Server terminated>					/fatal
applerror	<Application reported error>				/error
applfatal	<Application reported fatal error>			/fatal
applwarning	<Application reported warning>				/warning
applinfo	<Application reported info>				/info
applstartup     <Application starting up>				/warning
arun		<Application running>					/info
applrestart	<Application restarting>				/info
applterm	<Application terminated>				/fatal
ptimeout	<Process timeout>					/fatal

.end

