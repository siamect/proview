/* 
 * Proview   $Id: co_errno.c,v 1.2 2005-09-01 14:57:52 claes Exp $
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
 */

/* co_errno.c -- Translate UNIX errno to Proview status.

   NOTE! The actual values of errno codes in <errno.h> must match
         the vector index used in 'errno_status'.  */ 
 
#include <errno.h>
#include "pwr.h"
#include "co_errno.h"
#include "co_errno_msg.h"

#ifndef OS_VMS
# error This file is only for OpenVMS
#endif

static struct {
  int err_no;
  pwr_tStatus sts;
} errno_status[] = {
  {0, ERRNO__SUCCESS},
  {EPERM, ERRNO__PERM},
  {ENOENT, ERRNO__NOENT},
  {ESRCH, ERRNO__SRCH},
  {EINTR, ERRNO__INTR},
  {EIO, ERRNO__IO},
  {ENXIO, ERRNO__NXIO},
  {E2BIG, ERRNO__2BIG},
  {ENOEXEC, ERRNO__NOEXEC},
  {EBADF, ERRNO__BADF},
  {ECHILD, ERRNO__CHILD},
  {EAGAIN, ERRNO__AGAIN},
  {ENOMEM, ERRNO__NOMEM},
  {EACCES, ERRNO__ACCES},
  {EFAULT, ERRNO__FAULT},
  {ENOTBLK, ERRNO__NOTBLK},
  {EBUSY, ERRNO__BUSY},
  {EEXIST, ERRNO__EXIST},
  {EXDEV, ERRNO__XDEV},
  {ENODEV, ERRNO__NODEV},
  {ENOTDIR, ERRNO__NOTDIR},
  {EISDIR, ERRNO__ISDIR},
  {EINVAL, ERRNO__INVAL},
  {ENFILE, ERRNO__NFILE},
  {EMFILE, ERRNO__MFILE},
  {ENOTTY, ERRNO__NOTTY},
  {ETXTBSY, ERRNO__TXTBSY},
  {EFBIG, ERRNO__FBIG},
  {ENOSPC, ERRNO__NOSPC},
  {ESPIPE, ERRNO__SPIPE},
  {EROFS, ERRNO__ROFS},
  {EMLINK, ERRNO__MLINK},
  {EPIPE, ERRNO__PIPE},
  {EDOM, ERRNO__DOM},
  {ERANGE, ERRNO__RANGE},
  {EWOULDBLOCK, ERRNO__WOULDBLOCK},
  {EINPROGRESS, ERRNO__INPROGRESS},
  {EALREADY, ERRNO__ALREADY},
  {ENOTSOCK, ERRNO__NOTSOCK},
  {EDESTADDRREQ, ERRNO__DESTADDRREQ},
  {EMSGSIZE, ERRNO__MSGSIZE},
  {EPROTOTYPE, ERRNO__PROTOTYPE},
  {ENOPROTOOPT, ERRNO__NOPROTOOPT},
  {EPROTONOSUPPORT, ERRNO__PROTONOSUPPORT},
  {ESOCKTNOSUPPORT, ERRNO__SOCKTNOSUPPORT},
  {EOPNOTSUPP, ERRNO__OPNOTSUPP},
  {EPFNOSUPPORT, ERRNO__PFNOSUPPORT},
  {EAFNOSUPPORT, ERRNO__AFNOSUPPORT},
  {EADDRINUSE, ERRNO__ADDRINUSE},
  {EADDRNOTAVAIL, ERRNO__ADDRNOTAVAIL},
  {ENETDOWN, ERRNO__NETDOWN},
  {ENETUNREACH, ERRNO__NETUNREACH},
  {ENETRESET, ERRNO__NETRESET},
  {ECONNABORTED, ERRNO__CONNABORTED},
  {ECONNRESET, ERRNO__CONNRESET},
  {ENOBUFS, ERRNO__NOBUFS},
  {EISCONN, ERRNO__ISCONN},
  {ENOTCONN, ERRNO__NOTCONN},
  {ESHUTDOWN, ERRNO__SHUTDOWN},
  {ETOOMANYREFS, ERRNO__TOOMANYREFS},
  {ETIMEDOUT, ERRNO__TIMEDOUT},
  {ECONNREFUSED, ERRNO__CONNREFUSED},
  {ELOOP, ERRNO__LOOP},
  {ENAMETOOLONG, ERRNO__NAMETOOLONG},
  {EHOSTDOWN, ERRNO__HOSTDOWN},
  {EHOSTUNREACH, ERRNO__HOSTUNREACH},
  {ENOTEMPTY, ERRNO__NOTEMPTY},
  {EPROCLIM, ERRNO__PROCLIM},
  {EUSERS, ERRNO__USERS},
  {EDQUOT, ERRNO__DQUOT},
  {ENOMSG, ERRNO__NOMSG},
  {EIDRM, ERRNO__IDRM},
  {EALIGN, ERRNO__ALIGN},
  {ESTALE, ERRNO__STALE},
  {EREMOTE, ERRNO__REMOTE},
  {ENOLCK, ERRNO__NOLCK},
  {ENOSYS, ERRNO__NOSYS},
  {EFTYPE, ERRNO__FTYPE},
  {ECANCELED, ERRNO__CANCELED},
  {EFAIL, ERRNO__FAIL},
  {EINPROG, ERRNO__INPROG},
  {ENOTSUP, ERRNO__NOTSUP},
  {EDEADLK, ERRNO__DEADLK},
  {ENWAIT, ERRNO__NWAIT},
  {EILSEQ, ERRNO__ILSEQ},
  {EBADCAT, ERRNO__BADCAT},
  {EBADMSG, ERRNO__BADMSG}
};

#define cMaxErrno ((int) (sizeof(errno_status) / sizeof(errno_status[0])))


int
errno_ExitStatus (
  pwr_tStatus sts
)
{
  return (int)sts;
}

int
errno_ExitErrno (
  int err_no
)
{
  return (int)errno_Status(err_no);
}

/* Check a POSIX return code and return
   status on PWR format, using errno. */

pwr_tStatus
errno_Pstatus (
  int psts
)
{

  if (psts == 0)
    return ERRNO__SUCCESS;

  pwr_Assert(psts == -1);

  if (errno < 0 || errno >= cMaxErrno)
    return ERRNO_BADCODE(errno);

  if (errno_status[errno].err_no == errno)
    return errno_status[errno].sts;

  return ERRNO_BADCODE(errno);
}

pwr_tStatus
errno_Status (
  int err_no
)
{
  pwr_tStatus sts;

  if (err_no < 0 || err_no >= cMaxErrno)
    return ERRNO_BADCODE(err_no);

  if (err_no == EVMSERR)
    return ERRNO__VMSERR;

  if (errno_status[err_no].err_no == err_no)
    return errno_status[err_no].sts;

  return ERRNO_BADCODE(err_no);
}

pwr_tStatus
errno_GetStatus ()
{
  pwr_tStatus sts;

  if (errno < 0 || errno >= cMaxErrno)
    return ERRNO_BADCODE(errno);

  if (errno == EVMSERR)
    return ERRNO__VMSERR;

  if (errno_status[errno].err_no == errno)
    return errno_status[errno].sts;

  return ERRNO_BADCODE(errno);
}
