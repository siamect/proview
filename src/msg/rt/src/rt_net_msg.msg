! 
!  Proview   $Id: rt_net_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  rt_net_msg.msg -- <short description>
!
.facility	NET,112 /prefix = NET__		! Net

success		<normal, successful completion>				/succ
crethread 	<creating thread>					/error
mutexinit 	<init mutex>						/error
nomessavail	<no messages are available> 				/warn
nosuchservice	<no such service exists>				/error
nosuchqcomtype	<no such Qcom message type exists>			/warn
nosuchqcomsubt  <no such Qcom message subtype exists>			/warn
qcomallocmsg 	<no Qcom message area is allocated>			/error
qcombindq 	<binding Qcom queue>					/error
qcomconappl 	<connecting application to Qcom>			/error
qcomconq 	<connecting to Qcom queue>				/error
qcomcreq 	<creating Qcom queue>					/error
qcomfreemsg 	<dispose received Qcom message>				/error
qcomgetmsg 	<get Qcom message>					/error
qcomgetqid 	<get Qcom identity>					/error
qcomputmsg 	<put Qcom message>					/error
qcomrcvmsg 	<receive Qcom message>					/error
qcomsendmsg 	<sending Qcom message>					/error
condinit 	<init condition variable>				/error
xdrfailed	<xdr failed>						/error
xdrunexpect	<xdr unexpected message type>				/error
nulldata	<null data, will not be sent>				/error

.end
