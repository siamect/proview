/* rt_errl.c -- Logging module

   PROVIEW/R
   Copyright (C) 1998 by Mandator AB.

   <Description> Handles logging for ELN.  
*/

#include $vaxelnc
#include $kerneldef
#include $mutex
#include descrip
#include errno
#include "rt_errh_msg.h"
#include "rt_errl.h"
#include "rt_errh.h"

static PORT	log_port;	/* The port object where error logs are received */
static NAME	port_name;	/* The name object for that port */
static char	term_name[256];	/* This is the name of the log terminal currently
				   active. A zero length name indicates no terminal. */
static char	file_name[256];	/* This is the name of the logfile currently active.
				   A zero length name indicates no file. */
static MUTEX	term_mutex;
static MUTEX	file_mutex;	/* Locked during transients on the logfile */
static FILE	*term;		/* The actual file variable to the log terminal */
static FILE	*file;		/* The actual file variable to the log file */
static EVENT	log_exit;	/* signalled when the test wants to exit */
static int	term_error = 0; 
static int	file_error = 0; 

static void log_thread (void);
static pwr_tBoolean file_dead(struct chf$signal_array *sp, struct chf$mech_array *mp);
static pwr_tBoolean term_dead(struct chf$signal_array *sp, struct chf$mech_array *mp);
static void write_file(char *s, int len);
static void write_term(char *s, int len);


/* Initialize the error logger.

   The procedure creates a port (log_port), assigns a 
   name to it (port_name) and finally creates the logging process.

   If 'termname' is nonzero, it will be opened and used as
   log terminal.  */

void
errl_Init (
  const char	*termname
)
{
  int		priority = 0;
  pwr_tStatus	sts;	
  PROCESS	process;
  $DESCRIPTOR(dsc, "ERR_LOG_PORT");
  static int initDone = 0;

  if (initDone)
    return;

  ELN$CREATE_MUTEX(term_mutex, &sts);
  ELN$CREATE_MUTEX(file_mutex, &sts);
  term_name[0] = '\0';
  file_name[0] = '\0';

  ker$create_event(&sts, &log_exit, EVENT$CLEARED);

  ker$create_port(&sts, &log_port, 40);
  ker$create_name(&sts, &port_name, &dsc, &log_port, NAME$LOCAL);
  ker$create_process(&sts, &process, log_thread, NULL, NULL);
  ker$set_process_priority(&sts, process, priority);

  errl_SetTerm(termname);
          
}

/* Change the current log terminal.
   The log terminal name is recorded globally in 
   this module in the vairable term_name. Since I/O operation
   might be in progress the 'term_mutex' is locked before 
   any change is done to the terminal.

   If there exists an open log terminal (term_name is nonzero)
   that terminal is closed. If the parameter to this procedure is
   nonzero, it's used as a file name to the new log terminal to 
   open. A message is logged to inform the operator about the 
   new  log terminal.  */

void
errl_SetTerm (
  char *termname
)
{
  pwr_tStatus sts;
  int len = 0;

  /* Close the current log terminal, if any */

  if (term_name[0] != '\0') {
    errh_CErrLog(ERRH__OLDTERM, errh_ErrArgAF(term_name), NULL);		
    ELN$LOCK_MUTEX(term_mutex);
    fclose(term);
    term_name[0] = '\0';
    ELN$UNLOCK_MUTEX(term_mutex);
  }

  if (termname != NULL)
    len = strlen(termname);

  if (len > 0) {
    ELN$LOCK_MUTEX(term_mutex);
      term = fopen(termname, "w+", "shr=get");
      if (term == NULL) {
	errh_CErrLog(ERRH__BADTERM, errh_ErrArgAF(termname), errh_ErrArgMsg(vaxc$errno), NULL);			
      } else {
	strncpy(term_name, termname, MIN(sizeof(term_name), len));
	errh_CErrLog(ERRH__NEWTERM, errh_ErrArgAF(term_name), NULL);		
      }
    ELN$UNLOCK_MUTEX(term_mutex);
  }
}

/* Terminate the error logger in an orderly way.  */

pwr_tStatus
errl_Exit ()
{
  pwr_tStatus sts;

  ker$signal(&sts, log_exit);

  return sts;
}

/* Conditionally close and (re-)open the log file.

   The procedure is called by system to change the current 
   log file. The log file name is recorded globally in this 
   module in the variable 'file_name'. Since I/O operation 
   might be in progress the 'file_mutex' is locked before 
   any change is done to the file.

   If there exists an open logfile ('file_name' is nonzero)
   that file is closed. If the parameter to this procedure is
   nonzero, use that as a file name to the new log file to 
   open. A message is logged to inform the operator about the 
   new  log file.	

   If no 'filename' is given there will be no open log file.  */

void
errl_SetFile (
  const char	*filename
)
{
  pwr_tStatus sts;
  int len = strlen(filename);

  ELN$LOCK_MUTEX(file_mutex);

  fclose(file);
  strcpy(file_name, filename);

  if (len == 0) {
    ELN$UNLOCK_MUTEX(file_mutex);
    return;
  }

  file = fopen(file_name, "w+", "shr=get");
  if (file == NULL) {
    errh_Error("Setting logfile to '%s'\n%s", filename, strerror(errno, vaxc$errno));
  } else {
    errh_Info("Logging to '%s'", filename);
  }

  ELN$UNLOCK_MUTEX(file_mutex);
}

/* This process is created by a errl_Init call
   which is called during init time. Although not passed 
   as parameters the 'file_mutex' and 'log_port' 
   variables must have valid contents (they are set by 
   errl_Init before this process is created).

   The process waits for a message to arrive on the port 
   and does some validation of the message. If ok, it sends 
   the message to the screen and to the error log file 
   before disposing it.  */

static void
log_thread ()
{
  char *s;
  MESSAGE mid;
  pwr_tStatus sts;
  int wait_result;
  int size;

  for (;;) {
    ker$wait_any(&sts, &wait_result, NULL, &log_port, log_exit);
    if (wait_result != 1)
      break;

    ker$receive(&sts, &mid, &s, &size, &log_port, NULL, NULL);
    if (EVEN(sts)) {
      char string[256];
      s = errh_Message(string, 'E', "ker$receive\n%m", sts);
      write_term(s, strlen(s));
      write_file(s, strlen(s));
      continue;
    }

    write_term(s, size);
    write_file(s, size);
    ker$delete(&sts, mid);
  }
}

/* Exeption handler for write fault on the terminal.  */

static pwr_tBoolean
term_dead (
  struct chf$signal_array *sp,
  struct chf$mech_array *mp
)
{

  term_error = 1;

  ker$unwind(NULL, 1, NULL);			

  return FALSE;
}

/* Exeption handler for write fault on log file.  */

static pwr_tBoolean
file_dead (
  struct chf$signal_array *sp,
  struct chf$mech_array *mp
)
{

  file_error = 1;

  ker$unwind(NULL, 1, NULL);			

  return FALSE;
}

/* The routine is used internally in LOGMOD to send the 
   first line of an error text printout to the screen package. 
   Hence the message will appear on the error line on the 
   Operator's screen.  */

static void
write_term (
  char *s,
  int len
)
{
  int size = len;

  if (term_error)
    return;

  if (s[len - 1] != '\n') {
    s[len] = '\n';
    ++size;
  }

  ELN$LOCK_MUTEX(term_mutex);
    vaxc$establish(term_dead);
    fwrite(s, size, 1, term);
    fflush(term);
  ELN$UNLOCK_MUTEX(term_mutex);

  if (s[len - 1] != '\n')
    s[len] = '\0';
}

/* Write to the error log file. The file mutex 
   'file_mutex' is locked before any operation takes 
   place to synchronize operations on the file. If there 
   is no logfile open, no processing takes place. If a file 
   exists, the contents of the error log message is formatted 
   and written to the file.  */

static void
write_file (
  char *s,
  int len
)
{
  int size = len;

  if (file_error)
    return;

  if (s[len - 1] != '\n') {
    s[len] = '\n';
    ++size;
  }

  ELN$LOCK_MUTEX(file_mutex);
    if (strlen(file_name) > 0) { 
      vaxc$establish(file_dead);
      fwrite(s, size, 1, file);
      fflush(file);
    }
  ELN$UNLOCK_MUTEX(file_mutex);

  if (s[len - 1] != '\n')
    s[len] = '\0';
}
