! 
!  Proview   $Id: rt_qcom_msg.msg,v 1.3 2005-09-01 14:58:00 claes Exp $
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
!  rt_qcom_msg.msg -- <short description>
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
down		<Link down>						/warning
.end




