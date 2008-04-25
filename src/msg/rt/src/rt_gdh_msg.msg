! 
!  Proview   $Id: rt_gdh_msg.msg,v 1.6 2008-04-25 11:09:10 claes Exp $
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
!  rt_gdh_msg.msg -- <short description>
!
.facility       GDH,110 /prefix = GDH__		! Global Data Handler

success		<successful completion>					/succ
nyi		<not yet implemented>					/fatal
badname		<bad object name>					/error
badobjtype	<bad object type number specified>			/error
no_parent	<no parent object exists>				/error
no_child	<no child object exists>				/error
no_sibling	<no more siblings exist>				/error
no_type		<no more objects of same type exists>			/error
remote		<referenced object is remotely stored>			/error
nosuchobj	<no such object exists>					/error
namebuf		<name buffer is too small>				/error
nodidx		<illegal node index>					/error
nosuchnode	<no such node>						/error
weird		<something is very weird in the database>		/fatal
noderunning	<node is already running>				/error
rtdbnull	<invalid rtdb pointer>					/error
nosubcli	<no such subscription>					/error
nonethandler	<no nethandler is running>				/error
children	<this object has got children>				/error
nobody		<object has no body>					/warn
badparent	<parent object remote or wrong type>			/error
reqobjid	<objid required>					/error
duplname	<duplicate name>					/error
ohash		<cannot map OHASH table>				/fatal
gdhpar		<missing or invalid GDH setup data>			/error
pni		<cannot map PNI table>					/fatal
size		<reference outsize object body>				/error
objtype		<object membertype prevents operation>			/error
childself	<object cannot be its own child>			/error
invohref	<invalid OH reference>					/warn
dlid		<bad dlid, no such direct link>				/error
subold		<subscription data is old>				/warn
subalrbuf	<subscription user buffer already associated>		/error
argcount	<wrong number of arguments>				/error
qcom		<Qcom is not correctly started>				/fatal
subscript	<attribute is not array or subscript out of range>	/error
attribute	<no such attribute>					/error
revlevel	<gdh structure revision level mismatch>			/error
aliascirc	<circluar alias reference>				/error
aliasmode	<creating alias, must be exclusive mode>		/error
aliasnref	<creating alias, reference is NOBJID>			/error
notmounted	<volume not mounted>					/error
nosuchvol	<no such volume exists>					/error
nidtab		<cannot map NodeId table>				/fatal
vidtab		<cannot map VolumeId table>				/fatal
staticvol	<function can not be done in static volume>		/error
samevolume	<function can only be done within one volume>		/error
badarg		<bad argument was used in fuction call>			/error
insvirmem	<insufficient virtual memory>				/error
notmovable	<object cannot be moved>				/error
namelen		<name is too long>					/error
obsolete	<function is obsolete>					/error
nosuchclass	<no such class exists>					/error
notowned	<object is not owned by this node>			/error
voldelete	<a volume cannot be deleted>				/error
rtnotstarted	<Proview runtime is not started>			/error
nocclasssup	<Cached Classes not supported on other node>		/error
nomountobject	<Mount object is missing>				/error
noattr          <AttrRef is not an attribute>				/error
noobject        <AttrRef is not an object>				/error
file		<Unable to open file>					/error
convert		<Unable to convert value>				/error
.end


