#ifndef co_errno_h
#define co_errno_h

#ifdef __cplusplus
extern "C" {
#endif

/* co_errno.h -- Translate UNIX errno error codes.

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   . */

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
