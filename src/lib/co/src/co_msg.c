#if defined OS_ELN
# include $kerneldef
# include $vaxelnc
# include stdio
# include descrip
#elif defined OS_VMS
# include <string.h>
# include <descrip.h>
# include <starlet.h>
#elif defined(OS_LYNX) || defined(OS_LINUX)
# include <string.h>
#endif

#include "co_msg.h"
#include "pwr_msg.h"

#define SEVERITY(sts)	(sts & 0x7)
#define FACNUM(sts)	(((sts >> 16) & 0x0fff) - 0x800)
#define MSGIDX(sts)	((sts & 0x7fff) >> 3)

#if defined OS_LYNX || defined OS_LINUX

  /* The heads of the message lists.  */

  extern msg_sHead coMsgHead;
  extern msg_sHead opMsgHead;
  extern msg_sHead qcomMsgHead;
  extern msg_sHead rtMsgHead;
  extern msg_sHead wbMsgHead;
  extern msg_sHead geMsgHead;
  extern msg_sHead flowMsgHead;
  extern msg_sHead pwrpMsgHead;

  static const msg_sHead *heads[] = {
    &rtMsgHead,
    &pwrpMsgHead,
    &qcomMsgHead,
    &coMsgHead,
    &opMsgHead,
    &wbMsgHead,
    &geMsgHead,
    &flowMsgHead,
    NULL
  };

  static msg_sFacility *
  facility (
    int facNum, 
    int idx 
  );

  static msg_sFacility *
  IsRightHead (
    const msg_sHead *h,
    int facNum, 
    int idx,
    int *invalidIdx /* Only valid if NULL is returned */
  );
#endif


/* Check if a given messagenumber exists,
   return string representation if valid.  */


char *
msg_GetMsg (
  const int	sts,
  char		*buf,
  int		bufSize
)
{

  return msg_GetMessage(sts, 0xf, buf, bufSize);
}


/* Checks if a given messagenumber exists,
  return string representation if valid.  */

char *
msg_GetText (
  const int sts,
  char *buf,
  int bufSize
)
{
  return msg_GetMessage(sts, 1, buf, bufSize);
}

#if defined OS_LYNX || defined OS_LINUX

/* Return the address to the Facility which contains the
   FacNum and MsgIdx.  */

static msg_sFacility *
facility(int facNum, int idx)
{
  msg_sFacility *f = NULL;  
  int i, invalidIdx;
  
  for (i = 0; heads[i] != NULL; i++) {
    if (heads[i]->NofFacility != 0) {
      f = IsRightHead(heads[i], facNum, idx, &invalidIdx); 
      if (f != NULL || invalidIdx)
        break;
    }
  } 
  return f;
}

/* Returns the address to the Facility which contains the
   FacNum and MsgIdx.
   
   'invalidIdx will be set to TRUE if the facnum is found
   but the idx  isn't valid.  */

static msg_sFacility *
IsRightHead (
  const msg_sHead *h,
  int facNum, 
  int idx,
  int *invalidIdx
)
{
  int i;
  msg_sFacility **f = h->Facility;
  *invalidIdx = 0;
 
  for (i = 0; i < h->NofFacility; i++) {
    if (f[i]->FacNum == facNum) {
      if ((idx > f[i]->NofMsg) || (idx < 1) ) {
        *invalidIdx = 1;
	return NULL;
      } else {
        return f[i];
      }
    }
  }

  return NULL;
}
#endif

#if defined(OS_VMS) || defined(OS_ELN)

char *
msg_GetMessage (
  const pwr_tStatus	sts,
  unsigned int		flags,
  char			*buf,
  int			bufSize
)
{
  char			dsc_buf[256];
  struct dsc$descriptor desc;
  unsigned short	msglen;

  desc.dsc$a_pointer  = dsc_buf;
  desc.dsc$w_length   = sizeof(dsc_buf);	
  desc.dsc$b_dtype    = DSC$K_DTYPE_T;
  desc.dsc$b_class    = DSC$K_CLASS_S;

  sys$getmsg(sts, &msglen, &desc, flags, NULL);
  if (flags == 0xf || flags == 1) {
    strncpy(buf, dsc_buf, MIN(bufSize, msglen));
    buf[MIN(msglen, bufSize)] = '\0';
  } else {
    strncpy(buf, &dsc_buf[1], MIN(bufSize, msglen-1));
    buf[MIN(msglen-1, bufSize)] = '\0';
  }
  return buf;

}
#else
static char
get_severity (
  pwr_tStatus sts
)
{

  switch (sts & 7) {
  case 0:  return 'W';
  case 1:  return 'S';
  case 2:  return 'E';
  case 3:  return 'I';
  case 4:  return 'F';
  default: return '?';
  }
}

char *
msg_GetMessage (
  const pwr_tStatus	sts,
  unsigned int		flags,
  char			*buf,
  int			bufSize
)
{
  int			facNum = FACNUM(sts);
  int			idx = MSGIDX(sts);
  msg_sFacility		*f = facility(facNum, idx);
  char			msg[256];
  char			*s = msg;
  char			*facName = "NONAME";
  char			*msgName = "NOMSG";
  char			text[sizeof("Message number 00000000")]; 
  char			*msgTxt = text;

  if (f != NULL) {
    facName = f->FacName;
    msgName = f->Msg[idx-1].MsgName;
    msgTxt  = f->Msg[idx-1].MsgTxt;
  } else {
    sprintf(text, "Message number %08X", sts);
  }

  if ((flags & 0xf) == 0xf)
    *s++ = '%';
  if (flags & 0x8)
    s += sprintf(s, "%s", facName);
  if (flags & 0x4) {
    if (flags & 0x8)
      *s++ = '-';
    *s++ = get_severity(sts);
  }
  if (flags & 0x2) {
    if (flags & 0xc)
      *s++ = '-';
    s += sprintf(s, "%s", msgName);
  }
  if (flags & 0x1) {
    if (flags & ~0x1)
      s += sprintf(s, ", ");
    s += sprintf(s, "%s", msgTxt);
  }
    
  *s = '\0';

  strncpy(buf, msg, bufSize - 1);
  buf[bufSize - 1] = '\0';

  return buf;

}
#endif
