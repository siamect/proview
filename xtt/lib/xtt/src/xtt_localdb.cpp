/* xtt_localdb.h -- Local parameter database

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


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
