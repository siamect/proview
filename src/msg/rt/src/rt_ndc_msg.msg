!  rt_ndc_msg.msg -- Network data conversion
!
!  PROVIEW/R
!  Copyright (C) 1997 by Comator Process AB.
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
