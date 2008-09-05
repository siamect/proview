/* 
 * Proview   $Id: rt_sevcli.h,v 1.2 2008-09-05 09:00:37 claes Exp $
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

#ifndef rt_sevcli_h
#define rt_sevcli_h

/* rt_sevcli.h -- Storage client */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
  qcom_sQid  qid;
  pwr_tNid   server;
  unsigned int msg_id;
} sevcli_sCtx, *sevcli_tCtx;

typedef struct {
  pwr_tOid 	oid;
  pwr_tAName	oname;
  pwr_tAName	aname;
  pwr_tDeltaTime storagetime;
  pwr_eType	type;
  unsigned int	size;
  pwr_tString80 description;
  pwr_tString16 unit;
  pwr_tFloat32  scantime;
} sevcli_sHistItem;


int sevcli_init( pwr_tStatus *sts, sevcli_tCtx *ctx);
int sevcli_close( pwr_tStatus *sts, sevcli_tCtx ctx);
void sevcli_set_servernid( sevcli_tCtx ctx, pwr_tNid nid);
int sevcli_set_servernode( pwr_tStatus *sts, sevcli_tCtx ctx, char *nodename);
int sevcli_get_itemlist( pwr_tStatus *sts, sevcli_tCtx ctx,
			 sevcli_sHistItem **list, unsigned int *cnt);
int sevcli_get_itemdata( pwr_tStatus *sts, sevcli_tCtx ctx, pwr_tOid oid, 
			 char *aname, pwr_tTime starttime, pwr_tTime endtime, int numpoints,
			 pwr_tTime **tbuf, void **vbuf, int *rows, 
			 pwr_eType *vtype, unsigned int *vsize);
int sevcli_delete_item( pwr_tStatus *sts, sevcli_tCtx ctx, pwr_tOid oid, char *aname);


#ifdef __cplusplus
}
#endif

#endif
