!  rt_ph_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  <Description>.
!
	.facility	PH,191 /prefix = PH__		! Message handler

success		<normal, successful completion>				/succ
queempty	<queue is empty>					/warn
nosuchque	<no such queue is available >				/error
timeout		<timeout>						/warn
getmsg		<error get message>					/error
allocmsg 	<error allocating message area>				/error
	.end
