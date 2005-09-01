! 
!  Proview   $Id: rt_ndc_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  rt_ndc_msg.msg -- Network data conversion
!
!  This module contains the defintion of the error messages used
!  in ndc modules.
!
.facility       NDC,101 /prefix = NDC__	

success		<successful completion>					/succ
offset		<invalid offset>					/error
convert		<error in conversion>					/error
op		<invalid operation code>				/error
nrattribute	<attribute doesn't exist on remote node>		/error
rnattribute	<remote attribute doesn't exist on this node>		/error
nrelem_idx	<element index doesn't exist on remote node>		/error
rnelem_idx	<remote element index doesn't exist on this node>	/error
noconv		<there is no valid conversion for attribute>		/error
bufovrun	<buffer was to small>					/error
