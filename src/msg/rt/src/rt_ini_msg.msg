! 
!  Proview   $Id: rt_ini_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  rt_ini_msg.msg -- <short description>
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
file		<unable to open file>					/error

.end
