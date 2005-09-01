/* 
 * Proview   $Id: testbitmask.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

typedef unsigned int	pwr_tVolumeId;
typedef unsigned int	pwr_tObjectIx;
typedef unsigned int	pwr_tClassId;
typedef unsigned int	pwr_tTypeId;
typedef unsigned int	pwr_tVersion;
typedef unsigned int	pwr_tPwrVersion;
typedef unsigned int	pwr_tProjVersion;
typedef unsigned int	pwr_tUserId;
typedef unsigned int	pwr_tDbId;
typedef pwr_tVolumeId	pwr_tNodeId;
typedef pwr_tNodeId	pwr_tNodeIndex;

typedef struct {
  pwr_tObjectIx	oix;
  pwr_tVolumeId vid;
} pwr_tObjid;


typedef struct {
#if defined(__vms)
  unsigned int	aix		: 12;
  unsigned int	reserved	: 3;
  unsigned int	tix		: 11;
  unsigned int	tyg		: 4;
  unsigned int	must_be_three	: 2;

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;

#elif defined (__powerpc__)

  unsigned int	must_be_three	: 2;
  unsigned int	tyg		: 4;
  unsigned int	tix		: 11;
  unsigned int	reserved	: 3;
  unsigned int	aix		: 12;

  unsigned int	vid_3		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;
#endif
} cdh_mTypeObjid;

typedef struct {
  unsigned int	oix		: 32;

#if defined(__vms)
  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;
#elif defined (__powerpc__)
  unsigned int	vid_3		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;
#endif
} cdh_mObjid;

typedef struct {
#if defined(__vms)
  unsigned int	aix		: 12;
  unsigned int	reserved	: 3;
  unsigned int	bix		: 3;
  unsigned int	cix		: 12;
  unsigned int  must_be_two	: 2;

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;
#elif defined (__powerpc__)
  unsigned int  must_be_two	: 2;
  unsigned int	cix		: 12;
  unsigned int	bix		: 3;
  unsigned int	reserved	: 3;
  unsigned int	aix		: 12;

  unsigned int	vid_3		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;
#endif
} cdh_mClassObjid;

typedef union {
  pwr_tObjid		pwr;
  cdh_mObjid		o;
  cdh_mClassObjid	c;
  cdh_mTypeObjid	t;
} cdh_uObjid;


#if 0
static char*
print_bin (
  int *i
)
{
  static char buff[80];
  int b;

  buff[0] = '\0';

  for (b = 0; b < 32; b++) {
    strcat(buff, ((*i & (1<<(31-b))) ? "1" : "0"));
    if ((b + 1) % 8 == 0) strcat(buff, " ");
  }

  return buff;
}
#endif

main ()
{
  int b;
  cdh_uObjid oid;

  union {
    int mask;
    struct {
      unsigned int b0  : 1;
      unsigned int b1  : 1;
      unsigned int b2  : 1;
      unsigned int b3  : 1;
      unsigned int b4  : 1;
      unsigned int b5  : 1;
      unsigned int b6  : 1;
      unsigned int b7  : 1;
      unsigned int b8  : 1;
      unsigned int b9  : 1;
      unsigned int b10 : 1;
      unsigned int b11 : 1;
      unsigned int b12 : 1;
      unsigned int b13 : 1;
      unsigned int b14 : 1;
      unsigned int b15 : 1;
      unsigned int b16 : 1;
      unsigned int b17 : 1;
      unsigned int b18 : 1;
      unsigned int b19 : 1;
      unsigned int b20 : 1;
      unsigned int b21 : 1;
      unsigned int b22 : 1;
      unsigned int b23 : 1;
      unsigned int b24 : 1;
      unsigned int b25 : 1;
      unsigned int b26 : 1;
      unsigned int b27 : 1;
      unsigned int b28 : 1;
      unsigned int b29 : 1;
      unsigned int b30 : 1;
      unsigned int b31 : 1;
    } b;
  } a;

  b = 0;


  oid.pwr.vid = 257;
  oid.pwr.oix = 1024;
  a.mask = 0;

  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
  a.mask = (1<<b++);
}
