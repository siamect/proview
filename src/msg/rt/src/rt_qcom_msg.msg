!  rt_qcom_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996,98 by Mandator AB.
!
!  <Description>.
!
.facility       QCOM,17 /prefix = QCOM__

success		<successful completion>					/succ
nyi		<not yet implemented>					/fatal
allocquota	<allocation quota exceeded>				/fatal
noforwardq	<not a forwarding queue>				/error
alrbound	<queue is already bound>				/error
noq		<queue does not exist>					/error
allrconnected	<already connected to Qcom>				/error
qallrexist	<queue does already exist>				/error
nolink		<there is no link to requested queue>			/error
linkdown	<the link is down to requested queue>			/error
notbound	<queue is not bound>					/error
weird		<something very weird has happened>			/error
bufovrun	<buffer was to small>					/error
invnid		<invalid node identifier>				/error
revlevel	<Qcom structure revision level mismatch>		/error
qempty		<queue is empty>					/error
notbindable	<queue is not bindable>					/error
notowned	<queue is not owned by this application>		/error
bound		<queue is bound>					/error
notalloc	<buffer not allocated from Qcom>			/error
nullbuff	<null buffer>						/error
qtype		<invalid queue type>					/error
inited		<already inited>					/error
badarg		<bad argument>						/error
nosuchnode	<no such node exists>					/error
no_node		<no more nodes>						/error
tmo		<time out>						/error
qnotlocal	<queue is not local>					/error
noexport	<no export defined for this system>			/error
hightmo		<time out time to high>					/error
notinited	<Qcom not inited for application>			/error
allrattached	<queue is already attached>				/error
notattachable	<queue can not be attached>				/error

.end
