/* ini_objpara.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.


   Routines used to initialize parameters, *ActualValue and
   ValueIndex in AI, AO, DI, DO, DV and AV-object.

   Initialize parameters, *RawValue, *AbsValue and ValueIndex
   in CO-object. 

   Count number of AI, AO, DI, DO, DV, CO and AV-object and 
   store it in RTDB (rtdb_node_config_datatype-object).  */


#include <string.h>
#include "pwr.h"
#include "pwr_class.h"
#include "rt_errh.h"
#include "rt_ini_msg.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "pwr_baseclasses.h"
#include "ini_objpara.h"

static pwr_tInt32 ObjParaAI(pwr_sNode *NodDataP);
static pwr_tInt32 ObjParaAO(pwr_sNode *NodDataP);
static pwr_tInt32 ObjParaDI(pwr_sNode *NodDataP);
static pwr_tInt32 ObjParaDO(pwr_sNode *NodDataP);
static pwr_tInt32 ObjParaAV(pwr_sNode *NodDataP, pwr_tFullName NodeObjName);
static pwr_tInt32 ObjParaDV(pwr_sNode *NodDataP, pwr_tFullName NodeObjName);
static pwr_tInt32 ObjParaCO(pwr_sNode *NodDataP);


/* Routines used to initialize parameters, ActualValue and
   ValueIndex in AI, AO, DI, DO, DV and AV-object. 

   Count number of AI, AO, DI, DO, DV and AV-object and 
   store it in RTDB (pwr_sNode-object).  */

#define IfErrInParThenLog(sts, str)\
  if (EVEN(sts)) errh_Error("Failed to init %s, reason:\n%m", str, sts)

pwr_tInt32	ini_ObjPara()

{

    pwr_tInt32	sts;
    pwr_sNode	*NodDataObjP;
    pwr_tObjid	NodDataObjId;

    pwr_tFullName  NodeObjName;

    /* Get pointer to $NODE object */

    sts = gdh_GetNodeObject (0, &NodDataObjId);
    if (ODD (sts)) sts = gdh_ObjidToPointer(NodDataObjId, (pwr_tAddress *)&NodDataObjP);
    if (EVEN (sts)) return sts;

    sts = gdh_ObjidToName ( NodDataObjId, NodeObjName, sizeof(NodeObjName), cdh_mNName);
    if (EVEN (sts)) return sts;

    /* Call function for each object type */

    sts = ObjParaDI(NodDataObjP);
    IfErrInParThenLog(sts, "Di");
    
    sts = ObjParaDO(NodDataObjP);
    IfErrInParThenLog(sts, "Do");

    sts = ObjParaAI(NodDataObjP);
    IfErrInParThenLog(sts, "Ai");

    sts = ObjParaAO(NodDataObjP);
    IfErrInParThenLog(sts, "Ao");

    sts = ObjParaAV(NodDataObjP, NodeObjName);
    IfErrInParThenLog(sts, "Av");
    
    sts = ObjParaDV(NodDataObjP, NodeObjName);
    IfErrInParThenLog(sts, "Dv");

    sts = ObjParaCO(NodDataObjP);
    IfErrInParThenLog(sts, "Co");


    return (INI__SUCCESS);

} /* END ini_ObjPara */

/************************************************************************
*
* Name:	ObjParaAI(NodDataObjP)		
*
* Type:	pwr_tInt32	return status
*
* TYPE			    PARAMETER	    IOGF    DESCRIPTION
* rtdb_node_config_datatyp  *NodDataObjP    I	    Pointer to node config obj
* 
*
* Description:	Initialize parameters, ActualValue and
*		ValueIndex in AI-object (rtdb_aitype-object). 
*
*		Count number of AI-object and store it in RTDB 
*		(rtdb_node_config-object).
*
*		Get max number of channels/card (config_ai_object) and
*		store it in RTDB (config_ai_object).
*
*************************************************************************/

static	pwr_tInt32	ObjParaAI(NodDataObjP)

pwr_sNode	*NodDataObjP;

{
    pwr_tObjid			BaseObjId;
    pwr_tObjid			ObjId;
    pwr_tObjid			NewObjId;
    pwr_sClass_ai_value_base	*BaseObjP;
    pwr_sClass_Ai		*ObjP;
    pwr_tInt32			sts;
    pwr_tFloat32		*BaseParaP;
    pwr_tInt32 		        NumberOfAI = 0;	

    NodDataObjP->NumberOfAI = 0;

    sts = gdh_GetClassList(pwr_cClass_ai_value_base, &BaseObjId);
    if (EVEN(sts)) 
    {

	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("AI_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("AI_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

        BaseObjId = NewObjId;
    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	
    }

    BaseParaP = &BaseObjP -> AIValue[0]; 




/** Loop Ai-object *******************************************************/

    sts = gdh_GetClassList(pwr_cClass_Ai,&ObjId);
    if (EVEN(sts))
	if (sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) 
		errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("Ai"),
	                     errh_ErrArgMsg(sts), NULL);

    while (ODD(sts))
    {
    	sts = gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&ObjP);	/** Test if remote **/ 
        if(EVEN(sts))
		if(sts != GDH__REMOTE) return (sts);
       				
	if(sts != GDH__REMOTE) 
        {
		gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> ActualValue),BaseParaP);
		ObjP -> ValueIndex = NumberOfAI;
		NumberOfAI++;
		BaseParaP++;
	}

	sts = gdh_GetNextObject(ObjId,&NewObjId);
        if(EVEN(sts))
    		if(sts != GDH__NO_TYPE ) return (sts);

	if(sts != GDH__NO_TYPE) 
	{
		ObjId = NewObjId;
	}
    }		

    if(sts == GDH__NO_TYPE ||
        sts == GDH__NOSUCHOBJ ||
        sts == GDH__BADOBJTYPE) 
      sts = INI__SUCCESS;


/** Store number of Ai in node_config_data_object  ***********************/

    NodDataObjP->NumberOfAI = NumberOfAI;


    return (sts);
} /* END ObjParaAI */


/************************************************************************
*
* Name:	ObjParaAO(NodDataObjP)		
*
* Type:	pwr_tInt32	return status
*
* TYPE			    PARAMETER	    IOGF    DESCRIPTION
* rtdb_node_config_datatyp  *NodDataObjP    I	    Pointer to node config obj
* 
*
* Description:	Initialize parameters, ActualValue and
*		ValueIndex in AO-object (rtdb_aotype-object). 
*
*		Count number of AO-object and store it in RTDB 
*		(rtdb_node_config-object).
*
*		Get max number of channels/card (config_ao_object) and
*		store it in RTDB (config_ao_object).
*
**************************************************************************/

static	pwr_tInt32	ObjParaAO(NodDataObjP)

pwr_sNode	*NodDataObjP;

{
    pwr_tObjid			BaseObjId;
    pwr_tObjid			ObjId;
    pwr_tObjid			NewObjId;
    pwr_sClass_ao_value_base	*BaseObjP;
    pwr_sClass_Ao		*ObjP;
    pwr_tInt32			sts;
    pwr_tFloat32		*BaseParaP;
    pwr_tInt32			NumberOfAO = 0;	

    NodDataObjP->NumberOfAO = NumberOfAO;


    sts = gdh_GetClassList(pwr_cClass_ao_value_base,&BaseObjId);
    if (EVEN(sts)) 
    {
	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("AO_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("AO_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

        BaseObjId = NewObjId;
    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	
    }

    BaseParaP = &BaseObjP -> AOValue[0]; 



/** Loop A0-object *******************************************************/

    sts = gdh_GetClassList(pwr_cClass_Ao,&ObjId);
    if (EVEN(sts))
	if (sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) 
		errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("Ao"),
	                     errh_ErrArgMsg(sts), NULL);

    while (ODD(sts))
    {
    	sts = gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&ObjP);	/** Test if remote **/ 
        if(EVEN(sts))
		if(sts != GDH__REMOTE) return (sts);
       				
	if(sts != GDH__REMOTE) 
        {
		gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> ActualValue),BaseParaP);
		ObjP -> ValueIndex = NumberOfAO;
		NumberOfAO++;
		BaseParaP++;
	}

	sts = gdh_GetNextObject(ObjId,&NewObjId);
        if(EVEN(sts))
    		if(sts != GDH__NO_TYPE ) return (sts);

	if(sts != GDH__NO_TYPE) 
	{
		ObjId = NewObjId;
	}
    }		

    if(sts == GDH__NO_TYPE ||
        sts == GDH__NOSUCHOBJ ||
        sts == GDH__BADOBJTYPE) 
      sts = INI__SUCCESS;


/** Store number of AO in node_config_data_object  ***********************/

    NodDataObjP -> NumberOfAO = NumberOfAO;


    return (sts);
} /* END ObjParaAO */


/************************************************************************
*
* Name:	ObjParaDI(NodDataObjP)		
*
* Type:	pwr_tInt32	return status
*
* TYPE			    PARAMETER	    IOGF    DESCRIPTION
* rtdb_node_config_datatyp  *NodDataObjP    I	    Pointer to node config obj
* 
*
* Description:	Initialize parameters, ActualValue and
*		ValueIndex in DI-object (rtdb_ditype-object). 
*
*		Count number of DI-object and store it in RTDB 
*		(rtdb_node_config-object).
*
*		Get max number of channels/card (config_di_object) and
*		store it in RTDB (config_di_object).
*************************************************************************/

static	pwr_tInt32	ObjParaDI(NodDataObjP)

pwr_sNode	*NodDataObjP;

{
    pwr_tObjid			BaseObjId;
    pwr_tObjid			ObjId;
    pwr_tObjid			NewObjId;
    pwr_sClass_di_value_base	*BaseObjP;
    pwr_sClass_Di		*ObjP;
    pwr_tInt32			sts;
    pwr_tBoolean		*BaseParaP;
    pwr_tInt32 		        NumberOfDI = 0;	


    NodDataObjP -> NumberOfDI = NumberOfDI;

    sts = gdh_GetClassList(pwr_cClass_di_value_base,&BaseObjId);
    if (EVEN(sts)) 
    {
	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("DI_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("DI_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

        BaseObjId = NewObjId;
    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	
    }

    BaseParaP = &BaseObjP -> DIValue[0]; 


/** Loop Di-object *******************************************************/

    sts = gdh_GetClassList(pwr_cClass_Di,&ObjId);
    if (EVEN(sts))
	if (sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) 
		errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("Di"),
	                     errh_ErrArgMsg(sts), NULL);

    while (ODD(sts))
    {
    	sts = gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&ObjP);	/** Test if remote **/ 
        if(EVEN(sts))
		if(sts != GDH__REMOTE) return (sts);
       				
	if(sts != GDH__REMOTE) 
        {
		gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> ActualValue),BaseParaP);
		ObjP -> ValueIndex = NumberOfDI;
		NumberOfDI++;
		BaseParaP++;
	}

	sts = gdh_GetNextObject(ObjId,&NewObjId);
        if(EVEN(sts))
    		if(sts != GDH__NO_TYPE ) return (sts);

	if(sts != GDH__NO_TYPE) 
	{
		ObjId = NewObjId;
	}
    }		

    if(sts == GDH__NO_TYPE ||
        sts == GDH__NOSUCHOBJ ||
        sts == GDH__BADOBJTYPE) 
      sts = INI__SUCCESS;


/** Store number of Ai in node_config_data_object  ***********************/

    NodDataObjP->NumberOfDI = NumberOfDI;



    return (sts);
} /* END ObjParaDI */


/************************************************************************
*
* Name:	ObjParaDO(NodDataObjP)		
*
* Type:	pwr_tInt32	return status
*
* TYPE			    PARAMETER	    IOGF    DESCRIPTION
* rtdb_node_config_datatyp  *NodDataObjP    I	    Pointer to node config obj
* 
*
* Description:	Initialize parameters, ActualValue and
*		ValueIndex in DO-object (rtdb_dotype-object). 
*
*		Count number of DO-object and store it in RTDB 
*		(rtdb_node_config-object).
*
*		Get max number of channels/card (config_do_object) and
*		store it in RTDB (config_do_object).
*************************************************************************/

static	pwr_tInt32	ObjParaDO(NodDataObjP)

pwr_sNode	*NodDataObjP;

{
    pwr_tObjid			BaseObjId;
    pwr_tObjid			ObjId;
    pwr_tObjid			NewObjId;
    pwr_sClass_do_value_base	*BaseObjP;
    pwr_sClass_Do		*ObjP;
    pwr_tInt32			sts;
    pwr_tBoolean		*BaseParaP;
    pwr_tInt32 		        NumberOfDO = 0;	

    NodDataObjP->NumberOfDO = NumberOfDO;

    sts = gdh_GetClassList(pwr_cClass_do_value_base,&BaseObjId);
    if (EVEN(sts)) 
    {
	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("DO_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("DO_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

        BaseObjId = NewObjId;
    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	
    }

    BaseParaP = &BaseObjP -> DOValue[0];

/** Loop DO-object *******************************************************/

    sts = gdh_GetClassList(pwr_cClass_Do,&ObjId);
    if (EVEN(sts))
	if (sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) 
		errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("Do"),
	                     errh_ErrArgMsg(sts), NULL);

    while (ODD(sts))
    {
    	sts = gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&ObjP);	/** Test if remote **/ 
        if(EVEN(sts))
		if(sts != GDH__REMOTE) return (sts);
       				
	if(sts != GDH__REMOTE) 
        {
		gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> ActualValue),BaseParaP);
		ObjP -> ValueIndex = NumberOfDO;
		NumberOfDO++;
		BaseParaP++;
	}

	sts = gdh_GetNextObject(ObjId,&NewObjId);
        if(EVEN(sts))
    		if(sts != GDH__NO_TYPE ) return (sts);

	if(sts != GDH__NO_TYPE) 
	{
		ObjId = NewObjId;
	}
    }		

    if(sts == GDH__NO_TYPE ||
        sts == GDH__NOSUCHOBJ ||
        sts == GDH__BADOBJTYPE) 
      sts = INI__SUCCESS;


/** Store number of Do in node_config_data_object  ***********************/

    NodDataObjP -> NumberOfDO = NumberOfDO;

    return (sts);
} /* END ObjParaDO */

/************************************************************************
*
* Name:	ObjParaCO(NodDataObjP)		
*
* Type:	pwr_tInt32	return status
*
* TYPE			    PARAMETER	    IOGF    DESCRIPTION
* rtdb_node_config_datatyp  *NodDataObjP    I	    Pointer to node config obj
* 
*
* Description:	Initialize parameters, *RawValue, *AbsValue and
*		ValueIndex in CO-object (rtdb_cotype-object). 
*
*		Count number of CO-object and store it in RTDB 
*		(rtdb_node_config-object).
*
*		Get max number of channels/card (config_co_object) and
*		store it in RTDB (config_co_object).
*************************************************************************/

static	pwr_tInt32	ObjParaCO(NodDataObjP)

pwr_sNode	*NodDataObjP;

{
    pwr_tObjid			BaseObjId;
    pwr_tObjid			ObjId;
    pwr_tObjid			NewObjId;
    pwr_sClass_co_value_base	*BaseObjP;
    pwr_sClass_ca_value_base    *AbsBaseObjP;
    pwr_sClass_Co		*ObjP;
    pwr_tInt32			sts;
    pwr_tInt32			*BaseParaP;
    pwr_tInt32			*AbsBaseParaP;
    pwr_tInt32 		        NumberOfCO = 0;	

    NodDataObjP->NumberOfCO = NumberOfCO;


    sts = gdh_GetClassList(pwr_cClass_co_value_base,&BaseObjId);
    if (EVEN(sts)) 
    {
	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("CO_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("CO_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

        BaseObjId = NewObjId;
    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	
    }

    BaseParaP = &BaseObjP -> COValue[0]; 


/** Get pointer to CA_VALUE_BASE object on local node ************/

    sts = gdh_GetClassList(pwr_cClass_ca_value_base,&BaseObjId);
    if (EVEN(sts)) 
    {
	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("CA_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&AbsBaseObjP);	

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("CA_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

        BaseObjId = NewObjId;
    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&AbsBaseObjP);	
    }

    AbsBaseParaP = &AbsBaseObjP -> CAValue[0]; 


/** Loop Co-object *******************************************************/

    sts = gdh_GetClassList(pwr_cClass_Co,&ObjId);
    if (EVEN(sts))
	if (sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) 
		errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("Co"),
	                     errh_ErrArgMsg(sts), NULL);

    while (ODD(sts))
    {
    	sts = gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&ObjP);	/** Test if remote **/ 
        if(EVEN(sts))
		if(sts != GDH__REMOTE) return (sts);
       				
	if(sts != GDH__REMOTE) 
        {
		gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> RawValue),BaseParaP);
                gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> AbsValue),AbsBaseParaP);
		ObjP -> ValueIndex = NumberOfCO;
		NumberOfCO++;
		AbsBaseParaP++;
		BaseParaP++;
	}

	sts = gdh_GetNextObject(ObjId,&NewObjId);
        if(EVEN(sts))
    		if(sts != GDH__NO_TYPE ) return (sts);

	if(sts != GDH__NO_TYPE) 
	{
		ObjId = NewObjId;
	}
    }		

    if(sts == GDH__NO_TYPE ||
        sts == GDH__NOSUCHOBJ ||
        sts == GDH__BADOBJTYPE) 
      sts = INI__SUCCESS;


/** Store number of Co in node_config_data_object  ***********************/

    NodDataObjP -> NumberOfCO = NumberOfCO;





    return (sts);
} /* END ObjParaCO */


/************************************************************************
*
* Name:	ObjParaAV(NodDataObjP, NodeObjName)		
*
* Type:	pwr_tInt32	return status
*
* TYPE			    PARAMETER	    IOGF    DESCRIPTION
* rtdb_node_config_datatyp  *NodDataObjP    I	    Pointer to node config obj
* pwr_tFullName		    NodeObjName	    I	    Node object name
* 
*
* Description:	Initialize parameters, ActualValue and
*		ValueIndex in AV-object (rtdb_AVtype-object). 
*
*		Initialize ActualValue, ActualValue = InitialValue.
*
*		Count number of AV-object and store it in RTDB 
*		(rtdb_node_config-object).
*************************************************************************/

static	pwr_tInt32	ObjParaAV(NodDataObjP, NodeObjName)

pwr_sNode	*NodDataObjP;
pwr_tFullName   NodeObjName;
{
    pwr_tObjid		    BaseObjId;
    pwr_tObjid		    ObjId;
    pwr_tObjid		    NewObjId;
    pwr_sClass_av_value_base
			    *BaseObjP;
    pwr_sClass_Av	    *ObjP;
    pwr_tInt32		    sts;
    pwr_tFloat32	    *BaseParaP;
    pwr_tInt32		    NumberOfAV = 0;	

    pwr_tFullName	     ObjName;
    pwr_tUInt32		     Size = 0;
    pwr_sClass_ca_value_base *AddrObjP;
   
    NodDataObjP -> NumberOfAV = NumberOfAV;

    sts = gdh_GetClassList(pwr_cClass_av_value_base,&BaseObjId);
    if (EVEN(sts)) 
    {
	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("AV_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("AV_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

	BaseObjId = NewObjId;

    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	
    }

    BaseParaP = &BaseObjP -> AVValue[0]; 

/** If not exists create object "-AvInitValueAddr" of class pwr_cClass_ca_value_base */
    sts = gdh_GetObjectSize ( BaseObjId, &Size);
    if (EVEN (sts))
      return(sts);

    strcpy (ObjName, "pwrNode-AvInitValueAddr");
    sts = gdh_CreateObject (ObjName,
                            pwr_cClass_ca_value_base,
                            Size,
                            &ObjId, pwr_cNObjid, 0, pwr_cNObjid);
    if (EVEN (sts))
      if ( sts != GDH__DUPLNAME ) return(sts);
    sts = gdh_NameToPointer ( ObjName, (pwr_tAddress*)&AddrObjP);
    if (EVEN (sts))
      return(sts);
    
    memset ( AddrObjP, 0, Size); 

/** Loop Av-object *******************************************************/

    sts = gdh_GetClassList(pwr_cClass_Av,&ObjId);
    if (EVEN(sts))
	if (sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) 
		errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("Av"),
	                     errh_ErrArgMsg(sts), NULL);

    while (ODD(sts))
    {
    	sts = gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&ObjP);	/** Test if remote **/ 
        if(EVEN(sts))
		if(sts != GDH__REMOTE) return (sts);
       				
	if(sts != GDH__REMOTE) 
        {
		gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> ActualValue),BaseParaP);
		ObjP -> ValueIndex = NumberOfAV;
                *BaseParaP = ObjP->InitialValue;
                gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(AddrObjP->CAValue[NumberOfAV]),
				      &(ObjP->InitialValue));
		NumberOfAV++;
		BaseParaP++;
	}

	sts = gdh_GetNextObject(ObjId,&NewObjId);
        if(EVEN(sts))
    		if(sts != GDH__NO_TYPE ) return (sts);

	if(sts != GDH__NO_TYPE) 
	{
		ObjId = NewObjId;
	}
    }		

    if(sts == GDH__NO_TYPE ||
        sts == GDH__NOSUCHOBJ ||
        sts == GDH__BADOBJTYPE) 
      sts = INI__SUCCESS;


/** Store number of Av in node_config_data_object  ***********************/

    NodDataObjP -> NumberOfAV = NumberOfAV;


    return (sts);

} /* END ObjParaAV */


/************************************************************************
*
* Name:	ObjParaDV(NodDataObjP, NodeObjName)		
*
* Type:	pwr_tInt32	return status
*
* TYPE			    PARAMETER	    IOGF    DESCRIPTION
* rtdb_node_config_datatyp  *NodDataObjP    I	    Pointer to node config obj
* pwr_tFullName             NodeObjName     I       Node object name
* 
*
* Description:	Initialize parameters, ActualValue and
*		ValueIndex in DV-object (rtdb_DVtype-object). 
*
*               Initialize ActualValue, ActualValue = InitialValue.
*
*		Count number of DV-object and store it in RTDB 
*		(rtdb_node_config-object).
*************************************************************************/

static	pwr_tInt32	ObjParaDV(NodDataObjP, NodeObjName)

pwr_sNode	*NodDataObjP;
pwr_tFullName   NodeObjName;

{
    pwr_tObjid		    BaseObjId;
    pwr_tObjid		    ObjId;
    pwr_tObjid		    NewObjId;
    pwr_sClass_Dv	    *ObjP;
    pwr_sClass_dv_value_base *BaseObjP;
    pwr_tInt32		    sts;
    pwr_tBoolean	    *BaseParaP;
    pwr_tInt32		    NumberOfDV=0;

    pwr_tFullName	     ObjName;
    pwr_tUInt32		     Size = 0;
    pwr_sClass_ca_value_base *AddrObjP;

/** Get address to DVValue in DV_VALUE_BASE_OBJECT on local node ************/

    sts = gdh_GetClassList(pwr_cClass_dv_value_base,&BaseObjId);
    if (EVEN(sts)) 
    {
	errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("DV_VALUE_BASE"),
	             errh_ErrArgMsg(sts), NULL);
	return (INI__ERRINIT);
    }

    sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	/** Test if remote **/ 

    while(EVEN(sts))					/** Test if remote **/ 
    {
    	if (sts != GDH__REMOTE) return(sts);

    	sts = gdh_GetNextObject(BaseObjId,&NewObjId);
        if(EVEN(sts))
        {
    		if(sts == GDH__NO_TYPE );
		{
			errh_CErrLog(INI__GETLOCALOBJ, 
				     errh_ErrArgAF("DV_VALUE_BASE"),
			             errh_ErrArgMsg(sts), NULL);
			return (INI__ERRINIT);
		}
                return(sts);
        }

        BaseObjId = NewObjId;
    	sts = gdh_ObjidToPointer(BaseObjId, (pwr_tAddress *)&BaseObjP);	
    }

    BaseParaP = &BaseObjP -> DVValue[0]; 
 
/** If not exists create object "-DvInitValueAddr" of class pwr_cClass_ca_value_base */
    sts = gdh_GetObjectSize ( BaseObjId, &Size);
    if (EVEN (sts))
      return(sts);

    strcpy (ObjName, "pwrNode-DvInitValueAddr");
    sts = gdh_CreateObject (ObjName,
                            pwr_cClass_ca_value_base,
                            Size * 4,
                            &ObjId, pwr_cNObjid, 0, pwr_cNObjid);
    if (EVEN (sts))
      if ( sts != GDH__DUPLNAME ) return(sts);

    sts = gdh_NameToPointer ( ObjName, (pwr_tAddress *)&AddrObjP);
    if (EVEN (sts))
      return(sts);

    memset ( AddrObjP, 0, Size * 4); 

/** Loop DV-object *******************************************************/

    sts = gdh_GetClassList(pwr_cClass_Dv,&ObjId);
    if (EVEN(sts))
	if (sts != GDH__NOSUCHOBJ && sts != GDH__BADOBJTYPE && sts != GDH__NO_TYPE) 
		errh_CErrLog(INI__GETTYPELIST, errh_ErrArgAF("Dv"),
	                     errh_ErrArgMsg(sts), NULL);

    while (ODD(sts))
    {
    	sts = gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&ObjP);	/** Test if remote **/ 
        if(EVEN(sts))
		if(sts != GDH__REMOTE) return (sts);
       				
	if(sts != GDH__REMOTE) 
        {
		gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(ObjP -> ActualValue),BaseParaP);
		ObjP -> ValueIndex = NumberOfDV;
                *BaseParaP = ObjP->InitialValue;

                gdh_StoreRtdbPointer ((pwr_tUInt32 *)&(AddrObjP->CAValue[NumberOfDV]),
				      &(ObjP->InitialValue));
		NumberOfDV++;
		BaseParaP++;	/* This change 910727 GPd */
	}

	sts = gdh_GetNextObject(ObjId,&NewObjId);
        if(EVEN(sts))
    		if(sts != GDH__NO_TYPE ) return (sts);

	if(sts != GDH__NO_TYPE) 
	{
		ObjId = NewObjId;
	}
    }		

    if(sts == GDH__NO_TYPE ||
        sts == GDH__NOSUCHOBJ ||
        sts == GDH__BADOBJTYPE) 
      sts = INI__SUCCESS;


/** Store number of Dv in node_config_data_object  ***********************/

    NodDataObjP -> NumberOfDV = NumberOfDV;

    return (sts);

} /* END ObjParaDV */

