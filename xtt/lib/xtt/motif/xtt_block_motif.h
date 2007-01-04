/* 
 * Proview   $Id: xtt_block_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_block_motif_h
#define xtt_block_motif_h

/* xtt_block_motif.h -- Alarm blocking window */

#ifndef xtt_block_h
# include "xtt_block.h"
#endif

class BlockMotif : public Block {
  public:
    BlockMotif( void *b_parent_ctx,
		Widget b_parent_wid,
		pwr_sAttrRef *b_oar,
		char *name,
		unsigned int priv,
		pwr_tStatus *status);
    ~BlockMotif();
    int execute();
    void update();

    Widget		parent_wid;
    Widget		toplevel;
    Widget		form;
    Widget		toggleA;
    Widget		toggleB;
    Widget		toggleC;
    Widget		toggleD;
    Widget		toggleNo;
    Widget		buttonOk;
    Widget		buttonApply;

    static void create_ok( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void create_apply( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void create_toggleA( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void create_toggleB( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void create_toggleC( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void create_toggleD( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void create_toggleNo( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void activate_ok( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void activate_cancel( Widget w, Block *blk, XmAnyCallbackStruct *data);
    static void activate_apply( Widget w, Block *blk, XmAnyCallbackStruct *data);
};

#endif



