/* 
 * Proview   $Id: xtt_block.cpp,v 1.4 2007-01-04 08:22:46 claes Exp $
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

/* xtt_bloc.cpp -- Alarm blocking window in xtt. */

#include "pwr_class.h"
#include "pwr_privilege.h"
#include "rt_gdh.h"
#include "rt_mh_outunit.h"
#include "co_cdh.h"
#include "xtt_block.h"
#include "co_lng.h"
#include "co_msg.h"

Block::~Block()
{
}

Block::Block( void *b_parent_ctx,
	      pwr_sAttrRef *b_oar,
	      char *name,
	      unsigned int priv,
	      pwr_tStatus *sts):
  parent_ctx(b_parent_ctx), oar(*b_oar)
{
  *sts = 1;
}









