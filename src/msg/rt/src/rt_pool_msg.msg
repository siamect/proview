!  rt_pool_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1997 by Comator Process AB.
!
!  <Description>.
!
.facility	POOL,13 /prefix = POOL__	! Pool package

success		<successful completion>					/succ
badoffs		<invalid pool reference>				/fatal
noslot		<pool directory full>					/fatal
badseg		<invalid pool reference>				/fatal
segsize		<invalid pool reference>				/fatal
nomark		<invalid pool reference>				/fatal
remapnyi	<remap needed, not implemented>				/fatal
bugcheck	<internal consistency error>				/fatal
insvirmem	<insufficient virtual memory>				/fatal
nulladdr	<a null address was passed>				/fatal
nullref		<a null reference was passed>				/fatal
incon		<pool inconsistency>					/fatal
toobig		<requested segment is too big>				/error
allrexist	<pool segment allready exist>				/error
allrmapped	<pool is allready mapped>				/error
nolist		<no free lookaside list>				/error
freenamed       <named segment cannot be freed>				/error
notcreated	<pool is not yet created>				/error

.end
