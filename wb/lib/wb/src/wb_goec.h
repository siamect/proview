#ifndef wb_goec_h
#define wb_goec_h

/* wb_goec.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef wb_gre_h
#include "wb_gre.h"
#endif


int
goec_con_draw(
  gre_ctx       grectx,
  vldh_t_con    con_object,
  int           create_flag,
  vldh_t_node   node
);

int
goec_con_delete( 
  gre_ctx      grectx,
  vldh_t_con   con_object
);

int
goec_con_delete_noredraw( 
  gre_ctx      grectx,
  vldh_t_con   con_object
);

int
goec_con_sethighlight(
  gre_ctx         grectx,
  vldh_t_con      con_object,
  unsigned long   highlight_flag
);

int
goec_con_gethighlight(
  gre_ctx          grectx,
  vldh_t_con       con_object,
  unsigned long   *highlight_flag
);

int
goec_con_draw_ref( 
  gre_ctx       grectx,
  vldh_t_con    con_object,
  int           create_flag,
  vldh_t_node   node
);

int
goec_get_refnodetypeid(
  gre_ctx          grectx,
  unsigned long   *refnode_type_id
);

int
goec_con_delete_ref( 
  gre_ctx      grectx,
  vldh_t_con   con_object
);

int
goec_con_delete_ref_noredraw(
  gre_ctx      grectx,
  vldh_t_con   con_object
);

int
goec_con_redraw_ref(
  gre_ctx       grectx,
  vldh_t_node   node
);

int
goec_con_reconfig_ref(
  gre_ctx       grectx,
  vldh_t_node   node
);

int
goec_conpoint_get_refcons( 
  vldh_t_node      node,
  unsigned long    point,
  unsigned long    flag,
  unsigned long   *cons
);

#endif
