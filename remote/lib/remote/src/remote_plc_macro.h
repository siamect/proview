/* 
 * Proview   $Id: remote_plc_macro.h,v 1.1 2006-01-26 08:10:24 claes Exp $
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

/*  Remote plc macros */
# include <float.h>

/*_*
  RemTransSend

  @aref remtranssend RemTransSend
*/
#define RemTransSend_exec( object, subwindow) \
  if ( *object->SendP && !object->Send) { \
    if ( ((pwr_sClass_RemTrans *)(*object->RemTransP))->DataValid > 0) \
      object->Error = 1; \
    else { \
      subwindow; \
      ((pwr_sClass_RemTrans *)(*object->RemTransP))->DataValid = 1; \
      object->Error = 0; \
    } \
  } \
  else \
    object->Error = \
        EVEN( ((pwr_sClass_RemTrans *)(*object->RemTransP))->LastSts); \
 \
  object->Send = *object->SendP; \
 \
  if ( ((pwr_sClass_RemTrans *)(*object->RemTransP))->DataValid > 0) \
    object->Occupied = 1; \
  else \
    object->Occupied = 0; \
 \
  if ( ((pwr_sClass_RemTrans *)(*object->RemTransP))->Buffers > 0) { \
    object->Buffer = 1; \
    if ( ((pwr_sClass_RemTrans *)(*object->RemTransP))->Buffers >= \
         ((pwr_sClass_RemTrans *)(*object->RemTransP))->MaxBuffers) \
      object->Occupied = 1; \
  } \
  else \
    object->Buffer = 0;

/*_*
  RemTransRcv

  @aref remtransrcv RemTransRcv
*/
#define RemTransRcv_exec( object, subwindow) \
  if ( ((pwr_sClass_RemTrans *)(*object->RemTransP))->DataValid > 0) { \
    subwindow; \
    ((pwr_sClass_RemTrans *)(*object->RemTransP))->DataValid = 0; \
      object->Received = 1; \
  } \
  else \
    object->Received = 0;

