!  wb_pwrs_msg.msg -- errlog messages for system methods.
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  This module contains the defintion of the error messages used
!  in pwrs modules.
!
.facility       PWRS,2 /prefix = PWRS__	! System Meta Data

success		<successful completion>					/succ
alrconn		<objects are already connected to each other>		/warning
palrconn	<object pointed at has no free connection>		/error
salrconn	<selected object has no free connection>		/error
perrconn	<object pointed at is incorrectly connected>		/error
serrconn	<selected object is incorrectly connected>		/error
notconn		<objects are not connected>				/error
pnotconn	<object pointed at is not connected with selected object> /error
snotconn	<selected object is not connected with object pointed at> /error
pwasconn	<object pointed at was connected with selected object>	/info
swasconn	<selected object was connected with object pointed at>	/info
pwerrconn	<object pointed at was incorrectly connected with selected object>	/warning
swerrconn	<selected object was incorrectly connected with object pointed at>	/warning
xreffull	<all connections are used>				/error
idxref		<inconnectable cross reference identities>		/error
stxref		<inconnectable source, target pair>			/error
