! 
!  Proview   $Id: rt_csup_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  rt_csup_msg.msg -- <short description>
!

.facility	CSUP,211 /prefix = CSUP__		! Cycle supervision

errcrenam	<create port name '%s'>					/error
errefobj	<reference object '%s'>					/error
errgetnodobj	<access node config object>				/error
errgetwdgobj	<access watchdog config object>				/error
errgettyp	<get first cycle supervision object>			/error
erringettim	<get system time, cannot check for delay>		/error
errinmhcall	<returned from MH-call>					/error
errtrnnam	<translate port name '%s'>				/error
errwait		<executing wait statement>				/error
illname		<watchdog name, failed to create/map object>		/error
namalrexist	<create port name '%s', name already exists>		/warning
nolimtochk	<no limit to check>					/error
nostartim	<time of next loop unknown, unable to check for delay>	/error
nottowatch	<cycle time/port absent, object deleted '%s'>		/error
nulpoint	<null pointer, can't use watchdog object>		/error
success		<normal, successful completion>				/succ
linkerror	<link error '%s'>					/error
linklog		<link log '%s'>						/info
insvirmem	<insufficient virtual memory>				/error

.end
