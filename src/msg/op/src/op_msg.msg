!  op_msg.msg -- messages from operator environment
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  <Description>.
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

