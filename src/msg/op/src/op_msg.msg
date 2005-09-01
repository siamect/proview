! 
!  Proview   $Id: op_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  op_msg.msg -- messages from operator environment
!
        .facility       PRO,230 /prefix = PRO__ ! Local Data Handler

success         <successful completion>                                 /succ
noconpic        <no picture connected to the  object>      		/info
claalrde        <class already defined>                                 /info
namenotfnd	<object/parameter name not found>			/warn
diginpparerr	<parameter error in a digital input button object>	/warn
illvarname	<illegal variable name>					/warn
cdntgetsubs	<subscription request failed>				/warn
invoppla        <invoke with opplace>                                   /error
retmainlo       <returned from mainloop>                                /error
noopplace       <couldnt fetch opplace from gdh>                        /error
noinst       	<no instance is current active>              		/error
nodefuser       <no defined user object connected to the opplace>       /error
gmserr          <GMS has signaled an error>                             /error
objnoava        <some error when objectpicture tried subscription>      /error
notintytab      <the object's code was not found in the type table>    	/error
notinapptab     <the picture's code was not found in the appl table>   	/error
converr		<error in converting data value>			/error
toofewvars	<a GMS object contained to few var refs>		/error
allocfail	<fail to allocate enough memory>			/fatal
error		<PROOPER error: !AF Reason:>				/error
log		<PROOPER log: !AF>					/info
notauthorized   <user is not authorized for this operation>             /error

