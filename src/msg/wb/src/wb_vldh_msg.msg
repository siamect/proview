! 
!  Proview   $Id: wb_vldh_msg.msg,v 1.3 2005-09-01 14:58:00 claes Exp $
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
!  wb_vldh_msg.msg -- very local datahandler
!
        .facility       VLDH,260 /prefix = VLDH__ ! Gre data handler

success		<successful completion>					/succ
nonodes		<no nodes found>					/info
nocons		<no connections found>					/info
nowinds		<no windows found>					/info
badobjtype	<object type out of range>				/error
badconpoint	<connection point out of range>				/error
objnotfound	<object not found>					/error
plcnotsaved	<plcprogram object is not saved>			/error
parentnotsaved	<parent node is  not saved>				/error
windcorrupt	<Window is corrupt>					/error


	.end
