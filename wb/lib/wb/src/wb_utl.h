/* 
 * Proview   $Id: wb_utl.h,v 1.9 2007-01-04 07:29:04 claes Exp $
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
  virtual int utl_foe_new( char *name, pwr_tOid	plcpgm,
			   ldh_tWBContext ldhwbctx, ldh_tSesContext ldhsesctx,
			   WFoe **foectx, int map_window, ldh_eAccess access) {return 0;}
  virtual int utl_foe_new_local( WFoe *foectx, char *name, pwr_tOid plcpgm, 
				 ldh_tWBContext ldhwbctx, ldh_tSesContext ldhsesctx, 
				 vldh_t_node nodeobject, unsigned long windowindex, 
				 unsigned long new_window, WFoe **return_foectx, 
				 int map_window, ldh_eAccess access, 
				 foe_eFuncAccess function_access) {return 0;}
  int print_plc( ldh_tSesContext ldhses, ldh_tWBContext ldhwb,
		 char *plcstring, int document, int overview);
  int print_plc_hier( ldh_tSesContext ldhses, ldh_tWBContext ldhwb,
		      char *hiername, char *fromname, int document,
		      int overview, int all);
  int redraw_plc( ldh_tSesContext ldhses, ldh_tWBContext ldhwb,
		  char *plcstring);
  int redraw_plc_hier( ldh_tSesContext ldhses,
		       ldh_tWBContext ldhwb, char *hiername,
		       char *fromname, int all);
  int print_document( pwr_tOid Objdid, ldh_tSesContext ldhses,
		      ldh_tWBContext ldhwb, unsigned long document,
		      unsigned long overview);
  int wb_utl::redraw_windows( pwr_tObjid Objdid, ldh_tSesContext ldhses,
			      ldh_tWBContext ldhwb);
};

#endif

