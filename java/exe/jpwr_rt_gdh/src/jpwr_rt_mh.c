
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

/*från xtt_ev.cpp*/
pwr_tStatus ev_mh_ack_bc( mh_sAck *MsgP);
pwr_tStatus ev_mh_return_bc( mh_sReturn *MsgP);
pwr_tStatus ev_mh_alarm_bc( mh_sMessage *MsgP);
pwr_tStatus ev_mh_info_bc( mh_sMessage *MsgP);
pwr_tStatus ev_mh_clear_alarmlist_bc( pwr_tNodeIndex nix);
/*slut på från xtt_ev.cpp*/
const int EventType_Info = 0;
const int EventType_Return = 1;
const int EventType_Ack = 2;
const int EventType_Alarm = 3;
const int EventType_ClearAlarmList = 4;
/*från jpwr_rt_gdh.c
typedef struct {
  char		TypeStr[32];
  pwr_eType	Type;
  int		Size;
} gdh_sSuffixTable;

static int gdh_ExtractNameSuffix(	char   *Name,
                          		char   **Suffix);
static void  gdh_TranslateSuffixToClassData (
    char        *SuffixPtr,
    pwr_eType 	*PwrType,
    int		*PwrSize,
    int  	*NoOfElements);
static int gdh_StringToAttr( char *str_value, char *buffer_p, int buffer_size,
	pwr_eType attrtype, int attrsize);
static void  gdh_AttrToString( int type_id, void *value_ptr,
        char *str, int size, int *len, char *format);
static void gdh_ConvertUTFstring( char *out, char *in);
//slut på från jpwr_rt_gdh.c
*/

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
    printf("null");
  }
  //hämta pekare till staticmetoden messReceived
  Mh_messReceived_id = (*env)->GetStaticMethodID( env, Mh_id, "messReceived",                              
  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIIILjava/lang/String;IILjpwr/rt/PwrtObjid;)V");

  sts = (*env)->GetJavaVM(env, &jvm);
  if(sts)
  {
    printf("Skit vid GetJavaVM\n");
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
    printf("efter status methodidhämtning\n");
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
  
  //printf("efter birthtime hämtning\n");
  
  eventId.Nix = (pwr_tNodeIndex)(*env)->GetIntField(env,id,nix_fid);
  eventId.Idx = (pwr_tUInt32)(*env)->GetIntField(env,id,idx_fid);
  
  //printf("efter getfielad\n");
  
  jstr = (*env)->GetObjectField(env,id,birthTime_fid);
  (const char *)str_copy = (*env)->GetStringUTFChars(env,jstr,NULL);
  //strcpy(str, str_copy);
  if (str_copy ==NULL)
  {
    sts = -1;//out of memory
  }
  //printf("efter strcpy\n");
  //printf("str = %s", str);
  
  time_AsciiToA(str_copy, &eventId.BirthTime);
  
  //printf("efter AsciiToA\n");
  //printf("eventid= %d %d ", eventId.Idx, eventId.Nix); 
  sts = mh_OutunitAck(&eventId);
  //printf("efter outunitack2 sts %d\n", sts);
  (*env)->ReleaseStringUTFChars(env,jstr,str_copy);
  //(*env)->ReleaseStringUTFChars(env,jstr,str);
  
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
  
  //printf("user.oix = %d user.vid = %d\n", user.oix, user.vid);
  
  // Wait for mh has flagged initizated qqq vad är detta 
  //printf("innan waitformh\n");
  mh_UtilWaitForMh();
  //printf("efter waitformh\n");

/*
  sts = gdh_ObjidToPointer ( user, (pwr_tAddress *) &userobject_ptr);
  if ( EVEN(sts)) 
  {
    printf("skit vid objidtopinter\n");
  }
*/


  //gör connect
  sts = mh_OutunitConnect(
		user,
		mh_eOutunitType_Operator,
		0,
		ev_mh_ack_bc,
		ev_mh_alarm_bc,
		NULL,
		NULL,
		ev_mh_clear_alarmlist_bc,
		NULL,
		ev_mh_info_bc,
		ev_mh_return_bc
		);
  //printf("I connect C-kod sts= %d\n", sts);
    
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
  //printf("I disconnect C-kod sts= %d\n", sts);
  
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
    printf("efter PwrtStatus methodid hämtning\n");
    if(pwrtStatus_cid == NULL)
    {
      printf("fel vid PwrtStatus methodid hämtning\n");
      return NULL;
    }
  }
  //printf("Innan receive C-kod\n");
  sts = mh_OutunitReceive();
  //printf("Efter receive C-kod sts= %d\n", sts);
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
  jint jevType;
  jint oix, vid;
  char time_str[40];
  char birthTime_str[40];
  pwr_tObjid objid = MsgP->Info.SupObject;
  
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
  jevText = (*env)->NewStringUTF( env, " "); //eventText används inte vid ack
  jevName = (*env)->NewStringUTF( env, MsgP->Info.EventName);
  jevTime = (*env)->NewStringUTF( env, time_str);
  jevBirthTime = (*env)->NewStringUTF( env, birthTime_str);
  
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)1; //finns ej i mh_sAck och används ej heller  
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;
  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx, jevType, objid_obj);
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
  char time_str[40];
  
  char birthTime_str[40];
  
  pwr_tObjid objid = MsgP->Info.SupObject;
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
  
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)1;//mh_sReturn har ingen status
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;
  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx, jevType, objid_obj);
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
  char time_str[40];
  
  char birthTime_str[40];
  
  pwr_tObjid objid = MsgP->Info.SupObject;
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
  
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)MsgP->Status;
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;
  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx, jevType, objid_obj);
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
  char time_str[40];
  
  char birthTime_str[40];
  
  pwr_tObjid objid = MsgP->Info.SupObject;
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
  
  //gör om till Java-int
  jevFlags = (jint)MsgP->Info.EventFlags;
  jevPrio = (jint)MsgP->Info.EventPrio;
  jevStatus = (jint)MsgP->Status;
  jevNix = (jint)MsgP->Info.Id.Nix;
  jevIdx = (jint)MsgP->Info.Id.Idx;
  jevType = (jint)MsgP->Info.EventType;
  //anropa callback metoden i Mh-klassen
  (*env)->CallStaticVoidMethod( env, Mh_id, Mh_messReceived_id, jevText, jevName,
                                jevTime, jevFlags, jevPrio, jevStatus, jevNix, jevBirthTime, jevIdx, jevType, objid_obj);
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
                                NULL, NULL, NULL, NULL, NULL, NULL, NULL, jevType, NULL);
  //printf("C-kod: efter callback\n");
  //important:check if an exception was raised 
  if ((*env)->ExceptionCheck(env))
  {
    printf("exception i clear alarmlist\n");
  }  

  return 1;
}

/*slut på från xtt_ev.cpp*/
