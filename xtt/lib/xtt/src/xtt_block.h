/* 
 * Proview   $Id: xtt_block.h,v 1.3 2007-01-04 08:22:46 claes Exp $
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

#ifndef xtt_block_h
#define xtt_block_h

/* xtt_block.h -- Alarm blocking window */

#ifndef pwr_h
# include "pwr.h"
#endif

class CoWow;

class Block {
  public:
    Block(
	void *b_parent_ctx,
	pwr_sAttrRef *b_oar,
	char *name,
	unsigned int priv,
	pwr_tStatus *status);
    virtual ~Block();

    virtual int execute() {return 0;}
    virtual void update() {}

    void 		*parent_ctx;
    pwr_sAttrRef	oar;
    CoWow		*wow;
};

#endif



