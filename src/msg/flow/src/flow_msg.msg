! 
!  Proview   $Id: flow_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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

        .facility       FLOW,305 /prefix = FLOW__ ! 

success 	<successful completion>				/succ
noelem		<No such element> 				/error
nonext		<There is no next element>			/error
noprevious	<There is no previous element>			/error
noparent	<There is no parent>				/error
noconpoint	<There is no such conpoint>			/error
nochild		<There is no child>				/error
nonextsibling	<There is no next sibling>			/error
destroyed       <Flow ctx is destoryed>                         /info
no_propagate    <Don't propagate event handling>                /info
fileopen        <Unable to open file>                           /error