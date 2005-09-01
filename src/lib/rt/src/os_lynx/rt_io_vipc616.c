/* 
 * Proview   $Id: rt_io_vipc616.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_io_vipc616.h -- I/O VIPC616, Greenspring. */
#if 0
#include <ctype.h>

#include "rt_io_vipc616.h"
#include "rt_io_msg.h"
#include "rt_gdh.h"
#include "rt_io_base.h"


pwr_tStatus vipc616_GetAddress(
  pwr_tObjid		ipObjid, /* Objid of IP-module */
  pwr_tUInt32		slot,
  char			**ioAddress,
  char			**idAddress,
  char			**memoryAddress
)
{
  pwr_tStatus		sts;
  pwr_tObjid		parent;
  pwr_tClassId		cid;
  pwr_sClass_VIPC616*	vipc;
  unsigned int 		memSize;
  int			slotNo;
  unsigned int		ioAdrs;
  unsigned int		memAdrs;

  sts = gdh_GetParent(ipObjid, &parent);
  if (EVEN(sts))
    return IO__NOTIPCARRIER;

  sts = gdh_GetObjectClass(parent, &cid);
  if (EVEN(sts))
    return IO__NOTIPCARRIER;

  if (pwr_cClass_VIPC616 != cid)
    return IO__NOTIPCARRIER;

  sts = gdh_ObjidToPointer(parent, (pwr_tAddress *)&vipc);
  if (EVEN(sts))
    return IO__NOTIPCARRIER;


  switch (toupper(slot)) {
  case 'A':
  case 'B':
  case 'C':
  case 'D':
    slotNo = toupper(slot) - 'A';
    break;
  default:
    return IO__INVIPSLOT;
    break;
  }  

  if (vipc->MemoryBase == 32)
    memSize = 0x800000; 	/* 8 MB */
  else if (vipc->MemorySlotSize <= 128)
    memSize = 0x20000;  	/* 128 kB */
  else if (vipc->MemorySlotSize <= 256)
    memSize = 0x40000;  	/* 256 kB */
  else if (vipc->MemorySlotSize <= 512)
    memSize = 0x80000;  	/* 512 kB */
  else if (vipc->MemorySlotSize <= 1024)
    memSize = 0x100000;  	/* 1 MB */
  else
    memSize = 0x200000;  	/* 2 MB */

  ioAdrs = io_GetShortIoAddress(); 
  /* Each slot have 128 bytes IO space */
  if (ioAddress != NULL) 
    *ioAddress = (char *) (ioAdrs + vipc->IoAddress + 
                          (unsigned int)(slotNo * 0x100));
  
  /* After the IO space each slot have 128 ID bytes  */
  if (idAddress != NULL)
    *idAddress = (char *) (ioAdrs + vipc->IoAddress + 
                          (unsigned int) 0x80 + (unsigned int)(slotNo * 0x100));

 
  if (memoryAddress != NULL) {
    if (vipc->MemoryBase == 32)
      memAdrs = io_GetA32D16Address();
    else
      memAdrs = io_GetA24Address();

    *memoryAddress = (char *)(memAdrs + (unsigned int)(slotNo * memSize) + vipc->MemoryAddress);
  }
  return IO__SUCCESS;  

}


#endif
