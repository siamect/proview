/* 
 * Proview   $Id: xtt_c_pb_dp_slave.h,v 1.1 2007-01-04 08:44:40 claes Exp $
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
 **/

#ifndef xtt_c_pb_dp_slave_h
#define xtt_c_pb_dp_slave_h

/* xtt_c_pb_dp_slave.h -- Profibus gsd configure method */

typedef struct {
  pb_gsd *gsd;
  GsdAttr *attr;
  pwr_tAttrRef aref;
  gsd_sModuleClass *mc;
  void *editor_ctx;
} xtt_slave_sCtx;

pwr_tStatus xtt_pb_dp_slave_create_ctx( pwr_tAttrRef aref, 
					void *editor_ctx, xtt_slave_sCtx **ctxp);
int xtt_pb_dp_slave_help_cb( void *sctx, char *text);
void xtt_pb_dp_slave_close_cb( void *sctx);
int xtt_pb_dp_slave_save_cb( void *sctx);

#endif
