/* wb_gobj.c -- handle connections

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module handles the connect function in foe.
   The connect function is a relation between two objects that
   is not displayed by graphic connection in foe.  */
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wb_foe_macros.h"

#include <Xm/Xm.h>

#include "wb_ldh.h"
#if defined OS_VMS
#include "wb_hina.h"
#endif
#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_foe.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_gobj.h"
#include "wb_api.h"
#include "co_wow.h"
#include "pwr_baseclasses.h"

#define	BEEP	    putchar( '\7' );

#define	GOBJ_MAX_METHOD 25

int	gobj_get_object_m0();
int	gobj_get_object_m1();
int	gobj_get_object_m2();
int	gobj_get_object_m3();
int	gobj_get_object_m4();
int	gobj_get_object_m5();
int	gobj_get_object_m6();
int	gobj_get_object_m7();
int	gobj_get_object_m8();
int	gobj_get_object_m9();
int	gobj_get_object_m10();
int	gobj_get_object_m11();
int	gobj_get_object_m12();
int	gobj_get_object_m13();
int	gobj_get_object_m14();
int	gobj_get_object_m15();
int	gobj_get_object_m16();
int	gobj_get_object_m17();
int	gobj_get_object_m18();
int	gobj_get_object_m19();
int	gobj_get_object_m20();
int	gobj_get_object_m21();
int	gobj_get_object_m22();
int	gobj_get_object_m23();
int	gobj_get_object_m24();
int	gobj_get_object_m25();

int	(* gobj_get_object_m[30]) () = {
	&gobj_get_object_m0,
	&gobj_get_object_m1,
	&gobj_get_object_m2,
	&gobj_get_object_m3,
	&gobj_get_object_m4,
	&gobj_get_object_m5,
	&gobj_get_object_m6,
	&gobj_get_object_m7,
	&gobj_get_object_m8,
	&gobj_get_object_m9,
	&gobj_get_object_m10,
	&gobj_get_object_m11,
	&gobj_get_object_m12,
	&gobj_get_object_m13,
	&gobj_get_object_m14,
	&gobj_get_object_m15,
	&gobj_get_object_m16,
 	&gobj_get_object_m17,
 	&gobj_get_object_m18,
 	&gobj_get_object_m19,
 	&gobj_get_object_m20,
 	&gobj_get_object_m21,
 	&gobj_get_object_m22,
 	&gobj_get_object_m23,
 	&gobj_get_object_m24,
 	&gobj_get_object_m25,
	};

static int	gobj_expand_m0(	foe_ctx		foectx,
				vldh_t_node	node,
				int		compress);
static int	gobj_expand_m1(	foe_ctx		foectx,
				vldh_t_node	node,
				int		compress);
static int	gobj_expand_m2(	foe_ctx		foectx,
				vldh_t_node	node,
				int		compress);
/*_Local procedues_______________________________________________________*/



/*************************************************************************
*
* Name:		gobj_get_object()
*
* Description:	
*	Function used in class template PlcPgm's.
*	If a connection to a template object is made, this is marked
*	with the vid of a reference volume. The reference is transfered
*	at compilation to the PlcHost or PlcConnect object.
*
**************************************************************************/

static pwr_tStatus gobj_ref_replace( ldh_tSesContext ldhses, 
				     vldh_t_node node, pwr_sAttrRef *attrref)
{
  pwr_tCid cid;
  int size;
  char name[32];
  vldh_t_plc plc;
  pwr_tOid parent, plcparent;
  pwr_tStatus sts;

  plc = (node->hn.wind)->hw.plc;

  sts = ldh_ObjidToName( ldhses, attrref->Objid, ldh_eName_Object, name, sizeof(name),
			 &size);
  if ( ODD(sts) &&  strcmp( name, "Template") == 0) {
    sts = ldh_GetObjectClass( ldhses, attrref->Objid, &cid);
    if ( EVEN(sts)) return sts;

    sts = ldh_GetParent( ldhses, attrref->Objid, &parent);
    if ( EVEN(sts)) return sts;

    sts = ldh_GetParent( ldhses, plc->lp.oid, &plcparent);
    if ( EVEN(sts)) return sts;

    if ( cdh_ObjidIsEqual( parent, plcparent)) {
      // Own template object, use $PlcHost reference
      attrref->Objid.vid = ldh_cPlcHostVolume;
      attrref->Objid.oix = cid;
      return FOE__REPLACED;
    }
    else {
      // Other template object, use $PlcConnect reference
      attrref->Objid.vid = ldh_cPlcConnectVolume;
      attrref->Objid.oix = cid;
      return FOE__REPLACED;
    }
  }
  return FOE__SUCCESS;
}

static int gobj_get_select( foe_ctx foectx, pwr_sAttrRef *attrref, int *is_attr)
{
  pwr_tStatus sts;
  char str[200];
  vldh_t_plc plc = foectx->grectx->wind->hw.plc;

  if ( foectx->nav_palette_managed) {
    sts = nav_get_select( foectx->navctx, attrref, is_attr);
    if ( ODD(sts))
      return sts;
  }

  sts = wtt_get_select_first( (wtt_tCtx)plc->hp.hinactx, attrref,
				is_attr);
  if ( ODD(sts))
    return sts;

  sts = wow_GetSelection( foectx->cp.parent_wid, str, sizeof(str), foectx->objid_atom);
  if ( ODD(sts)) {
    sts = ldh_NameToAttrRef( foectx->grectx->wind->hw.ldhses, str, attrref);
    if ( ODD(sts)) {
      if ( strchr( str, '.') != 0)
	*is_attr = 1;
      else
	*is_attr = 0;
    }
    return sts;
  }
  else {
    sts = wow_GetSelection( foectx->cp.parent_wid, str, sizeof(str), XA_STRING);
    if ( ODD(sts)) {
      sts = ldh_NameToAttrRef( foectx->grectx->wind->hw.ldhses, str, attrref);
      if ( ODD(sts)) {
	if ( strchr( str, '.') != 0)
	  *is_attr = 1;
	else
	  *is_attr = 0;
      }
      return sts;
    }
  }
  return sts;
}


/*************************************************************************
*
* Name:		gobj_get_object()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	This routine make som kind of binding that is not a graphic
*	connection. Calls the connect method for the class of
*	the object. The connect method is stored in graphbody of
*	the class object (parameter connectmethod).
*	
*
**************************************************************************/

int	gobj_get_object( 
	foe_ctx		foectx,
	vldh_t_node	node,
	unsigned long	index
)
{
	int			sts, size, connectmethod;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;
 	vldh_t_plc	plc;

	/* Fix to avoid crash if foe is started form hied */
	plc = (node->hn.wind)->hw.plc;
	if ( plc->hp.hinactx == 0 )
	{
	  foe_message( foectx, "Foe must be started from the navigator to connect");
	  return FOE__SUCCESS;
	}
 
	sts = ldh_GetClassBody( (node->hn.wind)->hw.ldhses,
		node->ln.cid, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if( EVEN(sts) ) return sts;

	connectmethod = graphbody->connectmethod;
	if ( connectmethod > GOBJ_MAX_METHOD ) return 0;

	sts = (gobj_get_object_m[connectmethod]) ( foectx, node, index);

	return sts;
}




/*************************************************************************
*
* Name:		gobj_get_object_m0()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	Method for objects with nothing to connect
*
**************************************************************************/

int	gobj_get_object_m0( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	foe_message( foectx,"Nothing to connect for this object");
	BEEP;
	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m1()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getdi. Inserts the selected di-object in the
*	navigator in the parameter DiObject in a GetDi object.
*
**************************************************************************/

int	gobj_get_object_m1( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;


	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select a Di object in the navigator");
	  BEEP;
	  return sts;
	}

	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if ( EVEN(sts)) return sts;

	if ( class != pwr_cClass_Di) {
	  foe_message( foectx, "Selected object is not a di object");
	  BEEP;
	  return 0;
	}

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"DiObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m2()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
* 	Method for getdo, setdo, resdo and stodo.
*	Inserts the selected do-object in the
*	navigator in the parameter DiObject in a GetDo object.
*
**************************************************************************/

int	gobj_get_object_m2( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select a Do object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is a do object */
	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if ( EVEN(sts)) return sts;

	if ( !(class == pwr_cClass_Do ||
	       class == pwr_cClass_Po)) {
	  foe_message( foectx, "Selected object is not a do object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"DoObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m3()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getdv, setdv, resdv, stodv. Inserts the selected dv-object 
*	in the navigator in the parameter DvObject in a GetDv object.
*
**************************************************************************/

int	gobj_get_object_m3( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
 	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select a Dv object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is a dv object */
	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Dv) {
	  foe_message( foectx, "Selected object is not a dv object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"DvObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m4()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getai. Inserts the selected ai-object in the
*	navigator in the parameter AiObject in a GetAi object.
*
**************************************************************************/

int	gobj_get_object_m4( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an Ai object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is a ai object */
	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Ai) {
	  foe_message( foectx, "Selected object is not an ai object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"AiObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m5()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getao, stoao, cstoao. Inserts the selected ao-object in the
*	navigator in the parameter AoObject in a GetAo object.
*
**************************************************************************/

int	gobj_get_object_m5( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an Ao object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is an ao object */
	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Ao) {
	  foe_message( foectx, "Selected object is not an ao object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"AoObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m6()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getav, stoav, cstoav. Inserts the selected av-object in the
*	navigator in the parameter AvObject in a GetAv object.
*
**************************************************************************/

int	gobj_get_object_m6( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an Av object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is an av object */
	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Av) {
	  foe_message( foectx, "Selected object is not an Av object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"AvObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m7()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	Method for getdp, getap, setdp, setap etc.
*		Places the objdid in the first found parameter in devboy
*		with type objdid.
*
**************************************************************************/

int	gobj_get_object_m7( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	ldh_tSesContext	ldhses;
	ldh_sParDef 	*bodydef;
	int 		rows;
	int		type;
	int		i, sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;
	pwr_tTid	tid;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an attribute in the navigator");
	  BEEP;
	  return sts;
	}

	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if ( EVEN(sts)) return sts;

	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	if ( cdh_tidIsCid( tid)) {
	  foe_message( foectx,"Select an attribute in the navigator");
	  BEEP;
	  return sts;
	}

	/* Get first attribute in devbody of type pwr_eType_AttrRef */
	sts = ldh_GetObjectBodyDef( ldhses,
			node->ln.cid, "DevBody", 1, 
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	for ( i = 0; i < rows; i++) {
	  switch ( bodydef[i].ParClass ) {
	  case pwr_eClass_Input:
	    type = bodydef[i].Par->Input.Info.Type;
	    break;
	  case pwr_eClass_Intern:
	    type = bodydef[i].Par->Intern.Info.Type;
	    break;
	  case pwr_eClass_Output:
	    type = bodydef[i].Par->Output.Info.Type;
	    break;
	  default:
	    ;
 	  }
	  if ( type ==  pwr_eType_AttrRef ) {
	    if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	      gobj_ref_replace( ldhses, node, &attrref);
	      if ( EVEN(sts)) return sts;
	    }

	    /* Set the parameter value */
	    sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		bodydef[i].ParName,
		(char *)&attrref, sizeof(attrref)); 
	    if ( EVEN(sts)) return sts;

	    gre_node_update( foectx->grectx, node);
	    break;
	  }
	}
	free((char *)bodydef);
	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m8()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	Method for ShowPlcpgm to get the resetobject.
*
**************************************************************************/

int	gobj_get_object_m8( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tObjid	objdid;
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select a Di, Do or Dv object in the navigator");
	  BEEP;
	  return sts;
	}
	objdid = attrref.Objid;

	ldhses =(node->hn.wind)->hw.ldhses;

	/* Check that the objdid is a di,do or dv object */
	sts = ldh_GetObjectClass( ldhses, objdid, &class);
	if (EVEN(sts)) return sts;

	if ( ( class != pwr_cClass_Di) &&
	     ( class != pwr_cClass_Do) &&
	     ( class != pwr_cClass_Po) &&
	     ( class != pwr_cClass_Dv) )
	{
	  foe_message( foectx, "Reset object has to be a di, do or dv.");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		plc->lp.oid, 
		"DevBody",
		"ResetObject",
		(char *)&objdid, sizeof(objdid)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m9()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	Method for getpi
*	Method for getpi. Inserts the selected co-object in the
*	navigator in the parameter PiObject in a GetPi object.
*
**************************************************************************/

int	gobj_get_object_m9( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select a Co object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is a co object */
	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Co) {
	  foe_message( foectx, "Selected object is not a co object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"CoObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m10()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Unoccupied.
*
**************************************************************************/

int	gobj_get_object_m10( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	pwr_sAttrRef	nattrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

        sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts) || is_attr == 1) { 
	  foe_message( foectx,"Select an object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Set the PlcConnect attribute in the current object */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"RtBody", "PlcConnect",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) { 
	  foe_message( foectx,"No PlcConnect attribute in object");
	  BEEP;
	  return sts;
	}

	/* Set the PlcConnect attribute in the connected object */
	nattrref = cdh_ObjidToAref( node->ln.oid);
	sts = ldh_SetObjectPar( ldhses, attrref.Objid,
		"RtBody", "PlcConnect",
		(char *)&nattrref, sizeof(nattrref)); 

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m11()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for reset_so. If one object (including or excluding selection
*	of the reset_so object) is selected in foe this
*	object is taken as the orderobject and inserted in the
*	parameter OrderObject in the reset_so. Otherwise the selected
*	object in the navigator is taken as the orderobject.
*	
*
**************************************************************************/

int	gobj_get_object_m11( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tObjid	objdid;
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		object;
        pwr_sAttrRef		attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( ((node_count == 1) && (*nodelist == node)) ||
	      ( node_count == 0) ) 
	{
	  /* Take the orderobject from the navigator */
	  sts = gobj_get_select( foectx, &attrref, &is_attr);
	  if ( EVEN(sts)) { 
	    foe_message( foectx,"Select an order object in the navigator or in the current window");
	    BEEP;
	    return sts;
	  }
	  objdid = attrref.Objid;
	}
	else if ( (node_count == 2) && 
		(( *nodelist == node) || ( *(nodelist + 1) == node)))
	{
	  /* Check if the other node is a orderobject */
	  if ( *nodelist == node)
	    object = *(nodelist + 1);
	  else
	    object = *nodelist;
	  if ( object->ln.cid == pwr_cClass_order)
	  {
	    objdid = object->ln.oid;
	  }
	  else
	  {
	    foe_message( foectx,"Select an order object in the navigator or in the current window first");
	    BEEP;
	    return 0;
	  }
	}
	else if ( node_count == 1)
	{
	  /* Check if the other node is a orderobject */
	  object = *nodelist;
	  if ( object->ln.cid == pwr_cClass_order)
	  {
	    objdid = object->ln.oid;
	  }
	  else
	  {
	    foe_message( foectx,"Select an order object in the navigator or in the current window first");
	    BEEP;
	    return 0;
	  }
	}
	else
	{
	  foe_message( foectx,"Select an order object in the navigator or in the current window first");
	    BEEP;
	  return 0;
	}
	if ( node_count > 0) XtFree((char *) nodelist);
	
	/* Check that the objdid is a do object */
	sts = ldh_GetObjectClass( ldhses, objdid, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_order)
	{
	  foe_message( foectx, "Selected object is not an order object");
	  BEEP;
	  return 0;
	}
	
	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"OrderObject",
		(char *)&objdid, sizeof(objdid)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);
	gre_unselect( foectx->grectx);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gobj_get_object_m12()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for PageRef. If one object (including or excluding selection
*	of the PageRef object) is selected in foe this
*	object is taken as the documentobject and inserted in the
*	parameter PageAttr in the PageRef. Otherwise the selected
*	object in the navigator is taken as the document-object.
*	
*
**************************************************************************/

int	gobj_get_object_m12( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tObjid	objdid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	unsigned long		node_count;
	vldh_t_node		*nodelist;
	vldh_t_node		object;
	char		name[200];
	int		size;
	pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	gre_get_selnodes( foectx->grectx, &node_count, &nodelist);

	if ( ((node_count == 1) && (*nodelist == node)) ||
	      ( node_count == 0) ) 
	{
	  /* Take the document from the navigator */
	  sts = gobj_get_select( foectx, &attrref, &is_attr);
	  if ( EVEN(sts)) { 
	    foe_message( foectx,"Select a document object in the navigator or in the current window first");
	    BEEP;
	    return sts;
	  }
	  objdid = attrref.Objid;
	}
	else if ( (node_count == 2) && 
		(( *nodelist == node) || ( *(nodelist + 1) == node)))
	{
	  /* Check if the other node is a documentobject */
	  if ( *nodelist == node)
	    object = *(nodelist + 1);
	  else
	    object = *nodelist;
	  if ( vldh_check_document( ldhses,  object->ln.oid))
	  {
	    objdid = object->ln.oid;
	  }
	  else
	  {
	    foe_message( foectx,"Select a document object in the navigator or in the current window first");
	    BEEP;
	    return 0;
	  }
	}
	else if ( node_count == 1)
	{
	  /* Check if the other node is a orderobject */
	  object = *nodelist;
	  if ( vldh_check_document( ldhses,  object->ln.oid))
	  {
	    objdid = object->ln.oid;
	  }
	  else
	  {
	    foe_message( foectx,"Select a document object in the navigator or in the current window first");
	    BEEP;
	    return 0;
	  }
	}
	else
	{
	  foe_message( foectx,"Select a document object in the navigator or in the current window first");
	    BEEP;
	  return 0;
	}
	if ( node_count > 0) XtFree((char *) nodelist);
	

	/* Check that the objdid is a do object */
	if ( !vldh_check_document( ldhses,  objdid))
	{
	  foe_message( foectx, "Selected object is not a document object");
	  BEEP;
	  return 0;
	}
	
	sts = ldh_ObjidToName( ldhses, objdid, ldh_eName_ArefVol,
		name, sizeof(name), &size);
	if ( EVEN(sts)) return sts;

	strcat( name, ".Page");
	sts = ldh_NameToAttrRef( ldhses, name, &attrref);
	if ( EVEN(sts)) return sts;

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid,
		"DevBody",
		"PageAttr",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);
	gre_unselect( foectx->grectx);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gobj_get_object_m13()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for GetData. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m13( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	pwr_sAttrRef	attrref;
	int		is_attr;


	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	/* Take the object from the navigator */
	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an object in the navigator");
	  BEEP;
	  return sts;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"DataObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);
	gre_unselect( foectx->grectx);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m14()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for GetAgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m14( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId 	create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an analog signal or attribute in the navigator");
	  BEEP;
	  return sts;
	}

	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;
	
	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid) {
	case pwr_eType_Float32:
	  /* Create a StoAp */
	  create_classid = pwr_cClass_GetAp;
	  strcpy( parname, "ApObject");
	  break;
	case pwr_eType_Int8:
	case pwr_eType_UInt8:
	case pwr_eType_Int16:
	case pwr_eType_UInt16:
	case pwr_eType_Int32:
	case pwr_eType_UInt32:
	  /* Create a GetIpToA */
	  create_classid = pwr_cClass_GetIpToA;
	  strcpy( parname, "IpObject");
	  break;
	case pwr_cClass_Ai:
	  strcpy( parname, "AiObject");
	  create_classid = pwr_cClass_GetAi;
	  break;
	case pwr_cClass_Ao:
	  strcpy( parname, "AoObject");
	  create_classid = pwr_cClass_GetAo;
	  break;
	case pwr_cClass_Av:
	  strcpy( parname, "AvObject");
	  create_classid = pwr_cClass_GetAv;
	  break;
	default:
	  foe_message( foectx,"Select an analog signal or attribute in the navigator");
	  BEEP;
	  return 0;
	}

	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;

	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node) {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	    
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
				new_node->ln.oid, 
				"DevBody",
				parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;
	
	gre_node_update( foectx->grectx, new_node);
	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m15()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for GetDgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m15( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
        pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId 	create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an digital signal or attribute in the navigator");
	  BEEP;
	  return sts;
	}

	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;
	
	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid) {
	case pwr_eType_Boolean:
	  /* Create a GetDp */
	  create_classid = pwr_cClass_GetDp;
	  strcpy( parname, "DpObject");
	  break;
	case pwr_cClass_Di:
	  strcpy( parname, "DiObject");
	  create_classid = pwr_cClass_GetDi;
	  break;
	case pwr_cClass_Do:
	  strcpy( parname, "DoObject");
	  create_classid = pwr_cClass_GetDo;
	  break;
	case pwr_cClass_Dv:
	  strcpy( parname, "DvObject");
	  create_classid = pwr_cClass_GetDv;
	  break;
	default:
	  foe_message( foectx,"Select an digital signal or attribute in the navigator");
	  BEEP;
	  return 0;
	}
	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;

	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node) {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
				new_node->ln.oid, 
				"DevBody",
				parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, new_node);
	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m16()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for StoAgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m16( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId 	create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an digital signal or attribute in the navigator");
	  BEEP;
	  return sts;
	}

	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;

	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid)  {
	case pwr_eType_Float32:
	  /* Create a StoAp */
	  strcpy( parname, "Object");
	  create_classid = pwr_cClass_stoap;
	  break;
	case pwr_eType_Int8:
	case pwr_eType_UInt8:
	case pwr_eType_Int16:
	case pwr_eType_UInt16:
	case pwr_eType_Int32:
	case pwr_eType_UInt32:
	  /* Create a StoAtoIp */
	  strcpy( parname, "Object");
	  create_classid = pwr_cClass_StoAtoIp;
	  break;
	case pwr_cClass_Ai:
	  strcpy( parname, "AiObject");
	  create_classid = pwr_cClass_stoai;
	  break;
	case pwr_cClass_Ao:
	  strcpy( parname, "AoObject");
	  create_classid = pwr_cClass_stoao;
	  break;
	case pwr_cClass_Av:
	  strcpy( parname, "AvObject");
	  create_classid = pwr_cClass_stoav;
	  break;
	default:
	  foe_message( foectx,"Select an digital signal or attribute in the navigator");
	  BEEP;
	  return 0;
	}

	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;

	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node) {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	    
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
				new_node->ln.oid, 
				"DevBody",
				parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, new_node);
	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}



/*************************************************************************
*
* Name:		gobj_get_object_m17()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for StoDgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m17( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId 	create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an digital signal in the navigator");
	  BEEP;
	  return sts;
	}

	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;

	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid)  {
	case pwr_eType_Boolean:
	  /* Create a StoDp */
	  strcpy( parname, "Object");
	  create_classid = pwr_cClass_stodp;
	  break;
	case pwr_cClass_Di:
	  strcpy( parname, "DiObject");
	  create_classid = pwr_cClass_stodi;
	  break;
	case pwr_cClass_Do:
	  strcpy( parname, "DoObject");
	  create_classid = pwr_cClass_stodo;
	  break;
	case pwr_cClass_Dv:
	  strcpy( parname, "DvObject");
	  create_classid = pwr_cClass_stodv;
	  break;
	default:
	  foe_message( foectx,"Select an digital signal in the navigator");
	  BEEP;
	  return 0;
	}

	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;
	
	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node) {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	    
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
				new_node->ln.oid, 
				"DevBody",
				parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, new_node);

	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m18()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getsv, stosv, cstosv. Inserts the selected sv-object in the
*	navigator in the parameter SvObject in a GetSv object.
*
**************************************************************************/

int	gobj_get_object_m18( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select Sv object in the navigator");
	  BEEP;
	  return sts;
	}


	/* Check that the objdid is an av object */
	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Sv) {
	  foe_message( foectx, "Selected object is not a Sv object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"SvObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m19()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for GetSgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m19( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId    create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select string value or attribute in the navigator");
	  BEEP;
	  return sts;
	}

 	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;

	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid)  {
	case pwr_eType_String:
	  /* Create a GetSp */
	  create_classid = pwr_cClass_GetSp;
	  strcpy( parname, "SpObject");
	  break;
	case pwr_cClass_Sv:
	  strcpy( parname, "SvObject");
	  create_classid = pwr_cClass_GetSv;
	  break;
	default:
	  foe_message( foectx,"Select string value or attribute in the navigator");
	  BEEP;
	  return 0;
	}
	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;

	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node) {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	    
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
				new_node->ln.oid, 
				"DevBody",
				parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;
	
	gre_node_update( foectx->grectx, new_node);

	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gobj_get_object_m20()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for StoSgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m20( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId 	create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 	
	  foe_message( foectx,"Select a string value or attribute in the navigator");
	  BEEP;
	  return sts;
	}

 	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;

	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid) {
	case pwr_eType_String:
	  /* Create a StoSp */
	  strcpy( parname, "Object");
	  create_classid = pwr_cClass_stosp;
	  break;
	case pwr_cClass_Sv:
	  strcpy( parname, "SvObject");
	  create_classid = pwr_cClass_stosv;
	  break;
	default:
	  foe_message( foectx,"Select a string value or attribute in the navigator");
	  BEEP;
	  return 0;
	}

	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;

	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node)  {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	    
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
				new_node->ln.oid, 
				"DevBody",
				parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, new_node);

	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gobj_get_object_m21()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getiv, stoiv, cstoiv. Inserts the selected iv-object in the
*	navigator in the parameter IvObject in a GetIv object.
*
**************************************************************************/

int	gobj_get_object_m21( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an Iv object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is an iv object */
 	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Iv) {
	  foe_message( foectx, "Selected object is not a Iv object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"IvObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m22()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getii, stoii, cstoii. Inserts the selected ii-object in the
*	navigator in the parameter IiObject in a GetIi object.
*
**************************************************************************/

int	gobj_get_object_m22( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an Ii object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is an ii object */
 	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Ii) {
	  foe_message( foectx, "Selected object is not a Ii object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"IiObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_get_object_m23()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for getio, stoio, cstoio. Inserts the selected io-object in the
*	navigator in the parameter IoObject in a GetIo object.
*
**************************************************************************/

int	gobj_get_object_m23( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tClassId	class;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
        pwr_sAttrRef	attrref;
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an Io object in the navigator");
	  BEEP;
	  return sts;
	}

	/* Check that the objdid is an io object */
 	sts = ldh_GetAttrRefTid( ldhses, &attrref, &class);
	if (EVEN(sts)) return sts;

	if ( class != pwr_cClass_Io) {
	  foe_message( foectx, "Selected object is not a Io object");
	  BEEP;
	  return 0;
	}
	
	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses,
		node->ln.oid, 
		"DevBody",
		"IoObject",
		(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gobj_get_object_m24()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for GetIgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m24( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId 	create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an integer signal or attribute in the navigator");
	  BEEP;
	  return sts;
	}

 	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;

	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid) {
	case pwr_eType_Int32:
	case pwr_eType_UInt32:
	case pwr_eType_Int16:
	case pwr_eType_UInt16:
	case pwr_eType_Int8:
	case pwr_eType_UInt8:
	  /* Create a GetIp */
	  create_classid = pwr_cClass_GetIp;
	  strcpy( parname, "IpObject");
	  break;
	case pwr_cClass_Ii:
	  strcpy( parname, "IiObject");
	  create_classid = pwr_cClass_GetIi;
	  break;
	case pwr_cClass_Io:
	  strcpy( parname, "IoObject");
	  create_classid = pwr_cClass_GetIo;
	  break;
	case pwr_cClass_Iv:
	  strcpy( parname, "IvObject");
	  create_classid = pwr_cClass_GetIv;
	  break;
	default:
	  foe_message( foectx,"Select an integer signal or attribute in the navigator");
	  BEEP;
	  return 0;
	}

	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;

	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node) {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	    
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses, new_node->ln.oid, 
				"DevBody", parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, new_node);

	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		gobj_get_object_m25()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	foectx		I	foe context.
* vldh_t_node	node		I	vldh node.
* unsigned long	index		I	index indicating if the click has hit
*					the upper or the lower part of the node.
*
* Description:	
*	Method for StoIgeneric. The selected object in the navigator is 
*	inserted.
*	
*
**************************************************************************/

int	gobj_get_object_m25( foectx, node, index)
foe_ctx		foectx;
vldh_t_node	node;
unsigned long	index;
{
	pwr_tTid	tid;
	ldh_tSesContext	ldhses;
	int		sts;
	vldh_t_plc	plc;
	vldh_t_con  	*con_list;
	vldh_t_con  	*con_ptr;
	vldh_t_node	new_node, source, dest;
	unsigned long	source_point, dest_point;
	unsigned long	con_count;
	int		j;
        pwr_sAttrRef	attrref;
    	pwr_tClassId 	create_classid;
	char		parname[40];
	int		is_attr;

	/* Get the selected object in the navigator */
	plc = (node->hn.wind)->hw.plc;
	ldhses =(node->hn.wind)->hw.ldhses;

	sts = gobj_get_select( foectx, &attrref, &is_attr);
	if ( EVEN(sts)) { 
	  foe_message( foectx,"Select an integer signal in the navigator");
	  BEEP;
	  return sts;
	}

 	sts = ldh_GetAttrRefTid( ldhses, &attrref, &tid);
	if (EVEN(sts)) return sts;

	if ( !cdh_tidIsCid(tid)) {
	  sts = ldh_GetAttrRefType( ldhses, &attrref, &tid);
	  if (EVEN(sts)) return sts;
	}

	sts = vldh_get_cons_node( node, &con_count, &con_list);
	if ( EVEN(sts)) return sts;

	switch ( tid) {
	case pwr_eType_Int32:
	case pwr_eType_UInt32:
	case pwr_eType_Int16:
	case pwr_eType_UInt16:
	case pwr_eType_Int8:
	case pwr_eType_UInt8:
	  /* Create a StoIp */
	  strcpy( parname, "Object");
	  create_classid = pwr_cClass_StoIp;
	  break;
	case pwr_cClass_Ii:
	  strcpy( parname, "IiObject");
	  create_classid = pwr_cClass_stoii;
	  break;
	case pwr_cClass_Io:
	  strcpy( parname, "IoObject");
	  create_classid = pwr_cClass_stoio;
	  break;
	case pwr_cClass_Iv:
	  strcpy( parname, "IvObject");
	  create_classid = pwr_cClass_stoiv;
	  break;
	default:
	  foe_message( foectx,"Select an integer signal in the navigator");
	  BEEP;
	  return 0;
	}

	sts = gre_create_node( foectx->grectx, create_classid, 
			       node->ln.x, node->ln.y, &new_node);
	if ( EVEN(sts)) return sts;
	
	/* Create new connections */
	con_ptr = con_list;
	for ( j = 0; j < con_count; j++) {
	  if ( (*con_ptr)->hc.source_node == node) {
	    source = new_node;
	    source_point = 0;
	    dest = (*con_ptr)->hc.dest_node;
	    dest_point = (*con_ptr)->lc.dest_point;
	  }
	  else {
	    dest = new_node;
	    dest_point = 0;
	    source = (*con_ptr)->hc.source_node;
	    source_point = (*con_ptr)->lc.source_point;
	  }
	  sts = gre_create_con( foectx->grectx, (*con_ptr)->lc.cid, 
				source, source_point,
				dest, dest_point, (*con_ptr)->lc.drawtype);
	  if (EVEN(sts)) return sts;
	  con_ptr++;
	}
	    
	/* Remove old node and connections */
	gre_delete_node( foectx->grectx, node);
	foectx->popupmenu_node = 0;

	if ( cdh_IsClassVolume( node->ln.oid.vid)) {
	  gobj_ref_replace( ldhses, node, &attrref);
	  if ( EVEN(sts)) return sts;
	}

	/* Set the parameter value */
	sts = ldh_SetObjectPar( ldhses, new_node->ln.oid, 
				"DevBody", parname,
				(char *)&attrref, sizeof(attrref)); 
	if ( EVEN(sts)) return sts;
	
	gre_node_update( foectx->grectx, new_node);

	if ( con_count > 0) XtFree((char *) con_list);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_expand_m0()
*
* Description:	Method for objects with nothing to expand
*
**************************************************************************/

int	gobj_expand(	foe_ctx		foectx,
			vldh_t_node	node,
			int		compress)
{
	int	sts;

	switch( node->ln.cid)
	{
          case pwr_cClass_GetDv:
          case pwr_cClass_GetDi:
          case pwr_cClass_GetDo:
          case pwr_cClass_GetAv:
          case pwr_cClass_GetAi:
          case pwr_cClass_GetAo:
          case pwr_cClass_GetAp:
          case pwr_cClass_GetDp:
          case pwr_cClass_GetIp:
          case pwr_cClass_GetIpToA:
          case pwr_cClass_GetPi:
	  case pwr_cClass_stodv:
	  case pwr_cClass_stoav:
	  case pwr_cClass_stoai:
	  case pwr_cClass_stoao:
	  case pwr_cClass_stodi:
	  case pwr_cClass_stodo:
	  case pwr_cClass_setdv:
	  case pwr_cClass_setdi:
	  case pwr_cClass_setdo:
	  case pwr_cClass_resdv:
	  case pwr_cClass_resdi:
	  case pwr_cClass_resdo:
	  case pwr_cClass_stopi:
          case pwr_cClass_stoap:
          case pwr_cClass_stodp:
          case pwr_cClass_setdp:
          case pwr_cClass_resdp:
          case pwr_cClass_StoIp:
          case pwr_cClass_StoAtoIp:
          case pwr_cClass_cstoao:
          case pwr_cClass_cstoav:
          case pwr_cClass_cstoai:
          case pwr_cClass_CStoIp:
          case pwr_cClass_CStoAtoIp:
          case pwr_cClass_cstoap:
          case pwr_cClass_GetData:
          case pwr_cClass_stosv:
          case pwr_cClass_cstosv:
          case pwr_cClass_GetSv:
          case pwr_cClass_stosp:
          case pwr_cClass_cstosp:
          case pwr_cClass_GetSp:
          case pwr_cClass_stonumsp:
          case pwr_cClass_cstonumsp:
          case pwr_cClass_GetIi:
          case pwr_cClass_GetIo:
          case pwr_cClass_GetIv:
          case pwr_cClass_stoii:
          case pwr_cClass_stoio:
          case pwr_cClass_stoiv:
          case pwr_cClass_cstoii:
          case pwr_cClass_cstoio:
          case pwr_cClass_cstoiv:
	    sts = gobj_expand_m1( foectx, node, compress);
	    break;
          case pwr_cClass_and:
          case pwr_cClass_or:
          case pwr_cClass_sum:
	    sts = gobj_expand_m2( foectx, node, compress);
	    break;
	  default:
	    sts = gobj_expand_m0( foectx, node, compress);
	}
	return sts;
}

/*************************************************************************
*
* Name:		gobj_expand_m0()
*
* Description:	Method for objects with nothing to expand
*
**************************************************************************/

static int	gobj_expand_m0(	foe_ctx		foectx,
				vldh_t_node	node,
				int		compress)
{
	foe_message( foectx,"Nothing to expand for this object");
	BEEP;
	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_expand_m1()
*
* Description:	Method for Get, Sto, Set and Res objects.
*
**************************************************************************/

static int	gobj_expand_m1(	foe_ctx		foectx,
				vldh_t_node	node,
				int		compress)
{
	ldh_sParDef 	*bodydef;
	int 		rows;
	char		attrname[80];	
	pwr_tUInt32	*segments_p, segments;
	int		sts, size;
	ldh_tSesContext	ldhses;

	ldhses =(node->hn.wind)->hw.ldhses;
	
	/* Get the devbody parameters for this class */
	sts = ldh_GetObjectBodyDef( ldhses,
			node->ln.cid, "DevBody", 1,
			&bodydef, &rows);
	if ( EVEN(sts) ) return sts;

	strcpy( attrname, bodydef[0].ParName);
	strcat( attrname, "Segments");

	sts = ldh_GetObjectPar( ldhses, node->ln.oid, "DevBody",
			attrname, (char **)&segments_p, &size);
	if (EVEN(sts)) return sts;
	  
	segments = *segments_p;
	free((char *) bodydef);	
	free((char *) segments_p);
	if ( !compress)
	  segments++;
	else
	{
	  segments--;
	  if ( segments < 1)
	    segments = 1;
	}
	sts = ldh_SetObjectPar( ldhses, node->ln.oid, "DevBody",
			attrname, (char *)&segments, sizeof(segments));
	if (EVEN(sts)) return sts;

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		gobj_expand_m1()
*
* Description:	Method for and and or objects.
*
**************************************************************************/

static int	gobj_expand_m2(	foe_ctx		foectx,
				vldh_t_node	node,
				int		compress)
{
	int		sts, size;
	ldh_tSesContext	ldhses;
	pwr_sPlcNode	*nodebuffer;
	pwr_eClass	class;
	int		max_input = 8;
	unsigned int	m;
	int		i, i_max;

	ldhses =(node->hn.wind)->hw.ldhses;
	
	sts = ldh_GetObjectBuffer( ldhses, node->ln.oid, "DevBody",
			"PlcNode", &class,
			(char **)&nodebuffer, &size);
	if (EVEN(sts)) return sts;
	  
	i_max = 0;
	for ( i = 0; i < max_input; i++)
	{
	  m = 1 << i;
	  if ( nodebuffer->mask[0] & m)
	    i_max = i;
	}
	if ( !compress)
	{
	  if ( i_max == max_input - 1)
	  {
	    free((char *) nodebuffer);
	    return FOE__SUCCESS;
	  }
	  nodebuffer->mask[0] |= 1 << (i_max + 1);
	}
	else
	{
	  if ( i_max == 0)
	  {
	    free((char *) nodebuffer);
	    return FOE__SUCCESS;
	  }
	  nodebuffer->mask[0] &= ~(1 << (i_max));
	}

	sts = ldh_SetObjectBuffer( ldhses, node->ln.oid, "DevBody",
		"PlcNode",
		(char *)nodebuffer);
	if (EVEN(sts)) return sts;

	free((char *) nodebuffer);

	gre_node_update( foectx->grectx, node);

	return FOE__SUCCESS;
}

