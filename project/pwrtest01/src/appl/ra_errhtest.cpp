/*
  Test errh functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>
#include "pwr_class.h"
#include "co_msg.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "rt_aproc.h"
#include "ra_errhtest.h"
#include "rt_pwr_msg.h"
#include "rt_errh_msg.h"
#include "rt_plc_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"

static float errhtest_wait=0.01;

void ra_errhtest::Init(void)
{
  m_sts = errh_Init("ra_errhtest", m_anix);
  if (m_sts != 1) {
    m_log->log('E', "Init", m_sts);
    return;
  }

  m_log->log('S', "Init", PWR__SUCCESS);
}

typedef enum {
  eErrht_No,
  eErrht_Int,
  eErrht_Sts,
  eErrht_Str
} eErrht;

typedef union {
  int i;
  float f;
  pwr_tStatus sts;
  char s[40];
} uErrht;

typedef struct {
  char str[80];
  eErrht argtype;
  uErrht arg;
  char infostr[200];
} sErrht;

void ra_errhtest::Success(void)
{
  sErrht d[] = {
    {"This is a success message",eErrht_No,{.i=0}, "This is a success message"},
    {"This is another success message, %d",eErrht_Int,{.i=222}, "This is another success message, 222"},
    {"This is another success message, %08d",eErrht_Int,{.i=222}, "This is another success message, 00000222"},
    {"This is another success message, %x",eErrht_Int,{.i=0xabcd123}, "This is another success message, abcd123"},
    {"This is another success message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "This is another success message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another success message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "This is another success message, String argument"}
  };
  
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_Success(d[i].str); 
      break;
    case eErrht_Int:
      errh_Success(d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_Success(d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_Success(d[i].str, d[i].arg.s); 
      break;
    }
    time_Sleep(errhtest_wait);
    if (strcmp(m_nodep->ProcMessage[m_anix-1], d[i].infostr) != 0) {
      m_log->log('E', "Success", i);
      return;
    }
  }

  m_log->log('S', "Success", PWR__SUCCESS);
}

void ra_errhtest::Info(void)
{
  sErrht d[] = {
    {"This is an info message",eErrht_No,{.i=0}, "This is an info message"},
    {"This is another info message, %d",eErrht_Int,{.i=222}, "This is another info message, 222"},
    {"This is another info message, %08d",eErrht_Int,{.i=222}, "This is another info message, 00000222"},
    {"This is another info message, %x",eErrht_Int,{.i=0xabcd123}, "This is another info message, abcd123"},
    {"This is another info message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "This is another info message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another info message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "This is another info message, String argument"}
  };
  
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_Info(d[i].str); 
      break;
    case eErrht_Int:
      errh_Info(d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_Info(d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_Info(d[i].str, d[i].arg.s); 
      break;
    }
    time_Sleep(errhtest_wait);
    if (strcmp(m_nodep->ProcMessage[m_anix-1], d[i].infostr) != 0) {
      m_log->log('E', "Info", i);
      return;
    }
  }

  m_log->log('S', "Info", PWR__SUCCESS);
}

void ra_errhtest::Warning(void)
{
  sErrht d[] = {
    {"This is a warning message",eErrht_No,{.i=0}, "This is a warning message"},
    {"This is another warning message, %d",eErrht_Int,{.i=222}, "This is another warning message, 222"},
    {"This is another warning message, %08d",eErrht_Int,{.i=222}, "This is another warning message, 00000222"},
    {"This is another warning message, %x",eErrht_Int,{.i=0xabcd123}, "This is another warning message, abcd123"},
    {"This is another warning message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "This is another warning message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another warning message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "This is another warning message, String argument"}
  };
  
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_Warning(d[i].str); 
      break;
    case eErrht_Int:
      errh_Warning(d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_Warning(d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_Warning(d[i].str, d[i].arg.s); 
      break;
    }
    time_Sleep(errhtest_wait);
    if (strcmp(m_nodep->ProcMessage[m_anix-1], d[i].infostr) != 0) {
      m_log->log('E', "Warning", i);
      return;
    }
  }

  m_log->log('S', "Warning", PWR__SUCCESS);
}

void ra_errhtest::Error(void)
{
  sErrht d[] = {
    {"This is an error message",eErrht_No,{.i=0}, "This is an error message"},
    {"This is another error message, %d",eErrht_Int,{.i=222}, "This is another error message, 222"},
    {"This is another error message, %08d",eErrht_Int,{.i=222}, "This is another error message, 00000222"},
    {"This is another error message, %x",eErrht_Int,{.i=0xabcd123}, "This is another error message, abcd123"},
    {"This is another error message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "This is another error message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another error message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "This is another error message, String argument"}
  };
  
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_Error(d[i].str); 
      break;
    case eErrht_Int:
      errh_Error(d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_Error(d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_Error(d[i].str, d[i].arg.s); 
      break;
    }
    time_Sleep(errhtest_wait);
    if (strcmp(m_nodep->ProcMessage[m_anix-1], d[i].infostr) != 0) {
      m_log->log('E', "Error", i);
      return;
    }
  }

  m_log->log('S', "Error", PWR__SUCCESS);
}

void ra_errhtest::Fatal(void)
{
  sErrht d[] = {
    {"This is a fatal message",eErrht_No,{.i=0}, "This is a fatal message"},
    {"This is another fatal message, %d",eErrht_Int,{.i=222}, "This is another fatal message, 222"},
    {"This is another fatal message, %08d",eErrht_Int,{.i=222}, "This is another fatal message, 00000222"},
    {"This is another fatal message, %x",eErrht_Int,{.i=0xabcd123}, "This is another fatal message, abcd123"},
    {"This is another fatal message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "This is another fatal message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another fatal message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "This is another fatal message, String argument"}
  };
  
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_Fatal(d[i].str); 
      break;
    case eErrht_Int:
      errh_Fatal(d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_Fatal(d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_Fatal(d[i].str, d[i].arg.s); 
      break;
    }
    time_Sleep(errhtest_wait);
    if (strcmp(m_nodep->ProcMessage[m_anix-1], d[i].infostr) != 0) {
      m_log->log('E', "Fatal", i);
      return;
    }
  }

  m_log->log('S', "Fatal", PWR__SUCCESS);
}

typedef struct {
  pwr_tStatus sts;
  eErrht argtype;
  uErrht arg;
  char infostr[200];
} sCErrLog;

void ra_errhtest::CErrLog(void)
{
  sCErrLog d[] = {
    {PWR__SUCCESS,eErrht_No,{.i=0}, "PWR-SUCCESS, Successful completion"},
    {PLC__ERRCREOBJ,eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "PLC-ERRCREOBJ, create object \'String argument\'"}
  };
  
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_CErrLog(d[i].sts,NULL); 
      break;
    case eErrht_Int:
      errh_CErrLog(d[i].sts, errh_ErrArgL(d[i].arg.i), NULL); 
      break;
    case eErrht_Sts:
      errh_CErrLog(d[i].sts, errh_ErrArgMsg(d[i].arg.sts), NULL); 
      break;
    case eErrht_Str:
      errh_CErrLog(d[i].sts, errh_ErrArgAF(d[i].arg.s), NULL); 
      break;
    }
    time_Sleep(errhtest_wait);
    if (strcmp(m_nodep->ProcMessage[m_anix-1], d[i].infostr) != 0) {
      m_log->log('E', "CErrLog", i);
      return;
    }
  }

  m_log->log('S', "CErrLog", PWR__SUCCESS);
}

void ra_errhtest::SetAnix(void)
{
  errh_eAnix anix = errh_eAnix_appl2;

  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  m_nodep->ProcMsgSeverity[anix-1] = 0;
  errh_SetAnix(anix);
  errh_Info("Setting anix to appl2"); 
  time_Sleep(errhtest_wait);
  if (strcmp(m_nodep->ProcMessage[anix-1], "Setting anix to appl2") != 0) {
    m_log->log('E', "SetAnix");
    errh_SetAnix(m_anix);
    return;
  }

  errh_SetAnix(m_anix);

  m_log->log('S', "SetAnix", PWR__SUCCESS);
}

typedef struct {
  pwr_tStatus sts;
} sSetStatus;

void ra_errhtest::SetStatus(void)
{
  sCErrLog d[] = {
    {PWR__ARUN},
    {PWR__SRUN},
    {PWR__APPLWARNING},
    {PWR__APPLINFO},
    {0}
  };

  aproc_TimeStamp(10, 10);
  m_nodep->ProcStatus[m_anix-1] = 0;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    errh_SetStatus(d[i].sts); 
    time_Sleep(errhtest_wait);
    if (m_nodep->ProcStatus[m_anix-1] != d[i].sts) {
      m_log->vlog('E', "SetStatus, idx %d (%d != %d)", i, 
		  m_nodep->ProcStatus[m_anix-1], d[i].sts);
      return;
    }
  }

  m_log->log('S', "SetStatus", PWR__SUCCESS);
}

// Should be last, sets interactive

void ra_errhtest::LogSuccess(void)
{
  sErrht d[] = {
    {"This is a success message",eErrht_No,{.i=0}, "S This is a success message"},
    {"This is another success message, %d",eErrht_Int,{.i=222}, "S This is another success message, 222"},
    {"This is another success message, %08d",eErrht_Int,{.i=222}, "S This is another success message, 00000222"},
    {"This is another success message, %x",eErrht_Int,{.i=0xabcd123}, "S This is another success message, abcd123"},
    {"This is another success message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "S This is another success message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another success message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "S This is another success message, String argument"}
  };
  pwr_tFileName fname, fname2;
  FILE *fp;
  char line[200];
  
  errh_Interactive();
  dcli_translate_filename(fname, "/tmp/errh.txt");
  freopen(fname, "w", stdout);
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_LogSuccess(0, d[i].str); 
      break;
    case eErrht_Int:
      errh_LogSuccess(0, d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_LogSuccess(0, d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_LogSuccess(0, d[i].str, d[i].arg.s); 
      break;
    }
  }
  dcli_translate_filename(fname2, "/tmp/errh2.txt");
  freopen(fname2, "w", stdout);

  fp = fopen(fname, "r");
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (dcli_read_line(line, sizeof(line), fp)) {
      if (strcmp(line, d[i].infostr) != 0) {
	m_log->log('E', "LogSuccess", i);
	fclose(fp);
	return;
      }
    }
    else {
      m_log->log('E', "LogSuccess, no line", i);
      fclose(fp);
      return;
    }
  }
  fclose(fp);

  m_log->log('S', "LogSuccess", PWR__SUCCESS);
}

void ra_errhtest::LogInfo(void)
{
  sErrht d[] = {
    {"This is a info message",eErrht_No,{.i=0}, "I This is a info message"},
    {"This is another info message, %d",eErrht_Int,{.i=222}, "I This is another info message, 222"},
    {"This is another info message, %08d",eErrht_Int,{.i=222}, "I This is another info message, 00000222"},
    {"This is another info message, %x",eErrht_Int,{.i=0xabcd123}, "I This is another info message, abcd123"},
    {"This is another info message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "I This is another info message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another info message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "I This is another info message, String argument"}
  };
  pwr_tFileName fname, fname2;
  FILE *fp;
  char line[200];
  
  errh_Interactive();
  dcli_translate_filename(fname, "/tmp/errh.txt");
  freopen(fname, "w", stdout);
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_LogInfo(0, d[i].str); 
      break;
    case eErrht_Int:
      errh_LogInfo(0, d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_LogInfo(0, d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_LogInfo(0, d[i].str, d[i].arg.s); 
      break;
    }
  }
  dcli_translate_filename(fname2, "/tmp/errh2.txt");
  freopen(fname2, "w", stdout);

  fp = fopen(fname, "r");
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (dcli_read_line(line, sizeof(line), fp)) {
      if (strcmp(line, d[i].infostr) != 0) {
	m_log->log('E', "LogInfo", i);
	fclose(fp);
	return;
      }
    }
    else {
      m_log->log('E', "LogInfo, no line", i);
      fclose(fp);
      return;
    }
  }
  fclose(fp);

  m_log->log('S', "LogInfo", PWR__SUCCESS);
}

void ra_errhtest::LogError(void)
{
  sErrht d[] = {
    {"This is a error message",eErrht_No,{.i=0}, "E This is a error message"},
    {"This is another error message, %d",eErrht_Int,{.i=222}, "E This is another error message, 222"},
    {"This is another error message, %08d",eErrht_Int,{.i=222}, "E This is another error message, 00000222"},
    {"This is another error message, %x",eErrht_Int,{.i=0xabcd123}, "E This is another error message, abcd123"},
    {"This is another error message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "E This is another error message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another error message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "E This is another error message, String argument"}
  };
  pwr_tFileName fname, fname2;
  FILE *fp;
  char line[200];
  
  errh_Interactive();
  dcli_translate_filename(fname, "/tmp/errh.txt");
  freopen(fname, "w", stdout);
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_LogError(0, d[i].str); 
      break;
    case eErrht_Int:
      errh_LogError(0, d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_LogError(0, d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_LogError(0, d[i].str, d[i].arg.s); 
      break;
    }
  }
  dcli_translate_filename(fname2, "/tmp/errh2.txt");
  freopen(fname2, "w", stdout);

  fp = fopen(fname, "r");
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (dcli_read_line(line, sizeof(line), fp)) {
      if (strcmp(line, d[i].infostr) != 0) {
	m_log->log('E', "LogError", i);
	fclose(fp);
	return;
      }
    }
    else {
      m_log->log('E', "LogError, no line", i);
      fclose(fp);
      return;
    }
  }
  fclose(fp);

  m_log->log('S', "LogError", PWR__SUCCESS);
}

void ra_errhtest::LogFatal(void)
{
  sErrht d[] = {
    {"This is a fatal message",eErrht_No,{.i=0}, "F This is a fatal message"},
    {"This is another fatal message, %d",eErrht_Int,{.i=222}, "F This is another fatal message, 222"},
    {"This is another fatal message, %08d",eErrht_Int,{.i=222}, "F This is another fatal message, 00000222"},
    {"This is another fatal message, %x",eErrht_Int,{.i=0xabcd123}, "F This is another fatal message, abcd123"},
    {"This is another fatal message, %m",eErrht_Sts,{.sts=PWR__SUCCESS}, "F This is another fatal message, %PWR-S-SUCCESS, Successful completion"},
    {"This is another fatal message, %s",eErrht_Str,{.s={'S','t','r','i','n','g',' ','a','r','g','u','m','e','n','t'}}, "F This is another fatal message, String argument"}
  };
  pwr_tFileName fname, fname2;
  FILE *fp;
  char line[200];
  
  errh_Interactive();
  dcli_translate_filename(fname, "/tmp/errh.txt");
  freopen(fname, "w", stdout);
  m_nodep->ProcMsgSeverity[m_anix-1] = 0;
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    switch (d[i].argtype) {
    case eErrht_No:
      errh_LogFatal(0, d[i].str); 
      break;
    case eErrht_Int:
      errh_LogFatal(0, d[i].str, d[i].arg.i); 
      break;
    case eErrht_Sts:
      errh_LogFatal(0, d[i].str, d[i].arg.sts); 
      break;
    case eErrht_Str:
      errh_LogFatal(0, d[i].str, d[i].arg.s); 
      break;
    }
  }
  dcli_translate_filename(fname2, "/tmp/errh2.txt");
  freopen(fname2, "w", stdout);

  fp = fopen(fname, "r");
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    if (dcli_read_line(line, sizeof(line), fp)) {
      if (strcmp(line, d[i].infostr) != 0) {
	m_log->log('E', "LogFatal", i);
	fclose(fp);
	return;
      }
    }
    else {
      m_log->log('E', "LogFatal, no line", i);
      fclose(fp);
      return;
    }
  }
  fclose(fp);

  m_log->log('S', "LogFatal", PWR__SUCCESS);
}

// Constructor
ra_errhtest::ra_errhtest()
{
  pwr_tOid oid;

  m_anix = errh_eAnix_appl1;
  m_log = new tst_log(&m_sts, "rt-Errh", "$pwrp_log/errh.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
  m_sts = gdh_Init("ra_errhtest");
  if (EVEN(m_sts)) {
    m_log->log('F', "errh gdh_Init", m_sts);  
    exit(0);
  }
  m_sts = gdh_GetNodeObject(pwr_cNNid, &oid);
  if (EVEN(m_sts)) {
    m_log->log('F', "errh gdh_GetNodeObject", m_sts);  
    exit(0);
  }
  m_sts = gdh_ObjidToPointer(oid, (void **)&m_nodep);
  if (EVEN(m_sts)) {
    m_log->log('F', "errh gdh_ObjidToPointer", m_sts);  
    exit(0);
  }

}

// Destructor
ra_errhtest::~ra_errhtest()
{
  delete m_log;
}

int main()
{
  ra_errhtest errh;

  errh.Init();
  errh.Success();
  errh.Info();
  errh.Warning();
  errh.Error();
  errh.Fatal();
  errh.CErrLog();
  errh.SetAnix();
  errh.SetStatus();
  errh.LogSuccess();
  errh.LogInfo();
  errh.LogError();
  errh.LogFatal();
}


