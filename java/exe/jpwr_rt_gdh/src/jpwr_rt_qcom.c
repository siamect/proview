#include "jpwr_rt_qcom.h"
#include "pwr.h"
#include "rt_qcom.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_cdh_msg.h"
#include "rt_qcom_msg.h"

JNIEXPORT jobject JNICALL Java_jpwr_rt_Qcom_createQ
  (JNIEnv *env, jobject object, jint qix, jint nid, jstring jname)
{
  jclass 	qcomrCreateQ_id;
  jmethodID 	qcomrCreateQ_cid;
  jobject 	return_obj;
  jint		jsts;
  qcom_sQid	qid;
  qcom_sQattr	attr;
  int		sts;
  char		*cstr;
  const char 	*name;

//  qcom_sQid other_que = qcom_cQapplEvent;

  qcomrCreateQ_id = (*env)->FindClass( env, "jpwr/rt/QcomrCreateQ");
  qcomrCreateQ_cid = (*env)->GetMethodID( env, qcomrCreateQ_id,
    	"<init>", "(III)V");

  name = (*env)->GetStringUTFChars( env, jname, 0);
  cstr = (char *)name;

  attr.type = qcom_eQtype_private;
  attr.quota = 100;
  qid.qix = qix;
  qid.nid = nid;
  qcom_CreateQ( &sts, &qid, &attr, cstr);
  (*env)->ReleaseStringUTFChars( env, jname, cstr);
  printf( "Create que, qix %d, nid %d, sts %d\n", qid.qix, qid.nid, sts);
  jsts = (jint) sts;
  return_obj = (*env)->NewObject( env, qcomrCreateQ_id,
  	qcomrCreateQ_cid, qid.qix, qid.nid, jsts);

  // Bind to broadcast que
  // qcom_Bind( &sts, &qid, &other_que);
  // if ( EVEN(sts))
  //   printf("** Unable to bind to qcom broadcast que\n");

  return return_obj;
}

JNIEXPORT jobject JNICALL Java_jpwr_rt_Qcom_getString
  (JNIEnv *env, jobject object, jint qix, jint nid)
{
  int 		sts;
  jclass 	cdhrString_id;
  jmethodID 	cdhrString_cid;
  jobject 	return_obj;
  jint 		jsts;
  qcom_sQid	qid;
  qcom_sGet	get;
  jstring	jdata = NULL;
  int 		sts2;
//  static qcom_sAid op_aid = {0,0};
//  qcom_sAid aid;

  cdhrString_id = (*env)->FindClass( env, "jpwr/rt/CdhrString");
  cdhrString_cid = (*env)->GetMethodID( env, cdhrString_id,
    	"<init>", "(Ljava/lang/String;I)V");
  qid.qix = qix;
  qid.nid = nid;
  memset( &get, 0, sizeof(get));
  qcom_Get( &sts, &qid, &get, 0);
  if ( ODD(sts))
  {
    printf("Qcom_get: Received data: %s\n", (char *)get.data);
    jdata = (*env)->NewStringUTF( env, (char *)get.data);


    // Get disconnect broadcast...
    // if ( get.type.b == qcom_eBtype_qcom &&
    //      get.type.s == qcom_eStype_applDisconnect) 
    // {
    //   aid = ((qcom_sAppl*) get.data)->aid;
    //   printf("applDisconnect received %d\n", aid.aix);

    //   if ( qcom_AidIsEqual( &aid, &op_aid))
    //   {
    //     jdata = (*env)->NewStringUTF( env, "qcom_exit");
    //   }
    // }
    // else
    // {
    //   op_aid = get.sender;
    //   printf( "Aid received: %d\n", op_aid.aix);
    //   jdata = (*env)->NewStringUTF( env, (char *)get.data);
    // }

    qcom_Free( &sts2, get.data);
  }

  jsts = (jint) sts;
  return_obj = (*env)->NewObject( env, cdhrString_id,
  	cdhrString_cid, jdata, jsts);
  return return_obj;  
}

JNIEXPORT jobject JNICALL Java_jpwr_rt_Qcom_putString
  (JNIEnv *env, jobject object, jint qix, jint nid, jstring jdata)
{
  int 		sts;
  const char 	*data;
  jclass 	pwrtStatus_id;
  jmethodID 	pwrtStatus_cid;
  jobject 	return_obj;
  jint		jsts;
  qcom_sQid	qid;
  qcom_sPut	put;
  char		*cstr;
//  int		sts2;

  pwrtStatus_id = (*env)->FindClass( env, "jpwr/rt/PwrtStatus");
  pwrtStatus_cid = (*env)->GetMethodID( env, pwrtStatus_id,
    	"<init>", "(I)V");

  data = (*env)->GetStringUTFChars( env, jdata, 0);
  cstr = (char *)data;
  put.data = (char *)data;
  put.size = strlen(data) + 1;
  put.type.b = qcom_eBtype__;
  put.type.s = qcom_eStype__;
  put.reply.qix = 0;
  put.reply.nid = 0;
  qid.qix = qix;
  qid.nid = nid;
  sts = qcom_Put( &sts, &qid, &put);
  (*env)->ReleaseStringUTFChars( env, jdata, cstr);
//  if ( ODD(sts))
//    qcom_Free( &sts2, put.data);
  jsts = (jint) sts;
  return_obj = (*env)->NewObject( env, pwrtStatus_id,
  	pwrtStatus_cid, jsts);
  return return_obj;
}

