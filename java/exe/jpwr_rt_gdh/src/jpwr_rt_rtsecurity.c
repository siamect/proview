
#include "jpwr_rt_rtsecurity.h"
#include "pwr.h"
#include "pwr_privilege.h"
#include "co_api.h"

JNIEXPORT jobject JNICALL Java_jpwr_rt_RtSecurity_checkUser
  (JNIEnv *env , jclass object, jstring jsystemgroup, jstring juser, 
	jstring jpassword)
{
  int sts;
  const char *systemgroup;
  char *csystemgroup;
  const char *user;
  char *cuser;
  const char *password;
  char *cpassword;
  jclass cdhrInt_id;
  jmethodID cdhrInt_cid;
  jobject return_obj;
  unsigned int priv;
  jint jsts;
  jint jpriv;

  cdhrInt_id = (*env)->FindClass( env, "jpwr/rt/CdhrInt");
  cdhrInt_cid = (*env)->GetMethodID( env, cdhrInt_id,
    	"<init>", "(II)V");

  systemgroup = (*env)->GetStringUTFChars( env, jsystemgroup, 0);
  csystemgroup = (char *)systemgroup;
  user = (*env)->GetStringUTFChars( env, juser, 0);
  cuser = (char *)user;
  password = (*env)->GetStringUTFChars( env, jpassword, 0);
  cpassword = (char *)password;

  sts = user_CheckUser( csystemgroup, cuser, cpassword, &priv);

  (*env)->ReleaseStringUTFChars( env, jsystemgroup, csystemgroup);
  (*env)->ReleaseStringUTFChars( env, juser, cuser);
  (*env)->ReleaseStringUTFChars( env, jpassword, cpassword);

  jsts = (jint) sts;
  if ( sts % 2 == 0)
    priv = pwr_mPrv_RtRead;
  jpriv = priv;
  return_obj = (*env)->NewObject( env, cdhrInt_id,
  	cdhrInt_cid, jpriv, jsts);
  return return_obj; 
}

