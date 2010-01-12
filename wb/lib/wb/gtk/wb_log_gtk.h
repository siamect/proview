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

#ifndef wb_log_gtk_h
#define wb_log_gtk_h

#include "pwr.h"
#include "cow_log_gtk.h"
#include "wb_log.h"

class wb_log_gtk : public wb_log
{
  CoLogGtk *m_log;

 public:
  wb_log_gtk( GtkWidget *parent) {
    m_log = new CoLogGtk( parent, wlog_cLogFile);
    m_log->set_default();
  }
  ~wb_log_gtk() { delete m_log;}

};

#endif

