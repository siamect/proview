#ifndef wb_foe_callbacks_h
#define wb_foe_callbacks_h

/* wb_foe_callbacks.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

int foe_register_callbacks (
  foe_ctx foectx
);

void foe_activate_quit (
    Widget	w,
    foe_ctx	foectx,
    XmAnyCallbackStruct	*data
);


#endif
