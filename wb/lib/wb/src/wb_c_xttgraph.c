/* wb_c_xttgraph.c -- work bench methods of the XttGraph class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <X11/Intrinsic.h>
#undef Status
#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "ge.h"
#include "wb_pwrb_msg.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"
#include "co_cdh.h"


/*----------------------------------------------------------------------------*\
  To open Ge.
\*----------------------------------------------------------------------------*/

static pwr_tStatus OpenGraph (
  ldh_sMenuCall *ip
)
{
  int 		sts;
  char		*action;
  int		size;
  char		graph_name[80];

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			"Action", &action, &size); 
  if ( EVEN(sts)) return sts;

  cdh_ToLower( graph_name, action);
  free( (char *)action);
  if ( strstr( graph_name, ".pwg"))
  {
    ge_new( NULL, ip->WindowContext, ip->PointedSession, 0, graph_name);
    return 1;
  }
  return PWRB__GRAPHACTION;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(XttGraph) = {
  pwr_BindMethod(OpenGraph),
  pwr_NullMethod
};





