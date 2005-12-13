/* 
 * Proview   $Id: jpwr_rt_mh.c,v 1.7 2005-12-13 15:11:11 claes Exp $
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


#include <string.h>
#include <stdio.h>
#include "jpwr_rt_mh.h"
#include "pwr.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "rt_mh_util.h"

#include "rt_gdh.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_cdh_msg.h"
#include "rt_gdh_msg.h"

pwr_tStatus ev_mh_ack_bc( mh_sAck *MsgP);
pwr_tStatus ev_mh_return_bc( mh_sReturn *MsgP);
pwr_tStatus ev_mh_alarm_bc( mh_sMessage *MsgP);
pwr_tStatus ev_mh_info_bc( mh_sMessage *MsgP);
pwr_tStatus ev_mh_clear_alarmlist_bc( pwr_tNodeIndex nix);

jclass Mh_id;
jmethodID Mh_messReceived_id;
JavaVM *jvm;

JNIEXPORT void JNICALL Java_jpwr_rt_Mh_initIDs
  (JNIEnv *env, jclass cls)
{
  int sts;

  //initiera alla pekare till javametoderna som behövs
  Mh_id = (*env)->FindClass( env, "jpwr/rt/Mh");
  if(Mh_id == NULL)
  {
    printf("jpwr_rt_mh.c: couldnt find jpwr/rt/Mh class\n");
  }
  //hämta pekare till staticmetoden messReceived
  Mh_messReceived_id = (*env)->GetStaticMethodID( env, Mh_id, "messReceived",                              
  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIIILjava/lang/String;IILjava/lang/String;IILjpwr/rt/PwrtObjid;)V");
  sts = (*env)->GetJavaVM(env, &jvm);
  if(sts)
  {
    printf("Hittar ej JavaVM\n");
  }
}

JNIEXPORT jobject JNICALL Java_jpwr_rt_Mh_outunitAck
  (JNIEnv *env, jclass obj, jobject id)
{
  int sts = 1;

  //char *str;
  char *str_copy;
  mh_sEventId eventId;
  jfieldID nix_fid;
  jfieldID idx_fid;
  jfieldID birthTime_fid;
  jclass mhrsEventId_class;

  jclass pwrtStatus_id = NULL;
  static jmethodID pwrtStatus_cid = NULL;

  jint jsts;  
  jobject return_obj;

  jstring jstr;
  
  pwrtStatus_id = (*env)->FindClass( env, "jpwr/rt/PwrtStatus");
  if(pwrtStatus_id == NULL)
  {
    printf("fel vid PwrtStatus classidhämtning\n");
    return NULL;
  }
  
  if(pwrtStatus_cid == NULL)
  {

    //hämta upp classid och sedan pekare till konstruktorn för PwrtStatus
    pwrtStatus_cid = (*env)->GetMethodID( env, pwrtStatus_id,
    	                                  "<init>", "(I)V");  
    if(pwrtStatus_cid == NULL)
    {
      printf("fel vid PwrtStatus methodidhämtning\n");
      return NULL;
    }
  }
  
  //hämta upp classid till MhrsEventId-objektet
  mhrsEventId_class =(*env)->GetObjectClass(env,id);
  
  nix_fid =(*env)->GetFieldID(env,mhrsEventId_class,"nix", "I");
  if (nix_fid ==NULL)
  {
    printf("hittade inte nix\n");
    sts = -2;//lyckades inte hitta fältet
  }
  
  //printf("efter nix hämtning\n");
  
  idx_fid =(*env)->GetFieldID(env,mhrsEventId_class,"idx", "I");
  if (idx_fid ==NULL)
  {
    printf("hittade inte idx\n");
    sts = -2;//lyckades inte hitta fältet
  }
  
  //printf("efter idx hämtning\n");
  
  birthTime_fid =(*env)->GetFieldID(env,mhrsEventId_class,"birthTime",
                                    "Ljava/lang/String;");
  if (birthTime_fid ==NULL)
  {
    printf("hittade inte birthTime\n");
    sts = -2;//lyckades inte hitta fältet
  }
  
  eventId.Nix = (pwr_tNodeIndex)(*env)->GetIntField(env,id,nix_fid);
  eventId.Idx = (pwr_tUInt32)(*env)->GetIntField(env,id,idx_fid);
    
  jstr = (*env)->GetObjectField(env,id,birthTime_fid);
  str_copy = (char *)(*env)->GetStringUTFChars(env,jstr,NULL);

  if (str_copy ==NULL)
  {
    sts = -1;//out of memory
  }
  
  time_AsciiToA(str_copy, &eventId.BirthTime);
  
  sts = mh_OutunitAck(&eventId);
  (*env)->ReleaseStringUTFChars(env,jstr,str_copy);
  
  jsts = (jint) sts; 
  //skapa returobjekt
  return_obj = (*env)->NewObject( env, pwrtStatus_id,
  	pwrtStatus_cid, jsts);
  
  (*env)->DeleteLocalRef(env,pwrtStatus_id);
  return return_obj;
}





JNIEXPORT jobject JNICALL Java_jpwr_rt_Mh_outunitConnect
  (JNIEnv *env, jclass obj, jobject objid_obj)
{
  int		sts;
  jclass 	PwrtObjid_id;
  jmethodID 	PwrtObjid_getOix;
  jmethodID 	PwrtObjid_getVid;
  jmethodID 	PwrtObjid_cid;
  pwr_tObjid 	user;
  jclass 	pwrtStatus_id;
  jmethodID 	pwrtStatus_cid;
  jobject 	return_obj;
  jint 		jsts;

  //hämta upp classid och sedan pekare till konstruktorn för PwrtStatus
  pwrtStatus_id = (*env)->FindClass( env, "jpwr/rt/PwrtStatus");
  pwrtStatus_cid = (*env)->GetMethodID( env, pwrtStatus_id,
    	"<init>", "(I)V");
  
  //hämta upp classid och sedan pekare till konstruktorn för PwrtObjid
  PwrtObjid_id = (*env)->FindClass( env, "jpwr/rt/PwrtObjid");
  if(PwrtObjid_id == NULL) printf("Pwrtobjid_id är NULL");
  
  PwrtObjid_cid = (*env)->GetMethodID( env, PwrtObjid_id, "<init>", "(II)V");
  if(PwrtObjid_cid == NULL) printf("Pwrtobjid_cid är NULL");
  
  //hämta pekare till getOix och getVid i PwrtObjid    
  PwrtObjid_getOix = (*env)->GetMethodID( env, PwrtObjid_id, "getOix", "()I");
  if(PwrtObjid_getOix == NULL) printf("Pwrtobjid_getoix är NULL");
  
  PwrtObjid_getVid = (*env)->GetMethodID( env, PwrtObjid_id, "getVid", "()I");
  if(PwrtObjid_getVid == NULL) printf("Pwrtobjid_getVid är NULL");
  
  //hämta oix och vid för objektet som skickats med från anroparen
  user.oix = (*env)->CallIntMethod( env, objid_obj, PwrtObjid_getOix);
  user.vid = (*env)->CallIntMethod( env, objid_obj, PwrtObjid_getVid);
    
  //gör connect
  sts = mh_OutunitConnect(
		user,
		mh_eOutunitType_Operator,
		mh_mOutunitFlags_ReadWait,
		ev_mh_ack_bc,
		ev_mh_alarm_bc,
		NULL,
		NULL,
		ev_mh_clear_alarmlist_bc,
		NULL,
		ev_mh_info_bc,
		ev_mh_return_bc
		);
  jsts = (jint) sts;
  
  //skapa returobjekt
  return_obj = (*env)->NewObject( env, pwrtStatus_id,
  	pwrtStatus_cid, jsts);
  
  return return_obj;
}

JNIEXPORT jobject JNICALL Java_jpwr_rt_Mh_outunitDisConnect
  (JNIEnv *env, jclass obj)
{
  jclass 	pwrtStatus_id;
  jmethodID 	pwrtStatus_cid;
  int sts;
  jint 		jsts;
  jobject 	return_obj;
  
  //hämta upp classid och sedan pekare till konstruktorn för PwrtStatus
  pwrtStatus_id = (*env)->FindClass( env, "jpwr/rt/PwrtStatus");
  pwrtStatus_cid = (*env)->GetMethodID( env, pwrtStatus_id,
    	"<init>", "(I)V");
  
  sts = mh_OutunitDisconnect();
  
  jsts = (jint) sts;
  
  //skapa returobjekt
  return_obj = (*env)->NewObject( env, pwrtStatus_id,
  	pwrtStatus_cid, jsts);
  
  return return_obj;
}

JNIEXPORT jobject JNICALL Java_jpwr_rt_Mh_outunitReceive
  (JNIEnv *env, jclass obj)
{
  jclass pwrtStatus_id = NULL;
  static jmethodID pwrtStatus_cid = NULL;
  int sts;
  jint 		jsts;
  jobject 	return_obj;
  

  pwrtStatus_id = (*env)->FindClass( env, "jpwr/rt/PwrtStatus");
  if(pwrtStatus_id == NULL)
  {
    printf("fel vid PwrtStatus classid hämtning\n");
    return NULL;
  }

  if(pwrtStatus_cid == NULL)
  {
    //hämta upp classid och sedan pekare till konstruktorn för PwrtStatus
    pwrtStatus_cid = (*env)->GetMethodID( env, pwrtStatus_id,
    	                                  "<init>", "(I)V");
    if(pwrtStatus_cid == NULL)
    {
      printf("fel vid PwrtStatus methodid hämtning\n");
      return NULL;
    }
  }
  sts = mh_OutunitReceive();
  jsts = (jint) sts;
  
  //skapa returobjekt
  return_obj = (*env)->NewObject( env, pwrtStatus_id,
  	pwrtStatus_cid, jsts);
	
  (*env)->DeleteLocalRef(env,pwrtStatus_id);
  
  return return_obj;
}


/*från xtt_ev.cpp*/
pwr_tStatus ev_mh_ack_bc( mh_sAck *MsgP)
{
  JNIEnv *env;
  jclass PwrtObjid_id;
  jmethodID PwrtObjid_cid;
  jobject objid_obj = NULL;
  jstring jevText;
  jstring jevName;
  jstring jevTime;
  jstring jevBirthTime;


  jint jevFlags;
  jint jevPrio;
  jint jevStatus;
  jint jevNix;
  jint jevIdx;

  jstring jevTargetBirthTime;
  jint jevTargetNix;
  jint jevTargetIdx;
  char targetBirthTime_str[40];
  
  jint jevType;
  jint oix, vid;
  char time_str[40];
  char birthTime_str[40];
  pwr_tObjid objid = MsgP->Info.SupObject;
  
  pwr_tTime time = MsgP->Info.EventTime;
  pwr_tTime birthTime = MsgP->Info.Id.BirthTime;
  pwr_tTime targetBirthTime = MsgP->TargetId.BirthTime;
  
  //hämta enviormentpekaren
  (*jvm)->AttachCurrentThread(jvm,(void **)&env,NULL);
  if(env == NULL) printf("env är null");
  
  
  
  PwrtObjid_id = (*env)->FindClass( env, "jpwr/rt/PwrtObjid");
  PwrtObjid_cid = (*env)->GetMethodID( env, PwrtObjid_id,
    	"<init>", "(II)V");

  oix = (jint) objid.oix;
  vid = (jint) objid.vid;
  objid_obj = (*env)->NewObject( env, PwrtObjid_id, PwrtObjid_cid, 
    	oix, vid);

  
  
  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  time_AtoAscii( &birthTime, time_eFormat_ComprDateAndTime, birthTime_str, sizeof(birthTime_str));
  time_AtoAscii( &targetBirthTime, time_eFormat_ComprDateAndTime, targetBirthTime_str, sizeof(targetBirthTime_str));
  
  //gör om till Java-strängar
  jevText = (*env)->NewStringUTF( env, " "); //eventText används inte vid ack
  jevName = (*env)->NewStringUTF( env, MsgP->Info.EventName);
  jevTime = (*env)->NewStringUTF( env, time_str);
  jevBirthTime = (*env)->NewStringUTF( env, birthTime_str);
  jevTargetBirthTime = (*env)->NewStringUTF( env, targetBirthTime_str);
    
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)1; //finns ej i mh_sAck och används ej heller  
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;

  jevTargetNix = (jint)MsgP->TargetId.Nix;
  jevTargetIdx = (jint)MsgP->TargetId.Idx;

  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx,
				jevTargetNix, jevTargetBirthTime, jevTargetIdx,
				jevType, objid_obj);
  //important:check if an exception was raised 
  if ((*env)->ExceptionCheck(env))
  {
    printf("exception i ack\n");
    return 1;
  }
  
  return 1;
}

pwr_tStatus ev_mh_return_bc( mh_sReturn *MsgP)
{
  JNIEnv *env;
  jclass PwrtObjid_id;
  jmethodID PwrtObjid_cid;
  jobject objid_obj = NULL;
  jstring jevText;
  jstring jevName;
  jstring jevTime;
  jstring jevBirthTime;
  jint jevFlags;
  jint jevPrio;
  jint jevStatus;
  jint jevNix;
  jint jevIdx;
  jint jevType;
  jint oix, vid;

  jstring jevTargetBirthTime;
  jint jevTargetNix;
  jint jevTargetIdx;

  char targetBirthTime_str[40];

  char time_str[40];
  
  char birthTime_str[40];
  
  pwr_tObjid objid = MsgP->Info.Object;
  pwr_tTime time = MsgP->Info.EventTime;
  pwr_tTime birthTime = MsgP->Info.Id.BirthTime;
  pwr_tTime targetBirthTime = MsgP->TargetId.BirthTime;
  
  //hämta enviormentpekaren
  (*jvm)->AttachCurrentThread(jvm,(void **)&env,NULL);
  if(env == NULL) printf("env är null");
  
  PwrtObjid_id = (*env)->FindClass( env, "jpwr/rt/PwrtObjid");
  PwrtObjid_cid = (*env)->GetMethodID( env, PwrtObjid_id,
    	"<init>", "(II)V");

  oix = (jint) objid.oix;
  vid = (jint) objid.vid;
  objid_obj = (*env)->NewObject( env, PwrtObjid_id, PwrtObjid_cid, 
    	oix, vid);

  
  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  time_AtoAscii( &birthTime, time_eFormat_ComprDateAndTime, birthTime_str, sizeof(birthTime_str));
  time_AtoAscii( &targetBirthTime, time_eFormat_ComprDateAndTime, targetBirthTime_str, sizeof(targetBirthTime_str));
  
  //gör om till Java-strängar
  jevText = (*env)->NewStringUTF( env, MsgP->EventText);
  jevName = (*env)->NewStringUTF( env, MsgP->Info.EventName);
  jevTime = (*env)->NewStringUTF( env, time_str);
  jevBirthTime = (*env)->NewStringUTF( env, birthTime_str);
  jevTargetBirthTime = (*env)->NewStringUTF( env, targetBirthTime_str);

  
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)1;//mh_sReturn har ingen status
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;

  jevTargetNix = (jint)MsgP->TargetId.Nix;
  jevTargetIdx = (jint)MsgP->TargetId.Idx;


  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx,
				jevTargetNix, jevTargetBirthTime, jevTargetIdx,
				jevType, objid_obj);
  //important:check if an exception was raised 
  if ((*env)->ExceptionCheck(env))
  {
    printf("exception i return\n");
    return 1;
  }
  
  return 1;
}

pwr_tStatus ev_mh_alarm_bc( mh_sMessage *MsgP)
{
  JNIEnv *env;
  jclass PwrtObjid_id;
  jmethodID PwrtObjid_cid;
  jobject objid_obj = NULL;
  jstring jevText;
  jstring jevName;
  jstring jevTime;
  jstring jevBirthTime;
  jint jevFlags;
  jint jevPrio;
  jint jevStatus;
  jint jevNix;
  jint jevIdx;
  jint jevType;
  jint oix, vid;

  jstring jevTargetBirthTime;
  jint jevTargetNix = (jint) 0;
  jint jevTargetIdx = (jint) 0;


  char time_str[40];
  
  char birthTime_str[40];
  
  pwr_tObjid objid = MsgP->Info.Object;
  pwr_tTime time = MsgP->Info.EventTime;
  pwr_tTime birthTime = MsgP->Info.Id.BirthTime;
  
  //hämta enviormentpekaren
  (*jvm)->AttachCurrentThread(jvm,(void **)&env,NULL);
  if(env == NULL) printf("env är null");
  
  PwrtObjid_id = (*env)->FindClass( env, "jpwr/rt/PwrtObjid");
  PwrtObjid_cid = (*env)->GetMethodID( env, PwrtObjid_id,
    	"<init>", "(II)V");

  oix = (jint) objid.oix;
  vid = (jint) objid.vid;
  objid_obj = (*env)->NewObject( env, PwrtObjid_id, PwrtObjid_cid, 
    	oix, vid);

  
  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  time_AtoAscii( &birthTime, time_eFormat_ComprDateAndTime, birthTime_str, sizeof(birthTime_str));
  
  //gör om till Java-strängar
  jevText = (*env)->NewStringUTF( env, MsgP->EventText);
  jevName = (*env)->NewStringUTF( env, MsgP->Info.EventName);
  jevTime = (*env)->NewStringUTF( env, time_str);
  jevBirthTime = (*env)->NewStringUTF( env, birthTime_str);

  jevTargetBirthTime = (*env)->NewStringUTF( env, " ");

  
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)MsgP->Status;
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;
  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx,
				jevTargetNix, jevTargetBirthTime, jevTargetIdx,
				jevType, objid_obj);
  //important:check if an exception was raised 
  if ((*env)->ExceptionCheck(env))
  {
    printf("exception i alarm\n");
    return 1;
  }
  
  return 1;
}

pwr_tStatus ev_mh_info_bc( mh_sMessage *MsgP)
{
  JNIEnv *env;
  jclass PwrtObjid_id;
  jmethodID PwrtObjid_cid;
  jobject objid_obj = NULL;
  jstring jevText;
  jstring jevName;
  jstring jevTime;
  jstring jevBirthTime;
  jint jevFlags;
  jint jevPrio;
  jint jevStatus;
  jint jevNix;
  jint jevIdx;
  jint jevType;
  jint oix, vid;

  jstring jevTargetBirthTime;
  jint jevTargetNix = (jint) 0;
  jint jevTargetIdx = (jint) 0;

  char time_str[40];
  
  char birthTime_str[40];
  
  pwr_tObjid objid = MsgP->Info.Object;
  pwr_tTime time = MsgP->Info.EventTime;
  pwr_tTime birthTime = MsgP->Info.Id.BirthTime;
  
  //hämta enviormentpekaren
  (*jvm)->AttachCurrentThread(jvm,(void **)&env,NULL);
  if(env == NULL) printf("env är null");
  
  PwrtObjid_id = (*env)->FindClass( env, "jpwr/rt/PwrtObjid");
  PwrtObjid_cid = (*env)->GetMethodID( env, PwrtObjid_id,
    	"<init>", "(II)V");

  oix = (jint) objid.oix;
  vid = (jint) objid.vid;
  objid_obj = (*env)->NewObject( env, PwrtObjid_id, PwrtObjid_cid, 
    	oix, vid);

  
  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, sizeof(time_str));
  time_AtoAscii( &birthTime, time_eFormat_ComprDateAndTime, birthTime_str, sizeof(birthTime_str));
  
  //gör om till Java-strängar
  jevText = (*env)->NewStringUTF( env, MsgP->EventText);
  jevName = (*env)->NewStringUTF( env, MsgP->Info.EventName);
  jevTime = (*env)->NewStringUTF( env, time_str);
  jevBirthTime = (*env)->NewStringUTF( env, birthTime_str);

  jevTargetBirthTime = (*env)->NewStringUTF( env, " ");
  
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)MsgP->Status;
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;
  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx,
				jevTargetNix, jevTargetBirthTime, jevTargetIdx,
				jevType, objid_obj);
  //important:check if an exception was raised 
  if ((*env)->ExceptionCheck(env))
  {
    printf("exception i info\n");
    return 1;
  }
  
  return 1;
}

pwr_tStatus ev_mh_clear_alarmlist_bc( pwr_tNodeIndex nix)
{

  JNIEnv *env;
  jint jevType;
  
  //printf("C-kod: clearalarmlist2\n");
  //hämta enviormentpekaren
  (*jvm)->AttachCurrentThread(jvm,(void **)&env,NULL);
  if(env == NULL) printf("env är null");
  

  jevType = (jint)66;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, NULL, NULL,
                                NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
				NULL, NULL, jevType, NULL);
  //printf("C-kod: efter callback\n");
  //important:check if an exception was raised 
  if ((*env)->ExceptionCheck(env))
  {
    printf("exception i clear alarmlist\n");
  }  

  return 1;
}

/*slut på från xtt_ev.cpp*/
