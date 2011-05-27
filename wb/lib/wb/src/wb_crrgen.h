/* 
 * Proview   $Id: wb_crrgen.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_crrgen_h
#define wb_crrgen_h

#include <iostream>
#include "pwr.h"
#include "pwr_class.h"
#include "cow_wow.h"
#include "co_tree.h"
#include "wb_session.h"

class wb_crrgen {
 protected:
  wb_session *m_sp;
  tree_sTable *m_signal_th;
  tree_sTable *m_object_th;

 public:
  wb_crrgen( wb_session *sp);
  ~wb_crrgen();
  void load( pwr_tStatus *sts, int sim = 0);
  void write( pwr_tStatus *sts);
  void write_code( pwr_tStatus *rsts);
};

#endif
