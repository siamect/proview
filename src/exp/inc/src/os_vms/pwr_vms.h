/* 
 * Proview   $Id: pwr_vms.h,v 1.2 2005-09-01 14:57:49 claes Exp $
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

#ifndef pwr_vms_h
#define pwr_vms_h

/* pwr_vms.h -- OpenVMS specific stuff
*/


typedef struct vms_sItemList {
  short len;	/* Item buffer length */
  short code;	/* Item code */
  void *addr;	/* Item buffer address */
  short *rlen;	/* Item resultant length */
} vms_sItemList;

#define VMS_ITEM(i, a, b, c, d) ((i).len = (a), (i).code = (b), (i).addr = (c), (i).rlen = (d))
#define VMS_ITEM_NULL(i) ((i).len = 0, (i).code = 0, (i).addr = NULL, (i).rlen = NULL)

#endif
