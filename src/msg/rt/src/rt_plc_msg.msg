!  rt_plc_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996,98 by Mandator AB.
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

.end
