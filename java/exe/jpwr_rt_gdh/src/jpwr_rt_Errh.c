#include "jpwr_rt_Errh.h"
#include "pwr.h"
#include "rt_errh.h"
#include "rt_errh_msg.h"

JNIEXPORT jint JNICALL Java_jpwr_rt_Errh_init
  (JNIEnv *env, jobject object, jstring jname, jint janix)
{
  jint		jsts;
  int		sts;
  char		*cstr;
  const char 	*name;
  int		anix = janix;

  name = (*env)->GetStringUTFChars( env, jname, 0);
  cstr = (char *)name;

  sts = errh_Init( cstr, anix);
  (*env)->ReleaseStringUTFChars( env, jname, cstr);

  jsts = (jint) sts;
  return jsts;
}

JNIEXPORT void JNICALL Java_jpwr_rt_Errh_setStatus
  (JNIEnv *env, jobject object, jint jsts)
{
  int sts = jsts;
  errh_SetStatus( sts);
}

JNIEXPORT void JNICALL Java_jpwr_rt_Errh_fatal
  (JNIEnv *env, jobject object, jstring jmsg)
{
  char		*cstr;
  const char 	*msg;

  msg = (*env)->GetStringUTFChars( env, jmsg, 0);
  cstr = (char *)msg;

  errh_Fatal( cstr);
  (*env)->ReleaseStringUTFChars( env, jmsg, cstr);
}

JNIEXPORT void JNICALL Java_jpwr_rt_Errh_error
  (JNIEnv *env, jobject object, jstring jmsg)
{
  char		*cstr;
  const char 	*msg;

  msg = (*env)->GetStringUTFChars( env, jmsg, 0);
  cstr = (char *)msg;

  errh_Error( cstr);
  (*env)->ReleaseStringUTFChars( env, jmsg, cstr);
}

JNIEXPORT void JNICALL Java_jpwr_rt_Errh_warning
  (JNIEnv *env, jobject object, jstring jmsg)
{
  char		*cstr;
  const char 	*msg;

  msg = (*env)->GetStringUTFChars( env, jmsg, 0);
  cstr = (char *)msg;

  errh_Warning( cstr);
  (*env)->ReleaseStringUTFChars( env, jmsg, cstr);
}

JNIEXPORT void JNICALL Java_jpwr_rt_Errh_info
  (JNIEnv *env, jobject object, jstring jmsg)
{
  char		*cstr;
  const char 	*msg;

  msg = (*env)->GetStringUTFChars( env, jmsg, 0);
  cstr = (char *)msg;

  errh_Info( cstr);
  (*env)->ReleaseStringUTFChars( env, jmsg, cstr);
}

JNIEXPORT void JNICALL Java_jpwr_rt_Errh_success
  (JNIEnv *env, jobject object, jstring jmsg)
{
  char		*cstr;
  const char 	*msg;

  msg = (*env)->GetStringUTFChars( env, jmsg, 0);
  cstr = (char *)msg;

  errh_Success( cstr);
  (*env)->ReleaseStringUTFChars( env, jmsg, cstr);
}




