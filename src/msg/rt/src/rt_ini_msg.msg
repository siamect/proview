!  rt_ini_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996,98 by Mandator AB.
!
!  <Description>.
!
.facility	INI,180 /prefix = INI__		! System initialization

success		<normal, successful completion>				/succ
error		<nonrecoverable error found>				/error
headversion	<loadfile header version mismatch>			/error
sectversion	<loadfile section version mismatch>			/error
badsect		<loadfile section type unknown>				/info
multsect	<loadfile contains multiple sections of same type>	/error
sectorder	<bad loadfile section order>				/error
load		<loading '%s' program '%s'>				/info
start		<starting program as '%s'>				/info
errinit 	<error initializing I/O db> 				/error
gettypelist	<error getting typelist for '%s'>			/error
getlocalobj	<error finding local object of type '%s'>		/error
errinipar 	<error initializing I/O database for '%s'>		/error
plcchgini	<change of PLC program initiated at '%s'>		/info
plcchgcmp	<change of PLC program completed, I/O stalled '%s'>	/info
plcsavactval	<save actual value failed, object class '%s'>		/error
nodidx		<cannot determine node index in ld_boot file>		/error
xdr		<error when marshalling>				/error
insvirmem	<insufficient virtual memory>				/error

.end
