!  wb_ldh_msg.msg -- local data handler
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
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
