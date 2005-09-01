/** 
 * Proview   $Id: co_errno.h,v 1.3 2005-09-01 14:57:52 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#ifndef co_errno_h
#define co_errno_h

#ifdef __cplusplus
extern "C" {
#endif

/* co_errno.h -- Translate UNIX errno error codes. */

#ifndef pwr_h
# include "pwr.h" 
#endif
#ifndef pwr_msg_h
# include "pwr_msg.h"
#endif
#ifndef co_errno_msg_h
# include "co_errno_msg.h"
#endif

#define ERRNO_BADBIT 16384
#define EBADCODE 65534
#define ERRNO_BADCODE(code) (pwr_tStatus)((MSG_CLEAR_CODE(ERRNO__BADCODE)) | ERRNO_BADBIT | ((code) << MSG_S_CODE))

int		errno_ExitStatus(pwr_tStatus);
int		errno_ExitErrno(int);
pwr_tStatus	errno_Pstatus(int);
pwr_tStatus	errno_Status(int);
pwr_tStatus	errno_GetStatus();

#ifdef __cplusplus
}
#endif

#endif
