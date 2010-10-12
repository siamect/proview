! 
!  Proview   $Id: wb_pwrb_msg.msg,v 1.4 2006-03-31 14:46:26 claes Exp $
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
!  wb_pwrb_msg.msg -- work bench base classes messages
!
!  This module contains the defintion of the error messages used
!  in pwrb modules.
!
.facility       PWRB,3 /prefix = PWRB__	! Base Meta Data

success		<successful completion>					/succ
chanclass	<channel class not allowed on this card>		/error
alochan		<could not allocate a new channel>			/error
graphaction     <no action defined in graph object>                     /error
inlibhier       <Object is in library hierarchy>                      	/info
nobuilt    	<Nothing is built>					/info
plcpgmchild    	<Object can not be a child to a PlcPgm>			/error


