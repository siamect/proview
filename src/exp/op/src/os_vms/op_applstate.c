/* 
 * Proview   $Id: op_applstate.c,v 1.2 2005-09-01 14:57:49 claes Exp $
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

#include "pwr.h"
#include "op_appltable.h"
#include "rt_gdh.h"
#include "op_msg.h"
#include "pwr_baseclasses.h"
#include "decw$include:xm.h"
#include "gms$home:<lib>gms_cplusplus.h"
#include ""
#include "op_cgms.h"

#define _CLASSNAME "APL1"

typedef struct
{
    G_STATE_FIELDS
    pwr_sClass_Graph Graph;

    SUBID Subid;
    pwr_tFloat32 *ActValP,ActVal;

    pwr_tPathName GDHObjName;
    pwr_tPathName GMSObjName;

    pwr_tBoolean MoreActive;
    id MoreStateId;
    pwr_tUInt32 (*QuitFunc)();
} _STATEINST;

pwr_tUInt32 apl1_Initialize();
pwr_tUInt32 apl1_Another();
static pwr_tUInt32 TakeDownSubscriptions(_STATEINST *StateP);
static pwr_tUInt32 SetupSubscriptions(_STATEINST *StateP,id ModInst,id Model);

static pwr_tUInt32 MoreSave(),MoreQuit();

static pwr_tInt32 Update(_STATEINST *StateP);
static pwr_tInt32 Deactivate(_STATEINST *StateP);

static APPLTABLESTRUCT Appl = {_CLASSNAME,apl1_Initialize,apl1_Another,NULL};

/************************************************************************
*
* Name:	apl1_GetCallStruct(CallStructP)		
*
* Type:	pwr_tUInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* APPLTABLESTRUCT
*		*CallStructP	O	
*
* Description:	Fills the supplied APPLTABLESTRUCT with functions that
*		initializes and activates the class.	
*************************************************************************/

pwr_tUInt32 apl1_GetCallStruct(CallStructP)
APPLTABLESTRUCT *CallStructP;
{
    *CallStructP = Appl;
    return (PRO__SUCCESS);
} /* END apl1_GetCallStruct;

/************************************************************************
*
* Name:	CreateModelAndSetupSub(StateP)		
*
* Type:	static pwr_tUInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* _STATEINST	*StateP		IO
*
* Description:	This function is responsible for createing the model and 
*		connects the view with the database.
*************************************************************************/

static pwr_tUInt32 CreateModelAndSetupSub(StateP)
_STATEINST *StateP;
{
    pwr_tString40 ModelName,FileName;
    static pwr_tUInt32 S_ModelNo=0;
    pwr_tUInt32 sts;
    idPoint point;
    id ModInst,Model;

    gmsNoUpdImmu();
    sprintf(ModelName,"%sMODEL%d",_CLASSNAME,S_ModelNo);
    S_ModelNo++;

    Model = gmsModel(ModelName);

    point = pntNew();
    gmsSWPXY(point,0.0,0.0);

    strcpy(FileName,StateP->Graph.GMS_Model);
    ModInst = gmsModInst("",FileName,point);

    sts = SetupSubscriptions(StateP,ModInst,Model);
    gmsStAddModelName(StateP,ModelName);

    gmsRedraw(gmsQStView(StateP));
 
    return(PRO__SUCCESS);
} /* END CreateModelAndSetupSub */

/************************************************************************
*
* Name: Update(StateP)
*
* Type: static pwr_tInt32
*
* TYPE          PARAMETER       IOGF    DESCRIPTION
* _STATEINST    *StateP         IO
*
* Description:  This function is responsible for updating the view.
*************************************************************************/

static pwr_tInt32 Update(StateP)
_STATEINST *StateP;
{
    return(1);
} /* END Update */

/************************************************************************
*
* Name:	MoreQuit(StateP)
*
* Type:	static pwr_tUInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* _STATEINST	*StateP		IO
*
* Description:	This function is supplied to substates and is called when 
*		the substate is made "Quit".
*************************************************************************/

static pwr_tUInt32 MoreQuit(StateP)
_STATEINST *StateP;
{
    StateP->MoreActive = FALSE;

    return(PRO__SUCCESS);
}/*END MoreQuit*/

/************************************************************************
*
* Name:	Deactivate(StateP)
*
* Type:	static pwr_tInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* _STATEINST	*StateP		IO
*
* Description:	This function is responsible for deactivation.
*************************************************************************/

static pwr_tInt32 Deactivate(StateP)
_STATEINST *StateP;
{
    pwr_tUInt32 sts;

    sts = TakeDownSubscriptions(StateP);
    gmsObjFree(gmsQStModel(StateP));
    if (EVEN(sts))
        errh_CErrlog(sts, NULL);

    return(1);
} /* END Deactivate */

/************************************************************************
*
* Name:	More()
*
* Type:	static pwr_tInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description:	This function is called when the user presses More. It should 
*		produce another view that shows som more information about 
*		the object.
*************************************************************************/

static pwr_tInt32 More()
{
    _STATEINST *StateP;
    pwr_tString40 WSName,ViewName;
    pwr_tUInt32 sts;
    pwr_tString80 Title;

    StateP = gmsQStEvState();
    if (!StateP->MoreActive)
    {
	StateP->MoreActive = TRUE;
/*
	sprintf(Title,"Mer : %s",StateP->GDHObjName);
	sts = proutil_CreateWS(Title,WSName,ViewName,0,0,400,400);
	me1_Another(StateP->GDHObjName,WSName,ViewName,&StateP->MoreStateId,
		    StateP,MoreSave,MoreQuit);
*/
    }
}/* END More */

/************************************************************************
*
* Name:	CloseState()
*
* Type:	static pwr_tInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description:	This function is called when the user presses Quit.
*************************************************************************/

static pwr_tInt32 CloseState()
{
    _STATEINST *StateP;
    pwr_tUInt32 sts;
    void *SuperState;

    StateP = gmsQStEvState();

    (*StateP->QuitFunc)(StateP);
    SuperState = gmsQStSuperState(StateP);
    gmsStDeactivate(StateP);
    gmsRedraw(gmsQStView(SuperState));
} /* END CloseState */

/************************************************************************
*
* Name:	TakeDownSubscriptions(StateP)		
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* _STATEINST	*StateP		I	
*
* Description:	This function takes down all subscriptions for a specified 
*		state.
*************************************************************************/

static pwr_tUInt32 TakeDownSubscriptions(StateP)
_STATEINST *StateP;
{
    pwr_tUInt32 sts;

    sts = PRO__SUCCESS;
    return(sts);
} /* END TakeDownSubscriptions */

/************************************************************************
*
* Name:	SetupSubscriptions(StateP)		
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* _STATEINST	*StateP		I	
*
* Description:	This function sets up all subscriptions for a specified 
*		state.
*************************************************************************/

static pwr_tUInt32 SetupSubscriptions(StateP,ModInst,Model)
_STATEINST *StateP;
id ModInst,Model;
{
    pwr_tUInt32 sts;
    pwr_tFullName ParName1,ParName2;
    char Str[pwr_cSizFullName + 4 + pwr_cSizAttrName + 1];
    char TotRename[3*(pwr_cSizFullName + 4 + pwr_cSizAttrName + 1 + 1)];

/** Setup subscribtions or fetch data from RDB to local variables in your 
    state *****************************************************************/

/* 
    sprintf(ParName1,"%s.%s","sh-ai1","ActualValue");
    sts = gdh_RefObjectInfo(ParName0,&StateP->ActValP,
			    &StateP->Subid, 0);
*/

/** Initialize the states data ********************************************/

    StateP->ActVal = 0.0;
    StateP->MoreActive = FALSE;

/** Rename all variables in the picture to make it reusable ***************/

    strcpy(TotRename,"");
/*
    sprintf(ParName2,"%s_%s",StateP->GMSObjName,"ActualValue");
    sprintf(Str,"%s :: %s ","ActualValue",ParName2);
    strcat(TotRename,Str);
*/
    gmsRenamedStr(ModInst,TotRename);   
    gmsDynInit(Model);
    gmsDynUpdate(Model);
    gmsUpdate(gmsQStView(StateP));

/** Connect the variables with GMS variables ******************************/

    gmsVarDefine(ParName1,&StateP->ActVal,G_CHAR,1,1);
    gmsVarChanged("",&StateP->ActVal);

    gmsDynUpdate(Model);
    gmsEndM();
    return(sts);
} /* END SetupSubscriptions */

/************************************************************************
*
* Name:	Another(SelectedObjName,WSName,ViewName,StateP,ParentState)
*
* Type:	static pwr_tUInt32	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* char *	SelectedObjName	I	The name of the obj to be presented.
* char *	WSName		I	Name of workstation (GMS) 
* char *	ViewName	I	Name of view (GMS) 
* _STATEINST **	StateP		IO
* id		ParentState	IO	State
*
* Description:	This function should produce an instance of the files class.
*************************************************************************/

pwr_tUInt32 apl1_Another(SelectedObjName,WSName,ViewName,StateP,ParentState,QuitFunc)
char *SelectedObjName,*WSName,*ViewName;
_STATEINST **StateP;
id ParentState;
pwr_tUInt32 (*QuitFunc)();
{
    static pwr_tUInt32 S_StateNo = 0;
    pwr_tString16 StateName;
    pwr_tUInt32 sts,I;
    pwr_tString40 ClassName;

    sprintf(ClassName,"%sCLASS",_CLASSNAME);
    sprintf(StateName,"%sINST%d",_CLASSNAME,S_StateNo);

    *StateP = (_STATEINST *)gmsStateInst(StateName,ClassName);
    gmsStWorkstName(*StateP, WSName);
    gmsStViewName(*StateP, ViewName);

    gmsStClearFlag(*StateP,1);
    gmsStFreeFlag(*StateP,1);
    gmsStVisFlag(*StateP, 1);
    gmsStAutoDeactFlag(*StateP, G_OFF);

    gmsDynUpdate(nil);
    gmsUpdate(nil);

    strcpy((*StateP)->GDHObjName,SelectedObjName);
    sprintf((*StateP)->GMSObjName,"%sOBJ%d",_CLASSNAME,S_StateNo);
    S_StateNo++;
    (*StateP)->QuitFunc = QuitFunc;

    gmsStActivate(*StateP,ParentState);

    sts = gdh_GetObjectInfo(SelectedObjName,&(*StateP)->Graph,
			    sizeof(pwr_sClass_Graph));	    
    if (EVEN(sts))
    {
        return (sts);
    }  

    sts = CreateModelAndSetupSub(*StateP);
    if (EVEN(sts))
    {
        return (sts);
    }  
    return(PRO__SUCCESS);
} /* END apl1_Another */

/************************************************************************
*
* Name:	apl1_Initialize()		
*
* Type:	pwr_tUInt32
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description:	Sets up the functions associated with the class.
*************************************************************************/

pwr_tUInt32 apl1_Initialize()
{
    static id stclass = 0;
    static pwr_tInt32 Arg1I[] = { G_INTEGER };
    pwr_tString40 ClassName;

    sprintf(ClassName,"%sCLASS",_CLASSNAME);
    if (stclass)
	return (PRO__CLAALRDE);

    stclass = gmsStateClass(ClassName, sizeof(_STATEINST));
    gmsClassAddMethod(stclass, "update",Update, 0);
    gmsClassAddMethod(stclass, "deactivate",Deactivate, 0);
    gmsClassAddMethod(stclass, "loc_release",ps_op_com_loc_release, 0);
    gmsStClassInstall(stclass);

    gmsAddUserFctn("apl1_CloseState",CloseState,G_INTEGER, 1, Arg1I);
    gmsAddUserFctn("apl1_More",More,G_INTEGER, 1, Arg1I);

    me1_Initialize();

    return(PRO__SUCCESS);
} /* END apl1_Initialize */
