/* xtt_jop.cpp -- Handling of java process jop from xtt

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
}

#include "xtt_jop.h"
#include "rt_xnav_msg.h"

Jop::Jop(
	void *jop_parent_ctx) :
	parent_ctx(jop_parent_ctx), command_cb(NULL)
{
  jop_qcom_qid.qix = qcom_cNQix;
  qcom_qid.qix = qcom_cNQix;
  create_que();
  create_process();
}

Jop::~Jop()
{
  close();
}

void Jop::scan()
{
  char *command;
  int sts;

  sts = que_get( &command);
  while ( ODD(sts))
  {
    printf( "Jop received : %s\n", command);
    if ( command_cb)
      (command_cb)( parent_ctx, command);
    que_free( command);
    sts = que_get( &command);
  }
}

int Jop::create_que()
{
  int sts;
  qcom_sQattr attr;
  qcom_qid = qcom_cNQid;

  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  if (!qcom_CreateQ(&sts, &qcom_qid, &attr, "XttCommands")) 
  {
    printf("Failed to create QCOM que %d\n", sts);
    return sts;
  }
  printf( "Qcom que qix : %d, nid: %d\n", qcom_qid.qix, qcom_qid.nid);
  return XNAV__SUCCESS;
}


void Jop::que_free( char *data)
{
  int sts;
  qcom_Free( &sts, data);
}

int Jop::que_get( char **data)
{
  pwr_tStatus sts;
  qcom_sGet get;

  if ( qcom_qid.qix == qcom_cNQix)
    return XNAV__NOTCONNECTED;

  memset( &get, 0, sizeof(get));
  qcom_Get( &sts, &qcom_qid, &get, 0);
  if ( EVEN(sts)) return sts;
  *data = (char *) get.data;
  return XNAV__SUCCESS;
}

int Jop::que_put( char *d)
{
  pwr_tStatus sts;
  qcom_sPut put;
  static char data[200];

  strcpy( data, d);
  printf("qcom_put: %s\n", data);
  put.data = data;
  put.size = strlen(data) + 1;
  put.type.b = qcom_eBtype__;
  put.type.s = qcom_eStype__;
  put.reply.qix = 0;
  put.reply.nid = 0;
 
  if ( jop_qcom_qid.qix == qcom_cNQix)
    return XNAV__NOTCONNECTED;

  qcom_Put( &sts, &jop_qcom_qid, &put);
  return sts;
}

void Jop::set_jop_qid( int qix)
{
  jop_qcom_qid.qix = qix;
  jop_qcom_qid.nid = qcom_qid.nid;
}

int Jop::command( char *command)
{
  printf( "Jop command : %s\n", command);
  return que_put( command);
}

void Jop::close()
{
  printf( "Jop closeing down\n");
  que_put( "exit");
}

void Jop::create_process()
{
#if defined OS_LINUX
  int pid;

  pid = fork();
  if ( !pid)
  {
    char filename[80];
    char qid_str[20];
    char *s;

    if ( (s = getenv( "pwr_exe")) == NULL)
    {
      printf( "** pwr_exe is not defined\n");
      exit(0);
    }
    sprintf( filename, "%s/jop_start.sh", s);
    sprintf( qid_str, "%d", qcom_qid.qix);

    execlp( filename, filename, qid_str, NULL);
    exit(0);
  }
#endif
}
