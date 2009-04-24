/* 
 * Proview   $Id$
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

#ifndef wb_c_pndevice_h
#define wb_c_pndevice_h

/* wb_c_pndevice.h -- Profinet gsdml configure method */

typedef struct {
  pn_gsdml *gsdml;
  GsdmlAttr *attr;
  ldh_tSession ldhses;
  pwr_tAttrRef aref;
  gsdml_sModuleClass *mc;
  void *editor_ctx;
  int edit_mode;
} device_sCtx;
  
pwr_tStatus pndevice_create_ctx( ldh_tSession ldhses, pwr_tAttrRef aref, 
				 void *editor_ctx, device_sCtx **ctxp);
pwr_tStatus pndevice_init( device_sCtx *ctx);
int pndevice_help_cb( void *sctx, const char *text);
void pndevice_close_cb( void *sctx);
int pndevice_save_cb( void *sctx);

#endif
