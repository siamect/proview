!  wb_ate_msg.msg -- attribute editor messages
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  This module contains the defintion of the error messages used
!  in ate modules.
!
.facility       ATE,300 /prefix = ATE__	! Attribute editor

success		<successful completion>					/succ
converr		<error in converting string to value>			/error
internerr	<internal error>					/fatal
outofmemory	<out of memory>						/fatal
couldntalloc	<XtCalloc failed to allocate more memory>		/fatal

.end
