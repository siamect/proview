! 
!  Proview   $Id: wb_pwrs_msg.msg,v 1.3 2005-09-01 14:58:00 claes Exp $
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
!  wb_pwrs_msg.msg -- errlog messages for system methods.
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
posclassdef	<a ClassDef should be child to a ClassHier object>	/error
posclasshier	<a ClassHier should be a root object>			/error
posattribute	<an attribute or buffer object should be child to a ObjBodyDef object> /error
posobjbodydef	<a body object should be child to a ClassDef object> /error
moveattribute	<an attribute object can only be moved within the same childhood> /error
movebody	<a body object can only be moved within the same childhood> /error




