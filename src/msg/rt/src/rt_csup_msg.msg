!  rt_csup_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 2000 by Wiromania.
!
!  <Description>.
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
