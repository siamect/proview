!  rt_mh_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996,98 by Mandator AB.
!
!  <Description>.
!
.facility	MH,190 /prefix = MH__		! Event logger

success		<normal, successful completion>				/succ
nomessavail	<no messages are available> 				/warn
nosuchqcomtype	<no such Qcom message type exists>			/warn
nosuchqcomsubt  <no such Qcom message subtype exists>			/warn
qcomallocmsg 	<no Qcom message area is allocated>			/error
qcombindq 	<binding Qcom queue>					/error
qcomconappl 	<connecting application to Qcom>			/error
qcomconq 	<connecting to Qcom queue>				/error
qcomcreq 	<creating Qcom queue>					/error
qcomfreemsg 	<dispose received Qcom message>				/error
qcomgetmsg 	<get Qcom message>					/error
qcomgetqid 	<get Qcom Id>						/error
qcomputmsg 	<put Qcom message>					/error
qcomrcvmsg 	<receive Qcom message>					/error
qcomsendmsg 	<send Qcom message>					/error
freemsg 	<dispose message>	 				/error
nomesssend 	<no message is send>					/warn
errmh 	 	<error in mh>	 					/error
errinit 	<initialize mh>						/error
getlocalobj 	<no local object of type '%s'>				/info
nosuchdesc	<no such descriptor name exists>			/error
errcredb	<create Rtdb in '%s'>					/error
errgdhini	<initialize GDH in '%s'>				/error
errgetlist	<get type list '%s'>					/error
errgetpointer	<get pointer to object of type '%s'>			/error
errgetobjid	<get objid to object of type '%s'>			/error
errgetnext	<get next object of type '%s'>				/error
errcremess	<create message for '%s'>				/error
errsendmess	<send message for '%s'>					/error
errrecmess	<receive message>					/error
errwaitany	<wait any>						/error
errdelmess	<delete message>					/error
errascef	<associating event flag cluster, in '%s'>		/error
alarmlstfull	<alarm list is full, max number of alarms/appl= %d>	/error
errlstlocate	<search for '%s' in '%s'>				/error
allrcon		<already connected to event logger>			/error
notconn		<already connected to event logger>			/info
nooutunit	<this is no Outunit>					/error
error		<%s>							/error
log		<%s>							/info
nospace		<no dynamic space available>				/error
applquota	<number of application alarms has exceeded its quota>	/error
badparam	<bad parameter value>					/error
noappl		<this is no Appl object>				/error
eventerr	<unexpected event>					/error
progerr		<programming error in mh>				/error
nyi		<not yet implemented>					/error
nonexalarm	<a non existing alarm>					/error
allret		<alarm already returned>				/info
notowned	<alarm not owned by application>			/error
version		<wrong version of MH-protocoll>				/error
systemid	<different SystemIds>					/error
nomoremsg	<no more messages owned by application>			/info
nosuchobj	<no such object exists>					/error
handlerdown	<link to event logger is down>				/error
xdrfailed	<xdr failed>						/error
xdrunexpect	<xdr unexpected message type>				/error
tmo		<timeout>						/error
notmntclean 	<object is not mount clean>				/warn
iostalled	<IO stalled, cyclesup delay detected>			/fatal

.end
