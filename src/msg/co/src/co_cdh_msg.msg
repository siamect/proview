! 
!  Proview   $Id: co_cdh_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  co_cdh_msg.msg -- class definition handler messages
!
!  This module contains the defintion of the error messages used
!  in cdh modules.
!
.facility       CDH,100 /prefix = CDH__	

success		<successful completion>					/succ
invdeltatime	<invalid delta time format>				/error
invtime		<invalid time format>					/error
invtype		<invalid type>						/error
invbool		<invalid boolean format>				/error
invfloat32	<invalid float 32-bit format>				/error
invfloat64	<invalid float 64-bit format>				/error
invint8		<invalid integer 8-bit format>				/error
invint16	<invalid integer 16-bit format>				/error
invint32	<invalid integer 32-bit format>				/error
invuint8	<invalid unsigned integer 8-bit format>			/error
invuint16	<invalid unsigned integer 16-bit format>		/error
invuint32	<invalid unsigned integer 32-bit format>		/error
invcid		<invalid class identifier format>			/error
invsid		<invalid subscription identifier format>		/error
invdid		<invalid direct link identifier format>			/error
invtid		<invalid type identifier format>			/error
invvid		<invalid volume identifier format>			/error
invoix		<invalid object index format>				/error
seglen		<name segment is too long>				/error
namelen		<name is too long>					/error
noname		<no name is given>					/error
invchar		<invalid character>					/error
invid		<invalid identifier>					/error
invidchar	<invalid identifier character>				/error
invsyntax	<invalid syntax>					/error
invtypename	<invalid type name>					/error
invattrname	<invalid attribute name>				/error
invbid		<invalid body identifier>				/error
invattrindex	<invalid attribute index>				/error
