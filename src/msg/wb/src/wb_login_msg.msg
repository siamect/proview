!  wb_login_msg.msg -- <short description>
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  <Description>.

        .facility       LOGIN,320 /prefix = LOGIN__ ! Login

success		<successful completion>					/succ
usernotau	<user not authorized>					/error
userexist	<user already exixts>					/error
nomemory	<no dynamic memory>					/error
nouserlist	<userlist not opened>					/error
nouser		<no such user>						/error
userfile	<unable to oper userlist data file>			/error
nosave		<nothing written in user data file>			/info
nopriv		<no privilege for attempted operation>			/error
authfail	<user authorization failure>				/error
