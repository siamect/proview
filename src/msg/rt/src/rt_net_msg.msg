!  rt_net_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  <Description>.
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
