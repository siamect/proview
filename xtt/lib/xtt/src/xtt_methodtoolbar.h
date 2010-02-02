/* 
 * Proview   $Id: xtt_methodtoolbar.h,v 1.1 2007-01-17 06:19:57 claes Exp $
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

#ifndef xtt_methodtoolbar_h
#define xtt_methodtoolbar_h

#include <string.h>
#include "pwr.h"

class XNav;

typedef struct {
  char name[80];
  char method[80];
  char filter[80];
  char image[80];
  char tooltip[80];
} xtt_sMethodButton;

typedef enum {
  mt_mMethod_Help 		= 1 << 0,
  mt_mMethod_Note 		= 1 << 1,
  mt_mMethod_Trend 		= 1 << 2,
  mt_mMethod_Fast 		= 1 << 3,
  mt_mMethod_History 		= 1 << 4,
  mt_mMethod_Photo 		= 1 << 5,
  mt_mMethod_Graph 		= 1 << 6,
  mt_mMethod_RtNavigator       	= 1 << 7,
  mt_mMethod_ObjectGraph 	= 1 << 8,
  mt_mMethod_OpenPlc 		= 1 << 9,
  mt_mMethod_DataSheet 		= 1 << 10,
  mt_mMethod_HistEvent 		= 1 << 11,
  mt_mMethod_BlockEvents 	= 1 << 12,
  mt_mMethod_OpenObject 	= 1 << 13,
  mt_mMethod_Crossreferences 	= 1 << 14,
  mt_mMethod_CircuitDiagram 	= 1 << 15,
  mt_mMethod_HelpClass 		= 1 << 16
} mt_mMethod;

class XttMethodToolbar {
 public:
  static const int m_size = 17;
  static xtt_sMethodButton m_data[m_size];
  XNav *m_xnav;
  void *m_parent_ctx;
  unsigned int m_method_mask;
  char m_tooltip_suffix[80];
  int (*get_select_cb)( void *, pwr_tAttrRef *, int *);

  XttMethodToolbar( void *parent_ctx, void *xnav, unsigned int method_mask, const char *tooltip_suffix) :  
    m_xnav((XNav *)xnav), m_parent_ctx(parent_ctx), m_method_mask(method_mask), 
    get_select_cb(0) { strcpy( m_tooltip_suffix, tooltip_suffix); }
  virtual ~XttMethodToolbar() {}
  virtual void set_sensitive() {}
};

#endif
