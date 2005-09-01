! 
!  Proview   $Id: wb_wsx_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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
!  wb_wsx_msg.msg -- workbench syntax control
!
!  This module contains the defintion of the error messages used
!  in wsx modules.
!
.facility       WSX,221 /prefix = WSX__	! Wb syntax check

success		<successful completion>					/succ
carddevname	<DevName in card-object is missing>			/error
carderrsoftlim	<ErrorSoftLimit in card-object is missing>		/error
carderrhardlim	<ErrorHardLimit in card-object is missing>		/error
sigchancon	<Error in SigChanCon-object>				/error
sigchanconclass	<Error in class of SigChanCon-object>			/error
maxchan		<MaxNoOfChannels is out of range>			/error
numrange	<Number of is out of range>				/error
numnotunique	<Number is not unique>					/error
chancount	<Max number of channels exceeded>			/error
misplaced	<Misplaced object>					/error
bckinvalid	<Invalid backup data, check DataName>			/error
osinvalid	<Invalid operating system>				/error
nodecount	<Invalid number of $Node objects in volume>		/error
plcthread	<Invalid threadobject for plc program>			/error
plcwind		<No objects found in plcprogram>			/error
plcchild	<Misplaced child to plcprogram>				/error
dicount		<Max number of Di objects exceeded>			/error
docount		<Max number of Do objects exceeded>			/error
dvcount		<Max number of Dv objects exceeded>			/error
aicount		<Max number of Ai objects exceeded>			/error
aocount		<Max number of Ao objects exceeded>			/error
avcount		<Max number of Av objects exceeded>			/error
cocount		<Max number of Co objects exceeded>			/error
