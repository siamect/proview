#ifndef pwr_vms_h
#define pwr_vms_h

/* pwr_vms.h -- OpenVMS specific stuff

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */


typedef struct vms_sItemList {
  short len;	/* Item buffer length */
  short code;	/* Item code */
  void *addr;	/* Item buffer address */
  short *rlen;	/* Item resultant length */
} vms_sItemList;

#define VMS_ITEM(i, a, b, c, d) ((i).len = (a), (i).code = (b), (i).addr = (c), (i).rlen = (d))
#define VMS_ITEM_NULL(i) ((i).len = 0, (i).code = 0, (i).addr = NULL, (i).rlen = NULL)

#endif
