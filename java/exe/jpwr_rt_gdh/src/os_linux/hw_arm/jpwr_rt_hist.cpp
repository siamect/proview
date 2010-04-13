/* 
 * Proview   $Id: jpwr_rt_hist.cpp,v 1.6 2007-02-06 15:12:36 claes Exp $
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


using namespace std;

#include <string>
#include <string.h>
#include <stdio.h>
#include "jpwr_rt_hist.h"

JNIEXPORT void JNICALL Java_jpwr_rt_Hist_initHistIDs
  (JNIEnv *env, jclass cls)
{
}



/*
 * Class:     jpwr_rt_Mh
 * Method:    getHistList
 * Signature: (Ljava/lang/String;Ljava/lang/String;ZZZZZZZZLjava/lang/String;Ljava/lang/String;)[Ljpwr/rt/MhrEvent;
 */
JNIEXPORT jobjectArray JNICALL Java_jpwr_rt_Hist_getHistList
  (JNIEnv *env, jclass obj, jstring jstartTime, jstring jstopTime, jboolean jtypeAlarm, jboolean jtypeInfo, jboolean jtypeReturn, jboolean jtypeAck, jboolean jprioA, jboolean jprioB, jboolean jprioC, jboolean jprioD, jstring jname, jstring jtext)
{
  return (jobjectArray)NULL;
}

