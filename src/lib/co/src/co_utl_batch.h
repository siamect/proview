#ifndef co_utl_batch_h
#define co_utl_batch_h

/* co_utl_batch.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

/* Function prototypes. */

pwr_tStatus utl_BatchEnterJob (
  int  *Entry,
  char *Queue,
  char *File,
  char *Logfile,
  char *Par[]
);

pwr_tStatus
utl_BatchFindQueue (
  char *Queue
);

pwr_tStatus
utl_BatchWaitJob (
  int  *Entry
);

#endif
