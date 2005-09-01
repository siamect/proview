/* 
 * Proview   $Id: jpwr_rt_Errh.c,v 1.3 2005-09-01 14:57:47 claes Exp $
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




