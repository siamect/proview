!  rt_errh_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996,98 by Mandator AB.
!
!  <Description>.
!
.facility	ERRH,1	/prefix = ERRH__		! Error logger

recerr		<receive log message>					/error
badsize		<bad size of received log message -- ignored>		/warn
badfile		<cannot open log file '%s'>				/warn
newfile		<log file '%s' is now active>				/info
badterm		<cannot open log terminal '%s'>				/warn
newterm		<log terminal '%s' is now active>			/info
oldterm		<log terminal '%s' is now inactive>			/info
exception	<exception signalled>					/warn
closing		<process closing down>					/info

.end
