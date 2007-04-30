/* 
 * Proview   $Id: rt_errh.h,v 1.7 2007-04-30 07:27:59 claes Exp $
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

#ifndef rt_errh_h
#define rt_errh_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#ifndef rt_errl_h
# include "rt_errl.h"
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

#define errh_SeveritySuccess(sts) 	((sts) & 7 == 3)
#define errh_SeverityInfo(sts) 		((sts) & 7 == 1)
#define errh_SeverityWarning(sts) 	((sts) & 7 == 0)
#define errh_SeverityError(sts) 	((sts) & 7 == 2)
#define errh_SeverityFatal(sts) 	((sts) & 7 == 4)

typedef enum {
  errh_eSeverity_Null,
  errh_eSeverity_Success,
  errh_eSeverity_Info,
  errh_eSeverity_Warning,
  errh_eSeverity_Error,
  errh_eSeverity_Fatal
} errh_eSeverity;


#define errh_cAnix_SrvSize 40

typedef enum {
  errh_eNAnix		= 0,
  errh_eAnix_ini 	= 1,
  errh_eAnix_qmon 	= 2,
  errh_eAnix_neth 	= 3,
  errh_eAnix_neth_acp 	= 4,
  errh_eAnix_io		= 5,
  errh_eAnix_tmon 	= 6,
  errh_eAnix_emon 	= 7,
  errh_eAnix_alim 	= 8,
  errh_eAnix_bck 	= 9,
  errh_eAnix_linksup 	= 10,
  errh_eAnix_trend 	= 11,
  errh_eAnix_fast 	= 12,
  errh_eAnix_elog 	= 13,
  errh_eAnix_webmon 	= 14,
  errh_eAnix_webmonmh 	= 15,
  errh_eAnix_sysmon	= 16,
  errh_eAnix_plc 	= 17,
  errh_eAnix_remote 	= 18,
  errh_eAnix_opc_server	= 19,
  errh_eAnix_nmps_bck   = 20,
  errh_eAnix_appl1	= 21,
  errh_eAnix_appl2	= 22,
  errh_eAnix_appl3	= 23,
  errh_eAnix_appl4	= 24,
  errh_eAnix_appl5	= 25,
  errh_eAnix_appl6	= 26,
  errh_eAnix_appl7	= 27,
  errh_eAnix_appl8	= 28,
  errh_eAnix_appl9	= 29,
  errh_eAnix_appl10	= 30,
  errh_eAnix_appl11	= 31,
  errh_eAnix_appl12	= 32,
  errh_eAnix_appl13	= 33,
  errh_eAnix_appl14	= 34,
  errh_eAnix_appl15	= 35,
  errh_eAnix_appl16	= 36,
  errh_eAnix_appl17	= 37,
  errh_eAnix_appl18	= 38,
  errh_eAnix_appl19	= 39,
  errh_eAnix_appl20	= 40
} errh_eAnix;

typedef enum {
  errh_eMsgType_Log	= 1,
  errh_eMsgType_Status 	= 2
} errh_eMsgType;


typedef struct {
  pwr_tBoolean send;
  qcom_sQid logQ;
  qcom_sPut put;
} errh_sLog;

typedef struct {
  long int message_type;
  pwr_tStatus sts;
  errh_eAnix anix;
  char severity;
  char str[LOG_MAX_MSG_SIZE];
} errh_sMsg;

/* NOTE! errh_Init MUST always be called before any other
	 errh-function is called.  */

pwr_tStatus	errh_Init	(char *programName, errh_eAnix anix);
void		errh_SetStatus  (pwr_tStatus sts);
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
errh_eAnix	errh_Anix       ();
errh_eSeverity	errh_Severity	(pwr_tStatus);
#if defined __cplusplus
}
#endif

#endif





