/** 
 * Proview   $Id: co_wow_gtk.cpp,v 1.15 2008-12-01 16:42:35 claes Exp $
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

/* cow_log_gtk.cpp -- logging utitlity */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "pwr.h"
#include "cow_log_gtk.h"
#include "co_dcli.h"

CoLogGtk::CoLogGtk( GtkWidget *parent, const char *filename) :
  CoLog( filename)
{
  if ( parent)
    m_wow = new CoWowGtk( parent);
}






