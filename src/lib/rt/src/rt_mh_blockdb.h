#ifndef rt_mh_blockdb_h
#define rt_mh_blockdb_h

/* rt_mh_blockdb.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
