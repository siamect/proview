!  rt_pwr_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996,98 by Mandator AB.
!
!  <Description>.
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

