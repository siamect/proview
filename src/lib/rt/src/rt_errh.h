#ifndef rt_errh_h
#define rt_errh_h

/* rt_errh.h -- <short description>

   PROVIEW/R
   Copyright (C) 1998 by Mandator AB.

   <Description>.  
*/

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#if defined __cplusplus
extern "C" {
#endif

#ifndef errh_Bugcheck
# if defined OS_ELN || defined OS_VMS
#   define errh_Bugcheck(sts, str) \
	(errh_Fatal("pwr bugcheck: <%s>, in file %s, at line %d\n", (str),__FILE__,__LINE__),(lib$signal(sts)))
# elif defined OS_LYNX || defined OS_LINUX
#   define errh_Bugcheck(sts, str) \
	(errh_Fatal("pwr bugcheck: <%s>, in file %s, at line %d\n%m", (str),__FILE__,__LINE__, sts),(exit(sts)))
# else
#   error Platform not defined
# endif
#endif

#ifndef errh_ReturnOrBugcheck
# define errh_ReturnOrBugcheck(a, sts, lsts, str)\
         return (((int)(sts)?((*sts)=(lsts)):(EVEN(lsts)?(errh_Bugcheck(lsts, (str)),(lsts)):(lsts))),a)
#endif


typedef struct {
  pwr_tBoolean send;
  qcom_sQid logQ;
  qcom_sPut put;
} errh_sLog;

/* NOTE! errh_Init MUST always be called before any other
	 errh-function is called.  */

pwr_tStatus	errh_Init	(char *programName);
void		errh_Interactive	();
char		*errh_GetMsg	(const pwr_tStatus sts, char *buf, int bufSize);
char		*errh_GetText	(const pwr_tStatus sts, char *buf, int bufSize);
char		*errh_Log	(char *buff, char severity, char *msg, ...);
void		errh_Fatal	(char *msg, ...);
void		errh_Error	(char *msg, ...);
void		errh_Warning	(char *msg, ...);
void		errh_Info	(char *msg, ...);
void		errh_Success	(char *msg, ...);
void		errh_LogFatal	(errh_sLog*, char *msg, ...);
void		errh_LogError	(errh_sLog*, char *msg, ...);
void		errh_LogWarning	(errh_sLog*, char *msg, ...);
void		errh_LogInfo	(errh_sLog*, char *msg, ...);
void		errh_LogSuccess	(errh_sLog*, char *msg, ...);
void		*errh_ErrArgMsg	(pwr_tStatus sts);
void		*errh_ErrArgAF	(char *s);
void		*errh_ErrArgL	(int val);
void		errh_CErrLog	(pwr_tStatus sts, ...);
char		*errh_Message	(char *string, char severity, char *msg, ...);

#if defined __cplusplus
}
#endif

#endif
