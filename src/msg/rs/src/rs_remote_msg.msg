
        .facility       REM,531 /prefix = REM__ ! 

success 	<normal successful completion>			/succ
noremnode	<no remote node under node object>		/error
noremtrans	<no corresponding remtrans object found, '%s'>	/error
nomemory	<unable to allocate dynamic memory>		/error
novaluebase     <'%s' value base not present >			/error
nochannel	<'%s' has no channels as children>		/info
ignored		<'%s' is ignored>				/info
nonodeinfo	<no valid node info>				/error
trpstarterr	<'%s' couldn't be started>			/error
trpstarted	<'%s' started>					/info
trpdied		<Transport '%s' died. Restarts '%d'>		/warning
trpexh		<Transport '%s' died. No more restarts allowed>	/error
numitemdiff	<Number of '%s' differs for node '%s'>		/warning
numremnodediff	<Number of remnodes differs for trptype '%d'>	/warning
unknownsource	<Message from unknown source received>		/info
linkdown	<Remio link down for node '%s'>			/info
linkup		<Remio link up for node '%s'>			/info
weird		<Unknown message, class '%d', type '%d'>	/warning
overrun		<Message coming too soon, '%s'>			/warning
multnodes	<Multiple nodes not allowed for this transport>	/error
sndfail		<Transport process for '%s' failed in sending> 	/warning
sndfail3964r	<Transport process for 3964r failed in sending> /warning
rcvfail		<Transport process for '%s' failed in receive> 	/warning
rcvfail3964r	<Transport process for 3964r failed in receive> /warning
siemenserror	<Response message error code '%lx'>		/warning
tcplinkdown	<Remote TCP/IP link down '%s'>			/info
tcplinkup	<Remote TCP/IP link up '%s'>			/info
loggexit	<Remlogg has exited, reason: >			/fatal
loggfileopen	<Remlogg file open '%s'>			/info
loggiden	<Unknown remlogg identity received>		/error
loggwrite	<Remlogg write error>				/error
loggrcv		<Remlogg receive error>				/error
loggfile	<Unable to open loggfile '%s'>			/error
timeout		<Remote timeout>				/info
confinit	<Init error of LoggConfig object, Reason:> 	/error
duplident	<Identity is not unique>			/error
rcvsucc		<Transport process for '%s' success in receive> /info
rcvsucc3964r    <Transport process for 3964r success in receive> /info
crcerror        <Transport process for '%s' checksum error '%lx' '%lx'>      /warning
udplinkdown     <Remote UDP/IP link down '%s'>                     /info
udplinkup       <Remote UDP/IP link up '%s'>                     /info
udpunknown      <UDP message from unknown ip-address '%s' received>      /info
udpweirdheader  <UDP message with illegal header received>      /info
