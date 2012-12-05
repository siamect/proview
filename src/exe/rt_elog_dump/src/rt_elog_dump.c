/* rt_elog_dump.c -- <short description>

   Author Jonas Nylund.

   Dumps the rt_eventlog.db to a file.
   If the eventlog doesn't exist a errormess will appear.
   */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <db.h>
#include "co_time.h"
#include "rt_mh_def.h"
#include "co_dcli.h"
#include "rt_elog.h"

void ReadFromDBAndPutInFile(FILE *outFile);

void Init(char *fname);

void WriteColumnNames(FILE *outFile);

int Write(sEvent *sp, FILE *outFile);

void printMess(sEvent *sp, FILE *outFile);
void printAck(sEvent *sp, FILE *outFile);
void printRet(sEvent *sp, FILE *outFile);
void printBlock(sEvent *sp, FILE *outFile);
void printMsgInfo(mh_sMsgInfo *mp, FILE *outFile);


void printEventFlags(FILE *outFile, int flags);

void convertEventType(int type, char *ret);

void convertEventPrio(int prio, char *ret);

DB *dataBaseP = NULL;

int 
main (int argc, char *argv[])
{
  FILE *outFile;
  pwr_tInt32 ret;
  if(argc == 3)
  {
    if(argv[2] != NULL)
      outFile = fopen(argv[2], "w");
  }
  else
    outFile = fopen(DUMPFILE, "w");

  if(outFile == NULL)
  {
    printf("kunde ej skapa fil, avslutar");
    exit(1);
  }
  if(argc > 1)
    Init(argv[1]);
  else
    Init(DATABASE);
  ReadFromDBAndPutInFile(outFile);
  fclose(outFile);
  if((ret = dataBaseP->close(dataBaseP,0) != 0))
  {
    printf("error db_close: %s\n", db_strerror(ret));
  }
  if(argc == 3)
    printf("Programmet klart resultatet finns i filen %s\n", argv[2]);
  else
    printf("Programmet klart resultatet finns i filen %s\n", DUMPFILE);
  return 0;

}

void
Init (char *fname)
{
  pwr_tInt32 ret;
  char dbName[200];
  
  dcli_translate_filename( dbName, fname);

  /*create the database if it's not already created*/
  if((ret = db_create(&dataBaseP, NULL, 0)) != 0)
  {
    /*error creating db-handle send the mess to errh, then exit*/
    printf("error db_create: %s\n", db_strerror(ret));
    printf(" Fel vid skapande av databashandtag avslutar\n");
    exit(1);
  }

#if (DB_VERSION_MAJOR > 3) && (DB_VERSION_MINOR > 0)
  ret = dataBaseP->open(dataBaseP, NULL, dbName, NULL, DATABASETYPE, DB_RDONLY, 0);
#else
  ret = dataBaseP->open(dataBaseP, dbName, NULL, DATABASETYPE, DB_RDONLY, 0);
#endif


  if(ret != 0)
  {
    /*error opening/creating db send the mess to errh, then exit*/
    printf("error db_open: %s\n", db_strerror(ret));
    exit(1);
  }
}


void ReadFromDBAndPutInFile(FILE *outFile)
{
  pwr_tInt32 ret, sts;
  pwr_tUInt32 nrOfEvents = 0;
  sEvent *eventp;
  DBT data, key;
  DBC *dbcp;
  
  WriteColumnNames(outFile);
  
  /* Acquire a cursor for the database. */ 
  if ((ret = dataBaseP->cursor(dataBaseP, NULL, &dbcp, 0)) != 0) 
  {
    printf("error dataBaseP->cursor: %s\n", db_strerror(ret)); 
    exit(1);
  }

  /* Initialize the key/data return pair. */
  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));
  
  if((ret = dbcp->c_get(dbcp, &key, &data, DB_FIRST)) == 0)
  {
    eventp = data.data;
    sts = Write(eventp, outFile);
    nrOfEvents++;
  }
  
  while((ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT)) == 0)
  {
    eventp = data.data;
    sts = Write(eventp, outFile);
    nrOfEvents++;
  }
  if(ret != DB_NOTFOUND)
  {
    printf("error dbcp->c_get: %s\n", db_strerror(ret));
    printf("Fel vid försök att läsa post nr %u, avslutar\n", nrOfEvents);
    exit(1);
  }
  printf("Antal händelser skrivna till fil: %u\n", nrOfEvents);
  /*Close the cursor*/
  if((ret = dbcp->c_close(dbcp)) != 0)
  {
    printf("Error dbcp->c_close(): %s\n", db_strerror(ret));
  }       
}  
  
void WriteColumnNames(FILE *outFile)
{
  fprintf(outFile, "Nid\t");
  fprintf(outFile, "BirthTime\t");
  fprintf(outFile, "Idx\t");
  fprintf(outFile, "EventFlags\t");
  fprintf(outFile, "EventTime\t");
  fprintf(outFile, "EventName\t");
  fprintf(outFile, "EventType\t");
  fprintf(outFile, "EventPrio\t");
  fprintf(outFile, "EventText\t");
  //fprintf(outFile, "Status\t");
  fprintf(outFile, "TargetIdx\t");
  fprintf(outFile, "DetectTime\t");
  fprintf(outFile, "AValue\t");
  fprintf(outFile, "CtrlLimit\t");
  fprintf(outFile, "Hysteres\t");
  fprintf(outFile, "Unit\t");
  fprintf(outFile, "DValue\t");
  fprintf(outFile, "High\t");
  fprintf(outFile, "\n");
}

int
Write (
  sEvent *sp,
  FILE *outFile
)
{
  switch (sp->EventType) {
  case mh_eEvent_Alarm:
  case mh_eEvent_Info:
    printMess(sp, outFile);
    break;
  case mh_eEvent_Ack:
    printAck(sp, outFile);
    break;
  case mh_eEvent_Cancel:
  case mh_eEvent_Return:
    printRet(sp, outFile);
    break;
  case mh_eEvent_Block:
  case mh_eEvent_Unblock:
  case mh_eEvent_Reblock:
  case mh_eEvent_CancelBlock:
    printBlock(sp, outFile);
    break;
  default:
    printf("rt_elog_dump: Error in Write unknown EventType");
    break;
  }  
  fprintf(outFile, "\n");
  return 1;
}

void
printMess (
  sEvent *sp,
  FILE *outFile
)
{
  printMsgInfo(&(sp->Mess.message.Info), outFile);


  //EventText
  //Status
  //TargetIdx
  //DetectTime
  //AValue
  //CtrlLimit
  //Hysteres
  //Unit
  //DValue
  //High
  fprintf(outFile, "%s\t", sp->Mess.message.EventText);
  fprintf(outFile,"\t");
  fprintf(outFile,"\t");
  switch (sp->Mess.message.SupInfo.SupType) {
    case mh_eSupType_Analog:
      fprintf(outFile, "%.2f\t", sp->Mess.message.SupInfo.mh_uSupInfo_u.A.ActualValue);
      fprintf(outFile, "%.2f\t", sp->Mess.message.SupInfo.mh_uSupInfo_u.A.CtrlLimit);
      fprintf(outFile, "%.2f\t", sp->Mess.message.SupInfo.mh_uSupInfo_u.A.Hysteres);
      fprintf(outFile, "%s\t",   sp->Mess.message.SupInfo.mh_uSupInfo_u.A.Unit);
      fprintf(outFile,"\t");
      fprintf(outFile, "%u\t", sp->Mess.message.SupInfo.mh_uSupInfo_u.A.High);
      break;
    case mh_eSupType_Digital:
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile, "%u\t", sp->Mess.message.SupInfo.mh_uSupInfo_u.D.ActualValue);
      fprintf(outFile, "%u\t", sp->Mess.message.SupInfo.mh_uSupInfo_u.D.High);
      break;
    case mh_eSupType__:  
    case mh_eSupType_None:
    case mh_eSupType_Link:
    case mh_eSupType_:
      break;  
  }
}

void
printAck (
  sEvent *sp,
  FILE *outFile
)
{
  char time_str[40];
  pwr_tTime event_time;
  
  printMsgInfo(&(sp->Mess.ack.Info), outFile);
  
  //EventText
  //Status
  //TargetIdx
  //DetectTime
  //AValue
  //CtrlLimit
  //Hysteres
  //Unit
  //DValue
  //High

  fprintf(outFile,"\t");
  fprintf(outFile, "%u\t", sp->Mess.ack.TargetId.Idx);

  event_time = net_NetTimeToTime(&(sp->Mess.ack.DetectTime));    
  time_AtoAscii(&event_time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  fprintf(outFile, "%s\t", time_str);

  switch (sp->Mess.ack.SupInfo.SupType) {
    case mh_eSupType_Analog:
      fprintf(outFile, "%.2f\t", sp->Mess.ack.SupInfo.mh_uSupInfo_u.A.ActualValue);
      fprintf(outFile, "%.2f\t", sp->Mess.ack.SupInfo.mh_uSupInfo_u.A.CtrlLimit);
      fprintf(outFile, "%.2f\t", sp->Mess.ack.SupInfo.mh_uSupInfo_u.A.Hysteres);
      fprintf(outFile, "%s\t",   sp->Mess.ack.SupInfo.mh_uSupInfo_u.A.Unit);
      fprintf(outFile,"\t");
      fprintf(outFile, "%u\t", sp->Mess.ack.SupInfo.mh_uSupInfo_u.A.High);
      break;
    case mh_eSupType_Digital:
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile, "%u\t", sp->Mess.ack.SupInfo.mh_uSupInfo_u.D.ActualValue);
      fprintf(outFile, "%u\t", sp->Mess.ack.SupInfo.mh_uSupInfo_u.D.High);
      break;    
    case mh_eSupType__:  
    case mh_eSupType_None:
    case mh_eSupType_Link:
    case mh_eSupType_:
      break;  
  }
}

void
printRet (
  sEvent *sp,
  FILE *outFile
)
{
  char time_str[40];
  printMsgInfo(&(sp->Mess.ret.Info), outFile);
  pwr_tTime event_time;
  
  //EventText
  //Status
  //TargetIdx
  //DetectTime
  //AValue
  //CtrlLimit
  //Hysteres
  //Unit
  //DValue
  //High
  fprintf(outFile, "%s\t", sp->Mess.ret.EventText);
  fprintf(outFile, "%u\t", sp->Mess.ret.TargetId.Idx);

  event_time = net_NetTimeToTime(&(sp->Mess.ret.DetectTime));    
  time_AtoAscii( &event_time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  fprintf(outFile, "%s\t", time_str);

  switch (sp->Mess.ret.SupInfo.SupType) {
    case mh_eSupType_Analog:
      fprintf(outFile, "%.2f\t", sp->Mess.ret.SupInfo.mh_uSupInfo_u.A.ActualValue);
      fprintf(outFile, "%.2f\t", sp->Mess.ret.SupInfo.mh_uSupInfo_u.A.CtrlLimit);
      fprintf(outFile, "%.2f\t", sp->Mess.ret.SupInfo.mh_uSupInfo_u.A.Hysteres);
      fprintf(outFile, "%s\t",   sp->Mess.ret.SupInfo.mh_uSupInfo_u.A.Unit);
      fprintf(outFile,"\t");
      fprintf(outFile, "%u\t", sp->Mess.ret.SupInfo.mh_uSupInfo_u.A.High);
      break;
    case mh_eSupType_Digital:
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile,"\t");
      fprintf(outFile, "%u\t", sp->Mess.ret.SupInfo.mh_uSupInfo_u.D.ActualValue);
      fprintf(outFile, "%u\t", sp->Mess.ret.SupInfo.mh_uSupInfo_u.D.High);
      break;  
    case mh_eSupType__:  
    case mh_eSupType_None:
    case mh_eSupType_Link:
    case mh_eSupType_:
      break;  
  
  }

}

void
printBlock (
  sEvent *sp,
  FILE *outFile
)
{
  char time_str[40];
  pwr_tTime event_time;

    printMsgInfo(&(sp->Mess.block.Info), outFile);
  //EventText
  //Status
  //TargetIdx
  //DetectTime
  //AValue
  //CtrlLimit
  //Hysteres
  //Unit
  //DValue
  //High
  fprintf(outFile,"\t");
  fprintf(outFile, "%u\t", sp->Mess.block.TargetId.Idx);

  event_time = net_NetTimeToTime( &(sp->Mess.block.DetectTime));        
  time_AtoAscii(&event_time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  fprintf(outFile, "%s\t", time_str);
    
  fprintf(outFile,"\t");
  fprintf(outFile,"\t");
  fprintf(outFile,"\t");
  fprintf(outFile,"\t");
  fprintf(outFile,"\t");
  fprintf(outFile,"\t");
}

void
printMsgInfo (
  mh_sMsgInfo *mp,
  FILE *outFile
)
{
  char time_str[40];
  pwr_tTime event_time;
  
  fprintf(outFile, "%d\t", mp->Id.Nix);
  
  event_time = net_NetTimeToTime(&(mp->Id.BirthTime));        
  time_AtoAscii(&event_time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  fprintf(outFile, "%s\t", time_str);
  
  fprintf(outFile, "%d\t",mp->Id.Idx);
  /*
  sp->Object
  sp->SupObject
  */
  printEventFlags(outFile, mp->EventFlags);
  fprintf(outFile, "\t");

  event_time = net_NetTimeToTime(&(mp->EventTime));      
  time_AtoAscii( &event_time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  fprintf(outFile, "%s\t", time_str);
  
  fprintf(outFile, "%s\t",mp->EventName_V3);
  
  convertEventType(mp->EventType, time_str);
  fprintf(outFile, "%s\t", time_str);
  
  convertEventPrio(mp->EventPrio, time_str);
  fprintf(outFile, "%s\t",time_str);
}


void printEventFlags(FILE *outFile, int flags)
{
  if(flags & mh_mEventFlags_Return)
  {
    fprintf(outFile,"Return/");
  }
  if(flags & mh_mEventFlags_Ack)
  {
    fprintf(outFile,"Ack/");
  }
  if(flags & mh_mEventFlags_Bell)
  {
    fprintf(outFile,"Bell/");
  }
  if(flags & mh_mEventFlags_Force)
  {
    fprintf(outFile,"Force/");
  }
  if(flags & mh_mEventFlags_InfoWindow)
  {
    fprintf(outFile,"InfoWind");
  }
  if(flags & mh_mEventFlags_Returned)
  {
    fprintf(outFile,"Returned/");
  }
  if(flags & mh_mEventFlags_NoObject)
  {
    fprintf(outFile,"NoObject");
  }
}



void convertEventType(int type, char *ret)
{
  switch (type) {
  case mh_eEvent_Alarm:
    strcpy(ret, "Alarm");
    break;
  case mh_eEvent_Info:
    strcpy(ret, "Info");
    break;
  case mh_eEvent_Ack:
    strcpy(ret, "Ack");
    break;
  case mh_eEvent_Cancel:
    strcpy(ret, "Cancel");
    break;
  case mh_eEvent_Return:
    strcpy(ret, "Return");
    break;
  case mh_eEvent_Block:
    strcpy(ret, "Block");
    break;
  case mh_eEvent_Unblock:
    strcpy(ret, "Unblock");
    break;
  case mh_eEvent_Reblock:
    strcpy(ret, "Reblock");
    break;
  case mh_eEvent_CancelBlock:
    strcpy(ret, "CancelBlock");
    break;
  default:
    strcpy(ret, " ");
    break;
  }
}  

void convertEventPrio(int prio, char *ret)
{
  switch (prio) {
  case mh_eEventPrio_A:
    strcpy(ret, "A");
    break;
  case mh_eEventPrio_B:
    strcpy(ret, "B");
    break;
  case mh_eEventPrio_C:
    strcpy(ret, "C");
    break;
  case mh_eEventPrio_D:
    strcpy(ret, "D");
    break;
  default:
    strcpy(ret, " ");
    break;
  }
}  

