/* 
 * Proview   $Id: jpwr_rt_rtsecurity.c,v 1.4 2008-06-25 08:00:25 claes Exp $
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


#include "jpwr_rt_rtsecurity.h"
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_privilege.h"
#include "co_api_user.h"
#include "co_cdh.h"
#include "rt_gdh.h"

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
  pwr_tObjid	oid;
  pwr_sSecurity	sec;	
  pwr_tAttrRef aref;

  cdhrInt_id = (*env)->FindClass( env, "jpwr/rt/CdhrInt");
  cdhrInt_cid = (*env)->GetMethodID( env, cdhrInt_id,
    	"<init>", "(II)V");

  systemgroup = (*env)->GetStringUTFChars( env, jsystemgroup, 0);
  csystemgroup = (char *)systemgroup;
  user = (*env)->GetStringUTFChars( env, juser, 0);
  cuser = (char *)user;
  password = (*env)->GetStringUTFChars( env, jpassword, 0);
  cpassword = (char *)password;

  sts = gdh_GetClassList ( pwr_eClass_Security, &oid);
  if (ODD(sts)) {
    aref = cdh_ObjidToAref( oid);
    sts = gdh_GetObjectInfoAttrref( &aref, &sec, sizeof(sec));
    if ( ODD(sts)){
      sts = user_CheckUser( sec.WebSystemGroup, cuser, cpassword, &priv);
    }
  }
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

