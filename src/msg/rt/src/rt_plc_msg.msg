! 
!  Proview   $Id: rt_plc_msg.msg,v 1.3 2005-09-01 14:58:00 claes Exp $
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
!  rt_plc_msg.msg -- <short description>
!
!  This module contains the defintion of the error messages used
!  in the plc subsystem
!
.facility       PLC,130 /prefix = PLC__		! PLC-fuctions

success		<normal, successful completion>				/succ
errunload	<unload PLC program>					/error
wrongobjtype	<rtdb object type not a copy type %d>			/error
directlink	<obtain pointer to object '%s'>				/error
errcreobj	<create object '%s'>					/error
errefobj	<create direct link to object '%s'>			/error
errefwdgobj	<access watchdog object for subprocess cycle time %d>	/error
errinitio	<initialize I/O, no I/O performed>			/error
errwait		<wait in PLC loop, process %d>				/error
errwaitterm	<wait for PLC subproc %d to terminate>			/error
errwaitinit	<wait for subprocess %d to initalize>			/error
errunref	<deaccess direct linked object '%s'>			/error
errcrearea	<create PLC global area>				/error
errincrearea	<create or map sync area>				/error
errcreport	<create termination port>				/error
errcrename	<create termination port name>				/error
errtrnname	<translate termination port name>			/error
errgetobjsize	<get rtdb object size for '%s'>				/error
errcreproc	<create PLC subprocess %d>				/error
errcremsg	<create termination message>				/error
errsndmsg	<send termination message>				/error
errreaddi	<read Di, IO shut off>					/error
errwritedo	<write Do, IO shut off>					/error
errreadco	<read Co, IO shut off>					/error
errscandsuplst	<scan DSup list, IO shut off>				/error
errascef	<associate event flag cluster in '%s'>			/error
errgetef	<get event flag for PLC timer>				/fatal
nonodeobj       <no node objects exists>				/error
ininstep	<initialize init steps>					/error
iostalled	<IO stalled, cyclesup delay detected>			/fatal
ioread		<IO stalled, io read error>				/fatal
iowrite		<IO stalled, io write error>				/fatal
.end
