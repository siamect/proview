!  rt_io_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  <Description>.
!
.facility       IO,140 /prefix = IO__		! I/O-system

success		<normal, successful completion>				/succ
succconv	<normal, successful completion>				/succ
errcreport	<creating port>						/error
errcrename	<creating portname>					/error
errcreproc	<creating communications processes>			/error
io_comm_exit	<exit due to fatal error in any subprocess>		/fatal
erraiini	<initialize AI-device>					/error
erraoini	<initialize AO-device>					/error
errdiini	<initialize DI-device>					/error
errdoini	<initialize DO-device>					/error
errcoini	<initialize CO-device>					/error
errgettime	<get systemtime>					/error
errairead	<reading AI-signals>					/error
erraowrite	<writeing AO-signals>					/error
gettypelist 	<get object of type '%s'>				/error
getlocalobj 	<No local object of type '%s'>				/error
getobjinfo 	<get object information '%s'>				/error
errinidevice 	<initialization device  '%s'>				/error
errcredb	<create Rtdb>						/error
errinitgdh	<initialization of gdh>					/error
errdevice 	<device type '%s' number %d>				/fatal
errsoftdev 	<softerror, devicetype '%s' number %d>			/info
errharddev 	<harderror, devicetype '%s' number %d>			/error
wroobjtype	<wrong object type>					/info
convai		<no polynomial conversion done for Ai>			/info
convao		<no polynomial conversion done for Ao>			/info
infoconvao	<conversion not ok, signal value out of range>		/info
infoconvout	<signal value = %d>					/info
infoconvdev	<DevPolyType = %d>					/info
infoconvsens	<SensorPolyType = %d>					/info
infosenstype	<SensorTypeCode = %d>					/info
badobjtype	<bad object type number specified>			/error
iniser		<initialization serial signals>				/error
sendrecser	<send and receive serial signals>			/error
abbsr_success	<normal, successful completion>				/succ
abbsr_nomess	<all telegram hasn't received>				/error
abbsr_timeout	<timeout>						/error
abbsr_bccerror	<checksum error in received telegram>			/error
!
errcremsg	<creating message>					/error
errsndmsg	<sending message>					/error
!errrcvmsg	<receive message>					/error
errefwdgobj	<accessing watchdog obj>				/error
cyctimebus	<when get cycle time bus, default is used>		/error
cyctimeserial	<when get cycle time serial, default is used>		/error
nodiocobjects   <no RTP_DIOC objects found on this node>		/info
inidiocerr      <initializing RTP_DIOC>					/error
nodiocancestor  <a RTP_DIOC is missing a valid parent object>		/error
diocprocerr     <starting DIOC subprocess>				/error
startdiocproc   <starting dioc subprocess>				/info
diocerrgetptr   <get pointer to LAT line object>			/fatal
dioclatcreerr   <create and map LAT port>				/fatal
dioccharseterr  <set characteristics on LAT port>			/fatal
latconerr       <connect LAT port '%s' to '%s' '%s', reject %d>		/fatal
alloc		<memory allocation failed>				/error
nochannels	<card has no valid channels>				/warning
cardaddress	<invalid card address>					/error
notipcarrier	<parent is not an IP Carrier>				/error
invipslot	<invalid IP slot>					/error
ipmuxcall	<IP-Mux call failed>					/error
ipmuxnotinit	<IP-Mux is not initialized>				/error
ipmuxnotinput	<no valid INMUX for given node and group>		/error
ipmuxnotoutput	<no valid OUTMUX for given node and group>		/error
buserror	<bus error when accessing the VME bus>			/error
sighandler	<failed to set up signal handler>			/error
ip_wrongmodule	<unexpected IP-module in the slot>			/error
boardid	        <invalid board identification>				/error
not_impl	<function not implemented>				/error
trv_next	<traverse next sibling, not child objects>		/info
nomethod	<method is not found>					/error
no_rwaction	<no read or write action found for this process>	/info
is_stopped	<IO is stopped>						/info
timeinvalid	<Invalid scantime>					/error
classexceed	<Max number of classes exceeded>			/error
chanrange	<Error in channel range attributes>			/error
.end
