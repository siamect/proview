#ifndef xtt_block_h
#define xtt_block_h

/* xtt_block.h -- Alarm blocking window

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#if defined __cplusplus
}
#endif

class Block {
  public:
    Block(
	void *b_parent_ctx,
	Widget b_parent_wid,
	pwr_sAttrRef *b_oar,
	char *name,
	unsigned int priv,
	pwr_tStatus *status);
    ~Block();
    int execute();
    void update();

    void 		*parent_ctx;
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
    pwr_sAttrRef	oar;

    static void create_ok( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void create_apply( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void create_toggleA( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void create_toggleB( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void create_toggleC( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void create_toggleD( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void create_toggleNo( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void activate_ok( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void activate_cancel( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
    static void activate_apply( Widget w, Block *blk, 
					 XmAnyCallbackStruct *data);
};

#endif



