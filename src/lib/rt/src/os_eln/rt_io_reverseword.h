/* rt_io_reverseword.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

typedef union	{
    unsigned char    B[2];
    unsigned short   W;
    } IO_WORD_UNION;

unsigned short io_ReverseWord(
IO_WORD_UNION *Word);

