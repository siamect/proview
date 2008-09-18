/* 
 * Proview   $Id: xtt_ge_jprint.cpp,v 1.1 2008-09-18 15:16:18 claes Exp $
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

#include <vector.h>
#include "glow_std.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow.h"
#include "glow_curvectx.h"
#include "glow_growctx.h"
#include "glow_curveapi.h"
#include "glow_msg.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_graph.h"
#include "ge_dyn.h"
#include "ge_msg.h"
#include "ge_graph_journal.h"


int main( int argc, char *argv[])
{
  if ( argc < 2)
    printf( "Usage: xtt_ge_jprint 'jounrnalfile'\n");

  GraphJournal::print( argv[1]);
}
