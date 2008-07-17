/* 
 * Proview   $Id: sev_xtt_gtk.cpp,v 1.1 2008-07-17 11:18:31 claes Exp $
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


#include <gtk/gtk.h>

#include "pwr.h"
#include "pwr_class.h"
#include "xtt_tbl_gtk.h"
#include "co_error.h"
#include "rt_qcom.h"
#include "rt_errh.h"
#include "rt_sevcli.h"

int main(  int argc, char *argv[])
{
  pwr_tStatus 	sts;
  qcom_sAid    	aid;
  sevcli_tCtx 	sevcli;
  sevcli_sHistItem *items;
  unsigned int 	itemcnt;
  char servername[40] = "";
  
  // Get options
  for ( int i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-s") == 0 && i + 1 < argc) {
      strncpy( servername,  argv[i+1], sizeof(servername));
      i++;
    }
  }

  sts = qcom_Init( &sts, &aid, "sev_xtt");
  if ( EVEN(sts)) co_error(sts);

  sevcli_init( &sts, &sevcli);
  if ( EVEN(sts)) co_error(sts);

  if ( strcmp(servername, "") != 0)
    sevcli_set_servernode( &sts, sevcli, servername);

  sevcli_get_itemlist( &sts, sevcli, &items, &itemcnt);
  if ( EVEN(sts)) co_error(sts);

  gtk_init( &argc, &argv);

  new XttTblGtk( 0, 0, sevcli, items, itemcnt);

  gtk_main();
  return 1;
}
