
#include "statussrv_H.h"
#include "Service.nsmap"
#include "statussrv_Stub.h"
#include "statussrv_utl.h"
#include "co_time.h"
#include "rt_pwr_msg.h"

static struct soap soap;
static int init_done = 0;
static int port = 18084;

pwr_tStatus statussrv_GetStatus( char *nodename, statussrv_sGetStatus *result)
{
  pwr_tStatus sts = PWR__SUCCESS;
  char endpoint[80];

  if ( !init_done) {
    soap_init( &soap);
    init_done = 1;
  }

  sprintf( endpoint, "http://%s:%d", nodename, port);

  _s0__GetStatus get_status;
  _s0__GetStatusResponse get_status_response;

  get_status.ClientRequestHandle = new std::string("StatusSrv Client");

  if ( soap_call___s0__GetStatus( &soap, endpoint, NULL, &get_status, &get_status_response) ==
       SOAP_OK) {

    strncpy( result->Version, get_status_response.Version.c_str(), sizeof(result->Version));
    result->SystemStatus = get_status_response.SystemStatus;
    strncpy( result->SystemStatusStr, get_status_response.SystemStatusStr.c_str(), sizeof(result->SystemStatusStr));
    if ( get_status_response.Description)
      strncpy( result->Description, get_status_response.Description->c_str(), sizeof(result->Description));
    else
      strcpy( result->Description, "");

    if ( get_status_response.SystemTime)
      time_AsciiToA( (char *)get_status_response.SystemTime->c_str(), &result->SystemTime);
    else
      memset( &result->SystemTime, 0, sizeof(result->SystemTime));
  
    if ( get_status_response.BootTime)
      time_AsciiToA( (char *)get_status_response.BootTime->c_str(), &result->BootTime);
    else
      memset( &result->BootTime, 0, sizeof(result->BootTime));
  
    if ( get_status_response.RestartTime)
      time_AsciiToA( (char *)get_status_response.RestartTime->c_str(), &result->RestartTime);
    else
      memset( &result->RestartTime, 0, sizeof(result->RestartTime));
  
    if ( get_status_response.Restarts)
      result->Restarts = *get_status_response.Restarts;
    else
      result->Restarts = -1;

    if ( get_status_response.UserStatus1)
      result->UserStatus[0] = get_status_response.UserStatus1;
    if ( get_status_response.UserStatusStr1)
      strncpy( result->UserStatusStr[0], get_status_response.UserStatusStr1->c_str(), sizeof(result->UserStatusStr[0]));
    else
      strcpy( result->UserStatusStr[0], "");

    if ( get_status_response.UserStatus2)
      result->UserStatus[1] = get_status_response.UserStatus2;
    if ( get_status_response.UserStatusStr2)
      strncpy( result->UserStatusStr[1], get_status_response.UserStatusStr2->c_str(), sizeof(result->UserStatusStr[0]));
    else
      strcpy( result->UserStatusStr[1], "");

    if ( get_status_response.UserStatus3)
      result->UserStatus[2] = get_status_response.UserStatus3;
    if ( get_status_response.UserStatusStr3)
      strncpy( result->UserStatusStr[2], get_status_response.UserStatusStr3->c_str(), sizeof(result->UserStatusStr[0]));
    else
      strcpy( result->UserStatusStr[2], "");

    if ( get_status_response.UserStatus4)
      result->UserStatus[3] = get_status_response.UserStatus4;
    if ( get_status_response.UserStatusStr4)
      strncpy( result->UserStatusStr[3], get_status_response.UserStatusStr4->c_str(), sizeof(result->UserStatusStr[0]));
    else
      strcpy( result->UserStatusStr[3], "");

    if ( get_status_response.UserStatus5)
      result->UserStatus[4] = get_status_response.UserStatus5;
    if ( get_status_response.UserStatusStr5)
      strncpy( result->UserStatusStr[4], get_status_response.UserStatusStr5->c_str(), sizeof(result->UserStatusStr[0]));
    else
      strcpy( result->UserStatusStr[4], "");

  }
  else {
    sts = PWR__SRVCONNECTION;
    memset( result, 0, sizeof(*result));
    result->SystemStatus = sts;
    strcpy( result->SystemStatusStr, "Connection down to server");
  }

  delete get_status.ClientRequestHandle;
  soap_destroy( &soap);
  soap_end( &soap);
  // soap_done( &soap);

  return sts;
}

pwr_tStatus statussrv_GetExtStatus( char *nodename, statussrv_sGetExtStatus *result)
{
  pwr_tStatus sts = PWR__SUCCESS;
  char endpoint[80];

  if ( !init_done) {
    soap_init( &soap);
    init_done = 1;
  }

  sprintf( endpoint, "http://%s:%d", nodename, port);

  _s0__GetExtStatus get_status;
  _s0__GetExtStatusResponse get_status_response;

  get_status.ClientRequestHandle = new std::string("StatusSrv Client");

  if ( soap_call___s0__GetExtStatus( &soap, endpoint, NULL, &get_status, &get_status_response) ==
       SOAP_OK) {

    result->ServerSts[0] = get_status_response.ServerSts1;
    strncpy( result->ServerStsStr[0], get_status_response.ServerSts1Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[0], get_status_response.ServerSts1Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[1] = get_status_response.ServerSts2;
    strncpy( result->ServerStsStr[1], get_status_response.ServerSts2Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[1], get_status_response.ServerSts2Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[2] = get_status_response.ServerSts3;
    strncpy( result->ServerStsStr[2], get_status_response.ServerSts3Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[2], get_status_response.ServerSts3Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[3] = get_status_response.ServerSts4;
    strncpy( result->ServerStsStr[3], get_status_response.ServerSts4Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[3], get_status_response.ServerSts4Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[4] = get_status_response.ServerSts5;
    strncpy( result->ServerStsStr[4], get_status_response.ServerSts5Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[4], get_status_response.ServerSts5Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[5] = get_status_response.ServerSts6;
    strncpy( result->ServerStsStr[5], get_status_response.ServerSts6Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[5], get_status_response.ServerSts6Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[6] = get_status_response.ServerSts7;
    strncpy( result->ServerStsStr[6], get_status_response.ServerSts7Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[6], get_status_response.ServerSts7Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[7] = get_status_response.ServerSts8;
    strncpy( result->ServerStsStr[7], get_status_response.ServerSts8Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[7], get_status_response.ServerSts8Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[8] = get_status_response.ServerSts9;
    strncpy( result->ServerStsStr[8], get_status_response.ServerSts9Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[8], get_status_response.ServerSts9Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[9] = get_status_response.ServerSts10;
    strncpy( result->ServerStsStr[9], get_status_response.ServerSts10Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[9], get_status_response.ServerSts10Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[10] = get_status_response.ServerSts11;
    strncpy( result->ServerStsStr[10], get_status_response.ServerSts11Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[10], get_status_response.ServerSts11Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[11] = get_status_response.ServerSts12;
    strncpy( result->ServerStsStr[11], get_status_response.ServerSts12Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[11], get_status_response.ServerSts12Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[12] = get_status_response.ServerSts13;
    strncpy( result->ServerStsStr[12], get_status_response.ServerSts13Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[12], get_status_response.ServerSts13Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[13] = get_status_response.ServerSts14;
    strncpy( result->ServerStsStr[13], get_status_response.ServerSts14Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[13], get_status_response.ServerSts14Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[14] = get_status_response.ServerSts15;
    strncpy( result->ServerStsStr[14], get_status_response.ServerSts15Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[14], get_status_response.ServerSts15Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[15] = get_status_response.ServerSts16;
    strncpy( result->ServerStsStr[15], get_status_response.ServerSts16Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[15], get_status_response.ServerSts16Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[16] = get_status_response.ServerSts17;
    strncpy( result->ServerStsStr[16], get_status_response.ServerSts17Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[16], get_status_response.ServerSts17Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[17] = get_status_response.ServerSts18;
    strncpy( result->ServerStsStr[17], get_status_response.ServerSts18Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[17], get_status_response.ServerSts18Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[18] = get_status_response.ServerSts19;
    strncpy( result->ServerStsStr[18], get_status_response.ServerSts19Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[18], get_status_response.ServerSts19Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ServerSts[19] = get_status_response.ServerSts20;
    strncpy( result->ServerStsStr[19], get_status_response.ServerSts20Str.c_str(), 
	     sizeof(result->ServerStsStr[0]));
    strncpy( result->ServerStsName[19], get_status_response.ServerSts20Name.c_str(), 
	     sizeof(result->ServerStsName[0]));

    result->ApplSts[0] = get_status_response.ApplSts1;
    strncpy( result->ApplStsStr[0], get_status_response.ApplSts1Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[0], get_status_response.ApplSts1Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[1] = get_status_response.ApplSts2;
    strncpy( result->ApplStsStr[1], get_status_response.ApplSts2Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[1], get_status_response.ApplSts2Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[2] = get_status_response.ApplSts3;
    strncpy( result->ApplStsStr[2], get_status_response.ApplSts3Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[2], get_status_response.ApplSts3Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[3] = get_status_response.ApplSts4;
    strncpy( result->ApplStsStr[3], get_status_response.ApplSts4Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[3], get_status_response.ApplSts4Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[4] = get_status_response.ApplSts5;
    strncpy( result->ApplStsStr[4], get_status_response.ApplSts5Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[4], get_status_response.ApplSts5Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[5] = get_status_response.ApplSts6;
    strncpy( result->ApplStsStr[5], get_status_response.ApplSts6Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[5], get_status_response.ApplSts6Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[6] = get_status_response.ApplSts7;
    strncpy( result->ApplStsStr[6], get_status_response.ApplSts7Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[6], get_status_response.ApplSts7Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[7] = get_status_response.ApplSts8;
    strncpy( result->ApplStsStr[7], get_status_response.ApplSts8Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[7], get_status_response.ApplSts8Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[8] = get_status_response.ApplSts9;
    strncpy( result->ApplStsStr[8], get_status_response.ApplSts9Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[8], get_status_response.ApplSts9Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[9] = get_status_response.ApplSts10;
    strncpy( result->ApplStsStr[9], get_status_response.ApplSts10Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[9], get_status_response.ApplSts10Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[10] = get_status_response.ApplSts11;
    strncpy( result->ApplStsStr[10], get_status_response.ApplSts11Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[10], get_status_response.ApplSts11Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[11] = get_status_response.ApplSts12;
    strncpy( result->ApplStsStr[11], get_status_response.ApplSts12Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[11], get_status_response.ApplSts12Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[12] = get_status_response.ApplSts13;
    strncpy( result->ApplStsStr[12], get_status_response.ApplSts13Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[12], get_status_response.ApplSts13Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[13] = get_status_response.ApplSts14;
    strncpy( result->ApplStsStr[13], get_status_response.ApplSts14Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[13], get_status_response.ApplSts14Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[14] = get_status_response.ApplSts15;
    strncpy( result->ApplStsStr[14], get_status_response.ApplSts15Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[14], get_status_response.ApplSts15Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[15] = get_status_response.ApplSts16;
    strncpy( result->ApplStsStr[15], get_status_response.ApplSts16Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[15], get_status_response.ApplSts16Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[16] = get_status_response.ApplSts17;
    strncpy( result->ApplStsStr[16], get_status_response.ApplSts17Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[16], get_status_response.ApplSts17Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[17] = get_status_response.ApplSts18;
    strncpy( result->ApplStsStr[17], get_status_response.ApplSts18Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[17], get_status_response.ApplSts18Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[18] = get_status_response.ApplSts19;
    strncpy( result->ApplStsStr[18], get_status_response.ApplSts19Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[18], get_status_response.ApplSts19Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

    result->ApplSts[19] = get_status_response.ApplSts20;
    strncpy( result->ApplStsStr[19], get_status_response.ApplSts20Str.c_str(), 
	     sizeof(result->ApplStsStr[0]));
    strncpy( result->ApplStsName[19], get_status_response.ApplSts20Name.c_str(), 
	     sizeof(result->ApplStsName[0]));

  }
  else {
    sts = PWR__SRVCONNECTION;
    memset( result, 0, sizeof(*result));
  }
  delete get_status.ClientRequestHandle;
  soap_destroy( &soap);
  soap_end( &soap);
  // soap_done( &soap);

  return sts;
}

pwr_tStatus statussrv_Restart( char *nodename)
{
  pwr_tStatus sts = PWR__SUCCESS;
  char endpoint[80];

  if ( !init_done) {
    soap_init( &soap);
    init_done = 1;
  }

  sprintf( endpoint, "http://%s:%d", nodename, port);

  _s0__Restart restart;
  _s0__RestartResponse restart_response;

  restart.ClientRequestHandle = new std::string("StatusSrv Client");

  if ( soap_call___s0__Restart( &soap, endpoint, NULL, &restart, &restart_response) ==
       SOAP_OK) {
  }
  else {
    sts = PWR__SRVCONNECTION;
  }
  soap_destroy( &soap);
  soap_end( &soap);

  return sts;
}

pwr_tStatus statussrv_XttStart( char *nodename, char *opplace, char *lang, char *display,
				char *gui)
{
  pwr_tStatus sts = PWR__SUCCESS;
  char endpoint[80];

  if ( !init_done) {
    soap_init( &soap);
    init_done = 1;
  }

  sprintf( endpoint, "http://%s:%d", nodename, port);

  _s0__XttStart xtt_start;
  _s0__XttStartResponse xtt_start_response;

  xtt_start.ClientRequestHandle = new std::string("StatusSrv Client");
  if ( opplace && strcmp( opplace, "") != 0)
    xtt_start.OpPlace = new std::string(opplace);
  if ( lang && strcmp( lang, "") != 0)
    xtt_start.Language = new std::string(lang);
  if ( display && strcmp( display, "") != 0)
    xtt_start.Display = new std::string(display);
  if ( gui && strcmp( gui, "") != 0)
    xtt_start.GUI = new std::string(gui);

  if ( soap_call___s0__XttStart( &soap, endpoint, NULL, &xtt_start, &xtt_start_response) ==
       SOAP_OK) {
  }
  else {
    sts = PWR__SRVCONNECTION;
  }

  delete xtt_start.ClientRequestHandle;
  if ( xtt_start.OpPlace)
    delete xtt_start.OpPlace;
  if ( xtt_start.Language)
    delete xtt_start.Language;
  if ( xtt_start.Display)
    delete xtt_start.Display;
  if ( xtt_start.GUI)
    delete xtt_start.GUI;
  soap_destroy( &soap);
  soap_end( &soap);

  return sts;
}

pwr_tStatus statussrv_RtMonStart( char *nodename, char *lang, char *display, char *gui)
{
  pwr_tStatus sts = PWR__SUCCESS;
  char endpoint[80];

  if ( !init_done) {
    soap_init( &soap);
    init_done = 1;
  }

  sprintf( endpoint, "http://%s:%d", nodename, port);

  _s0__RtMonStart rtmon_start;
  _s0__RtMonStartResponse rtmon_start_response;

  rtmon_start.ClientRequestHandle = new std::string("StatusSrv Client");
  if ( lang && strcmp( lang, "") != 0)
    rtmon_start.Language = new std::string(lang);
   if ( display && strcmp( display, "") != 0)
    rtmon_start.Display = new std::string(display);
   if ( gui && strcmp( gui, "") != 0)
    rtmon_start.GUI = new std::string(gui);

  if ( soap_call___s0__RtMonStart( &soap, endpoint, NULL, &rtmon_start, &rtmon_start_response) ==
       SOAP_OK) {
  }
  else {
    sts = PWR__SRVCONNECTION;
  }
  delete rtmon_start.ClientRequestHandle;
  if ( rtmon_start.Language)
    delete rtmon_start.Language;
  if ( rtmon_start.Display)
    delete rtmon_start.Display;
  if ( rtmon_start.GUI)
    delete rtmon_start.GUI;
  soap_destroy( &soap);
  soap_end( &soap);

  return sts;
}

