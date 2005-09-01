/* 
 * Proview   $Id: xtt_localdb.cpp,v 1.2 2005-09-01 14:57:48 claes Exp $
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

/* xtt_localdb.h -- Local parameter database */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "pwr.h"
}

#include "xtt_localdb.h"
#include "rt_xnav_msg.h"

int	XNavLdb::add( char *name, xnav_eLdbType type, void *value_p)
{
  xnav_sLdbEntry 	*entry_p;
  xnav_eLdbType		get_type;
  void			*get_value_p;

  // Check that its not inserted
  if ( ODD( get( name, &get_type, &get_value_p)))
    return XNAV__ENTRYALREADYEXIST;

  entry_p = (xnav_sLdbEntry *) malloc( sizeof(*entry_p));
  strcpy( entry_p->name, name);
  entry_p->type = type;
  entry_p->value_p = value_p;
  entry_p->next = root;
  root = entry_p;

  return 1;
}

int	XNavLdb::remove( char *name)
{
  xnav_sLdbEntry *entry_p = root;
  xnav_sLdbEntry *prev_p = NULL;

  while ( entry_p)
  {
    if ( strcmp( entry_p->name, name) == 0)
    {
      if ( entry_p == NULL)
        root = entry_p->next;
      else
        prev_p->next = entry_p->next;
      free( entry_p);
      return 1;
    }
    entry_p = entry_p->next;
    prev_p = entry_p;
  }
  return 0;
}

void	XNavLdb::remove_all()
{
  xnav_sLdbEntry *entry_p = root;
  xnav_sLdbEntry *tmp_p;

  while ( entry_p)
  {
    tmp_p = entry_p->next;
    free( entry_p);
    entry_p = tmp_p;
  }
}

int	XNavLdb::get( char *name, xnav_eLdbType *type, void **value_p)
{
  xnav_sLdbEntry *entry_p = root;

  while ( entry_p)
  {
    if ( strcmp( entry_p->name, name) == 0)
    {
      *type = entry_p->type;
      *value_p = entry_p->value_p;
      return 1;
    }
    entry_p = entry_p->next;
  }
  return 0;
}
