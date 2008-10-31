/* 
 * Proview   $Id: wb_utl.h,v 1.14 2008-10-31 12:51:32 claes Exp $
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

#include <stdio.h>

#ifndef wb_utl_h
#define wb_utl_h

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef wb_foe_h
#include "wb_foe.h"
#endif

class wb_utl {
 public:
  wb_utl() {}
  virtual ~wb_utl() {}
  virtual int create_mainwindow( int argc, char **argv) {return 0;}
  virtual int destroy_mainwindow() {return 0;}
  virtual int utl_foe_new( const char *name, pwr_tOid	plcpgm,
			   ldh_tWBContext ldhwbctx, ldh_tSesContext ldhsesctx,
			   WFoe **foectx, int map_window, ldh_eAccess access) {return 0;}
  virtual int utl_foe_new_local( WFoe *foectx, const char *name, pwr_tOid plcpgm, 
				 ldh_tWBContext ldhwbctx, ldh_tSesContext ldhsesctx, 
				 vldh_t_node nodeobject, unsigned long windowindex, 
				 unsigned long new_window, WFoe **return_foectx, 
				 int map_window, ldh_eAccess access, 
				 foe_eFuncAccess function_access) {return 0;}
  int print_plc( ldh_tSesContext ldhses, ldh_tWBContext ldhwb,
		 char *plcstring, int document, int overview, int pdf);
  int print_plc_hier( ldh_tSesContext ldhses, ldh_tWBContext ldhwb,
		      char *hiername, char *fromname, int document,
		      int overview, int all, int pdf);
  int redraw_plc_hier( ldh_tSesContext ldhses,
		       ldh_tWBContext ldhwb, char *hiername,
		       char *fromname, int all, int templ);
  int print_document( pwr_tOid Objdid, ldh_tSesContext ldhses,
		      ldh_tWBContext ldhwb, unsigned long document,
		      unsigned long overview, int pdf, FILE *plclink);
  int redraw_windows( pwr_tObjid Objdid, ldh_tSesContext ldhses,
			      ldh_tWBContext ldhwb);
  int exec_plcpgms( ldh_tSesContext ldhses,
		    ldh_tWBContext ldhwb,
		    char *hiername,
		    char *fromname,
		    int	all,
		    int	templ,
		    int no_focode,
		    int (*cb) (void *, void *));
  int exec_plcpgm_windows( pwr_tObjid Objdid,
			   ldh_tSesContext ldhses,
			   ldh_tWBContext ldhwb,
			   int no_focode,
			   int (*cb) (void *, void *));
  int create_flow_plc( ldh_tSesContext ldhses,
		       ldh_tWBContext  ldhwb,
		       char		*hiername,
		       char		*fromname,
		       int		all,
		       int		templ);
  static int create_flow_cb( void *utl, void *foe);
  static int plc_redraw_cb( void *utl, void *foe);
  static pwr_tStatus utl_replace_symbol( ldh_tSesContext ldhses, 
					 pwr_tObjid oid,
					 pwr_sAttrRef *arp);
};

#endif

