
#include "flow_std.h"


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

extern "C" {
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "co_ccm_msg.h"
#include "co_api.h"
#include "flow_x.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/CascadeBG.h>
#include <Xm/MessageB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/SelectioB.h>
#include <Xm/SeparatoG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

#include "co_lng.h"
#include "co_dcli_msg.h"
#include "rt_xnav_msg.h"
#include "xtt_xnav.h"
#include "xtt_menu.h"

pwr_dImport pwr_BindXttMethods($Object);
pwr_dImport pwr_BindXttMethods(PlcPgm);

pwr_BindXttClasses(Base) = {
  pwr_BindXttClass($Object),
  pwr_BindXttClass(PlcPgm),
  pwr_NullClass
};

#define MENU_BAR      1
#define MENU_PULLDOWN 2
#define MENU_POPUP    3
#define MENU_OPTION   4

typedef struct MENU_ITEM {
    char		*Label;
    WidgetClass		*Class;
    char		*Mnemonic;
    char		*Accelerator;
    char		*AccelText;
    int			UserData;
    void		(*Callback)();
    XtPointer		CallbackData;
    struct MENU_ITEM	*SubItems;
    Boolean		Sensitive;
} MenuItem;

// MenuCall struct. Used by the popup menu
static xmenu_sMenuCall *mcp = NULL;
static xmenu_sMenuItem xmenu_lMenuItem[100];

static void xnav_popup_button_cb (Widget w, XNav *xnav);
static void xnav_popup_unmap_cb(Widget w, xmenu_sMenuCall *ip);
static int xnav_GetMenu( xmenu_sMenuCall *ip);
static int getAllMenuItems (
  xmenu_sMenuCall	*ip,
  xmenu_sMenuItem	**Item,
  pwr_tObjid		objid,
  pwr_tUInt32		Level,
  int			*nItems,
  int			AddSeparator);
static int xnav_GetMethod( char *name, 
			   pwr_tStatus (**method)( xmenu_sMenuCall *));

static Widget xnav_build_menu(
	Widget Parent,
	int   MenuType,
	char *MenuTitle,
	void *MenuUserData,
	void (*Callback)( Widget, XNav *),
	void *CallbackData,
	xmenu_sMenuItem *Items,
	int *idx
	)
{
  Widget Menu, Cascade, W;
  int i;
  unsigned int Level;
  XmString Str;
  WidgetClass Class;
  Arg ArgList[5]; 
  XmFontList fontlist;
  XFontStruct *font;
  XmFontListEntry fontentry;

  // Set default fontlist
  font = XLoadQueryFont( flow_Display(Parent),
  	      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
  fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
  fontlist = XmFontListAppendEntry( NULL, fontentry);
  XtFree( (char *)fontentry);

  i = 0;
  XtSetArg(ArgList[i], XmNuserData, MenuUserData); i++;
  XtSetArg(ArgList[i], XmNbuttonFontList, fontlist); i++;
  XtSetArg(ArgList[i], XmNlabelFontList, fontlist); i++;
  if (MenuType == MENU_PULLDOWN || MenuType == MENU_OPTION)
    Menu = XmCreatePulldownMenu(Parent, "_pulldown", ArgList, i);
  else if (MenuType == MENU_POPUP)
    Menu = XmCreatePopupMenu(Parent, "_popup", ArgList, i);  
  else 
  {
    XtWarning("Invalid menu type passed to BuildMenu().");
    return NULL;
  }

  if (MenuType == MENU_PULLDOWN) 
  {
    Str = XmStringCreateSimple(MenuTitle);	
    Cascade = XtVaCreateManagedWidget(MenuTitle,
	    xmCascadeButtonGadgetClass, Parent,
	    XmNsubMenuId,   Menu,
	    XmNlabelString, Str,
	    NULL);
    XmStringFree(Str);
  } 
  else if (MenuType == MENU_OPTION) 
  {
    Str = XmStringCreateSimple(MenuTitle);
    XtSetArg(ArgList[0], XmNsubMenuId, Menu);
    XtSetArg(ArgList[1], XmNlabelString, Str);
    Cascade = XmCreateOptionMenu(Parent, MenuTitle, ArgList, 2);
    XmStringFree(Str);
  }

  XmFontListFree( fontlist);

  Level = Items[*idx].Level;

  for (; Items[*idx].Level != 0 && Items[*idx].Level >= Level; (*idx)++) 
  {
    if (Items[*idx].Item == xmenu_eMenuItem_Cascade) 
    {
      if (MenuType == MENU_OPTION) 
      {
        XtWarning("You can't have submenus from option menu items.");
        return NULL;
      } 
      else 
      {
        i = *idx;
        (*idx)++;	
        xnav_build_menu(Menu, MENU_PULLDOWN, 
		  Lng::translate( Items[i].Name), MenuUserData, 
		  Callback, CallbackData, Items, idx);
        (*idx)--;
      }
    }
    else 
    {
      if (Items[*idx].Item == xmenu_eMenuItem_Separator)
        Class = xmSeparatorGadgetClass;
      else
        Class = xmPushButtonGadgetClass;
 
      W = XtVaCreateManagedWidget(Lng::translate( Items[*idx].Name), 
		    Class, Menu,
		    XmNuserData, *idx,
		    XmNsensitive, (Boolean)(Items[*idx].Flags.f.Sensitive == 1),
		    NULL);

      if (Callback && Class == xmPushButtonGadgetClass)
        XtAddCallback(W, XmNactivateCallback, (XtCallbackProc) Callback, 
		  (XtPointer) CallbackData);
    }
  }

  return MenuType == MENU_POPUP ? Menu : Cascade;
}

static int getAllMenuItems (
  xmenu_sMenuCall	*ip,
  xmenu_sMenuItem	**Item,
  pwr_tObjid		objid,
  pwr_tUInt32		Level,
  int			*nItems,
  int			AddSeparator
)
{
  int                   sts;
  pwr_tClassId          classid;
  pwr_tObjid            child;
  pwr_sMenuButton	*mbp;
  pwr_sMenuCascade	*mcp;
  pwr_tStatus           (*filter)( xmenu_sMenuCall *);
  int                   sensitive;
  int                   i;


  Level++;
  memset(*Item, 0, sizeof(**Item));

  if(AddSeparator) {
    (*Item)->Level = Level;
    (*Item)->Item = xmenu_eMenuItem_Separator;
    (*Item)->MenuObject = pwr_cNObjid;
    (*Item)++;
    (*nItems)++;
  } 
  else {
    sts = gdh_GetObjectClass( objid, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_eClass_MenuButton) {
      sts = gdh_ObjidToPointer( objid, (void **) &mbp);
      if ( EVEN(sts)) return sts;

      
      ip->ChosenItem = *nItems;
      for ( i = 0; i < 5; i++) {
        strcpy( (*Item)->FilterArguments[i], mbp->FilterArguments[i]);
      }

      // Call any filter method
      sensitive = 1;
      if ( strcmp( mbp->FilterName, "") != 0) {
        sts = xnav_GetMethod( mbp->FilterName, &filter);
        if ( ODD(sts)) {
          sts = (filter) ( ip);
	  if ( sts == XNAV__INSENSITIVE) 
            sensitive = 0;
        }
      }
      else
        sts = XNAV__SUCCESS;

      if ( ODD(sts)) {
        (*Item)->Level = Level;
        (*Item)->Item = xmenu_eMenuItem_Button;
        (*Item)->MenuObject = objid;

        (*Item)->Flags.f.Sensitive = sensitive;
        if (strcmp( mbp->MethodName, "")  == 0)
          (*Item)->Flags.f.Sensitive = 0;

        strcpy((*Item)->Name, mbp->ButtonName);
        strcpy( (*Item)->Method, mbp->MethodName);
        for ( i = 0; i < 5; i++) {
          strcpy( (*Item)->MethodArguments[i], mbp->MethodArguments[i]);
        }
        (*Item)++;
        (*nItems)++;
      }
    } 
    else if ( classid == pwr_eClass_MenuSeparator) {
      (*Item)->Level = Level;
      (*Item)->Item = xmenu_eMenuItem_Separator;
      (*Item)->MenuObject = objid;
      (*Item)++;
      (*nItems)++;
    } 
    else if ( classid == pwr_eClass_MenuCascade) {
      sts = gdh_ObjidToPointer( objid, (void **) &mcp);
      if ( EVEN(sts)) return sts;

      // Call any filter method
      if ( strcmp( mcp->FilterName, "") != 0) {
        sts = xnav_GetMethod( mcp->FilterName, &filter);
        if ( ODD(sts)) {
          sts = (filter) ( ip);
        }
      }
      else
        sts = XNAV__SUCCESS;

      if ( ODD(sts)) {
        (*Item)->Level = Level;
        (*Item)->Item = xmenu_eMenuItem_Cascade;
        (*Item)->Flags.f.Sensitive = 1;
        strcpy((*Item)->Name, mcp->ButtonName);
        (*Item)->MenuObject = objid;
        (*Item)++;
        (*nItems)++;
     
        sts = gdh_GetChild( objid, &child);
        while( ODD(sts)) {
          sts = getAllMenuItems(ip, Item, child, Level, nItems, 0);
          if ( EVEN(sts)) return sts;
          sts = gdh_GetNextSibling( child, &child);
        }
      }
    }
  }
  return XNAV__SUCCESS;
}


static int xnav_GetMenu( xmenu_sMenuCall *ip)
{
  pwr_tClassId          classid;
  int                   sts;
  pwr_tObjid            child;
  xmenu_sMenuItem	*Item = (xmenu_sMenuItem *) &xmenu_lMenuItem;
  pwr_tObjid		menu_objid;
  char  		menu[80];
  char  		classname[120];
  int			nItems = 0;

  if ( cdh_ObjidIsNotNull( mcp->Selected[0].Objid)) {
    sts = gdh_GetObjectClass( mcp->Selected[0].Objid, &classid);
    if ( EVEN(sts)) return sts;

    if ( mcp->ItemType == xmenu_eItemType_Object) {
      // Popup-menu for an object

      // Get the RtXtt common menu-objects
      strcpy( menu, "pwrs:Class-$Object-RtXtt");
      sts = gdh_NameToObjid( menu, &menu_objid);
      if ( ODD(sts)) {
        sts = gdh_GetChild( menu_objid, &child);
        while( ODD(sts)) {
          sts = getAllMenuItems( ip, &Item, child, 0, &nItems, 0);
          if ( EVEN(sts)) return sts;
          sts = gdh_GetNextSibling( child, &child);
        }
      }

      // Get the RtXtt menu-objects for this class
      sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid), classname, 
		     sizeof(classname), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      sprintf( menu, "%s-RtXtt", classname);
      sts = gdh_NameToObjid( menu, &menu_objid);
      if ( ODD(sts)) {
        sts = gdh_GetChild( menu_objid, &child);
        while( ODD(sts)) {
          sts = getAllMenuItems( ip, &Item, child, 0, &nItems, 0);
          if ( EVEN(sts)) return sts;
          sts = gdh_GetNextSibling( child, &child);
        }
      }
    }
    else if ( mcp->ItemType == xmenu_eItemType_Attribute) {   
      // Find attribute methods...
      // Get the RtXttAttribute common menu-objects
      strcpy( menu, "pwrs:Class-$Object-RtXttAttribute");
      sts = gdh_NameToObjid( menu, &menu_objid);
      if ( ODD(sts)) {
        sts = gdh_GetChild( menu_objid, &child);
        while( ODD(sts)) {
          sts = getAllMenuItems( ip, &Item, child, 0, &nItems, 0);
          if ( EVEN(sts)) return sts;
          sts = gdh_GetNextSibling( child, &child);
        }
      }
    }
    else if ( mcp->ItemType == xmenu_eItemType_Crossref) {   
      // Find attribute methods...
      // Get the RtXttCrossref common menu-objects
      strcpy( menu, "pwrs:Class-$Object-RtXttCrossref");
      sts = gdh_NameToObjid( menu, &menu_objid);
      if ( ODD(sts)) {
        sts = gdh_GetChild( menu_objid, &child);
        while( ODD(sts)) {
          sts = getAllMenuItems( ip, &Item, child, 0, &nItems, 0);
          if ( EVEN(sts)) return sts;
          sts = gdh_GetNextSibling( child, &child);
        }
      }
    }
  }
  else {
    // Find no object methods...
  }

  Item->Level = 0;
  ip->ItemCount = nItems - 1;
  return XNAV__SUCCESS;
}

static int xnav_GetMethod( char *name, 
			   pwr_tStatus (**method)( xmenu_sMenuCall *))
{
  int i, j;
  char cname[80];
  char mname[80];
  char *s;

  strcpy( cname, name);
  if ( (s = strchr( cname, '-'))) {
    *s = 0;
    strcpy( mname, s+1);
  }
  else
    strcpy( mname, "");

  for ( i = 0;; i++)
  {
    if (pwr_gBase_XttClassMethods[i].ClassName[0] == '\0') break;
    {
      if ( strcmp(pwr_gBase_XttClassMethods[i].ClassName, cname) == 0)
      {
        for (j = 0;; j++) 
        {
          if ((*pwr_gBase_XttClassMethods[i].Methods)[j].MethodName[0] == '\0')
	    break;
          if ( strcmp( (*pwr_gBase_XttClassMethods[i].Methods)[j].MethodName,
		       mname) == 0) {
	    *method = (*pwr_gBase_XttClassMethods[i].Methods)[j].Method;
	    return XNAV__SUCCESS;
          }
        }
	break;
      }
    }
  }
  return XNAV__NOMETHOD;
}

static int xnav_CallMenuMethod( xmenu_sMenuCall *ip, int idx)
{
  pwr_tStatus sts;
  pwr_tStatus (*method)( xmenu_sMenuCall *);

  sts = xnav_GetMethod( ip->ItemList[idx].Method, &method);
  if ( ODD(sts))
    sts = (method) ( ip);

  return sts;
}


Widget xnav_create_popup_menu( XNav *xnav, pwr_sAttrRef attrref, 
			       xmenu_eItemType item_type, 
			       xmenu_mUtility caller, 
			       unsigned int priv, char *arg)
{
  pwr_tStatus 	sts;
  int 		i;
  Widget 	popup;
  int		sel_cnt;

  if (mcp == NULL)
    mcp = (xmenu_sMenuCall *)XtCalloc(1, sizeof(xmenu_sMenuCall));

  mcp->ItemList = xmenu_lMenuItem;
  mcp->EditorContext = (void *)xnav;
  mcp->WindowContext = (void *)xnav->parent_wid;
  mcp->PointedSet = xmenu_eMenuSet_Object;

  mcp->Pointed = attrref;
  mcp->Caller = caller;
  mcp->ItemType = item_type;
  mcp->Priv = priv;
  mcp->SelectedSet = xmenu_eMenuSet_None;
  mcp->SelectedSet = xmenu_eMenuSet_Object;
  if ( arg)
    strcpy( mcp->Arg, arg);

  sel_cnt = 1;
  mcp->Selected = (pwr_sAttrRef *) XtCalloc( sel_cnt + 1, sizeof (pwr_sAttrRef));
  mcp->Selected[0] = attrref;
  mcp->Selected[sel_cnt].Objid = pwr_cNObjid;
  mcp->SelectCount = sel_cnt;

  sts = xnav_GetMenu( mcp);  
  if (EVEN(sts) || mcp->ItemList[0].Level == 0) {
    return NULL;
  }

  i = 0;
  popup = xnav_build_menu( xnav->parent_wid, MENU_POPUP, "", mcp, 
	xnav_popup_button_cb, (void *) xnav, 
	(xmenu_sMenuItem *) mcp->ItemList, &i); 
  if (popup != NULL) 
    XtAddCallback (popup, XmNunmapCallback, 
				(XtCallbackProc)xnav_popup_unmap_cb, mcp);

  return popup;

}/* hied_fun_popup_bc */ 

int xnav_call_method( XNav *xnav, char *method, char *filter,
			 pwr_sAttrRef attrref, 
			 xmenu_eItemType item_type, 
			 xmenu_mUtility caller,
			 unsigned int priv, char *arg)
{
  pwr_tStatus 	sts;
  int		sel_cnt;
  pwr_tStatus (*method_func)( xmenu_sMenuCall *);
  pwr_tStatus (*filter_func)( xmenu_sMenuCall *);

  if (mcp == NULL)
    mcp = (xmenu_sMenuCall *)XtCalloc(1, sizeof(xmenu_sMenuCall));

  mcp->EditorContext = (void *)xnav;
  mcp->WindowContext = (void *)xnav->parent_wid;
  mcp->PointedSet = xmenu_eMenuSet_Object;

  mcp->Pointed = attrref;
  mcp->Caller = caller;
  mcp->ItemType = item_type;
  mcp->Priv = priv;
  mcp->SelectedSet = xmenu_eMenuSet_None;
  mcp->SelectedSet = xmenu_eMenuSet_Object;
  if ( arg)
    strcpy( mcp->Arg, arg);

  sel_cnt = 1;
  mcp->Selected = (pwr_sAttrRef *) XtCalloc( sel_cnt + 1, sizeof (pwr_sAttrRef));
  mcp->Selected[0] = attrref;
  mcp->Selected[sel_cnt].Objid = pwr_cNObjid;
  mcp->SelectCount = sel_cnt;

  if ( filter && strcmp( filter, "") != 0) {
    sts = xnav_GetMethod( filter, &filter_func);
    if ( EVEN(sts)) return sts;

    sts = (filter_func) ( mcp);
    if ( EVEN(sts)) return sts;
  }

  sts = xnav_GetMethod( method, &method_func);
  if ( EVEN(sts)) return sts;

  sts = (method_func) ( mcp);
  if ( EVEN(sts)) return sts;

  return XNAV__SUCCESS;
}

static void xnav_popup_unmap_cb(Widget w, xmenu_sMenuCall *ip)
{
  // XtFree( (char *)ip);
  XtDestroyWidget(w);
}

static void xnav_popup_button_cb (Widget w, XNav *xnav)
{
  Widget menu;
  int idx;
  pwr_tStatus sts;

  // Find the menu widget
  menu = XtParent(w);
  while (1) {
    if (strcmp(XtName(menu), "_popup") == 0 || 
	  strcmp(XtName(menu), "_pulldown") == 0)
      break;
    menu = XtParent(menu);
  }

  XtVaGetValues (w, XmNuserData, &idx, NULL);

  mcp->ChosenItem = idx;
  // xnav->set_clock_cursor();
  sts = xnav_CallMenuMethod( mcp, mcp->ChosenItem);
  if (EVEN(sts))
    xnav->message( 'E', XNav::get_message(sts));
  // xnav->reset_cursor();

}




