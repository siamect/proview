/* 
 * Proview   $Id: sev_xtt_gtk.cpp,v 1.3 2008-10-31 12:51:30 claes Exp $
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


#include <fstream>
#include <vector>
#include <locale.h>
#include <gtk/gtk.h>

#include "pwr.h"
#include "pwr_class.h"
#include "co_error.h"
#include "rt_qcom.h"
#include "rt_errh.h"
#include "rt_sevcli.h"
#include "xtt_tbl_gtk.h"

void tbl_close_cb( void *tbl)
{
  XttTblGtk *xtttbl = (XttTblGtk *)tbl;

  delete xtttbl;
  exit(0);
}

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
  if ( EVEN(sts)) {
    printf( "Sev client init error %u\n", sts);
    exit(0);
  }

  if ( strcmp(servername, "") != 0)
    sevcli_set_servernode( &sts, sevcli, servername);

  sevcli_get_itemlist( &sts, sevcli, &items, &itemcnt);
  if ( EVEN(sts)) {
    printf( "Sev client get itemlist error %u\n", sts);
    exit(0);
  }

  gtk_init( &argc, &argv);

  setlocale( LC_NUMERIC, "POSIX");
  setlocale( LC_TIME, "en_US");

  XttTblGtk *tbl = new XttTblGtk( 0, 0, sevcli, items, itemcnt);
  tbl->close_cb = tbl_close_cb;

  gtk_main();
  return 1;
}
