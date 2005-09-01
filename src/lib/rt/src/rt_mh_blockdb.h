/* 
 * Proview   $Id: rt_mh_blockdb.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_mh_blockdb_h
#define rt_mh_blockdb_h

typedef struct {
  pwr_tTime CreationTime;  
  pwr_tTime OpenTime;  
  pwr_tTime CloseTime;  
  pwr_tTime PutTime;  
  pwr_tTime GetTime;  
  pwr_tUInt32 Version;
  long PutCount;
  long GetCount;
  long FileSize;
  long FreeSize;
  long MaxFreeSize;
  long SegSize;
  long MaxSegSize;
  long SectSize;
  long MaxSectSize;
  long SectPos;
} mh_sBlockDbHead;

typedef struct {
  FILE *File;
  mh_sBlockDbHead Head;
} mh_sBlockDb;

/*------------------------------------------------------------------------------
*/
mh_sBlockDb *mh_BlockDbOpen (
  char *FileName,
  pwr_tUInt32 *size
);

/*------------------------------------------------------------------------------
*/

mh_sBlockDb *mh_BlockDbClose (
  mh_sBlockDb *dp
);


/*------------------------------------------------------------------------------
*/

mh_sBlockDb *mh_BlockDbGet (
  mh_sBlockDb *dp,
  pwr_tUInt32 *size,
  char *buffer
);


/*------------------------------------------------------------------------------
*/

mh_sBlockDb *mh_BlockDbPut ( 
  mh_sBlockDb *dp,
  pwr_tUInt32 size,
  char *buffer
);
#endif
