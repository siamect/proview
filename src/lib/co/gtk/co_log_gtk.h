/** 
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

#ifndef co_log_gtk_h
#define co_log_gtk_h

/* co_log_gtk.h -- Logging utility */

#include "co_log.h"
#include "co_wow_gtk.h"

#include <gtk/gtk.h>



class CoLogGtk : public CoLog {
 public:
  CoLogGtk( GtkWidget *parent, const char *filename);
  ~CoLogGtk() { delete m_wow;}
};

#endif

