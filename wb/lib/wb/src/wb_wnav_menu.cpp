
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
#include "wb_ldh.h"
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

#include "wb_wtt.h"
#include "wb_wnav.h"
#include "wb_wnav_item.h"
#include "co_dcli_msg.h"
#include "wb_wnav_msg.h"


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
static ldh_sMenuCall *mcp = NULL;

static void wnav_popup_button_cb (Widget w, Wtt *wtt);
static void wnav_popup_unmap_cb(Widget w, Wtt *wtt);

static Widget wnav_build_menu(
	Widget Parent,
	int   MenuType,
	char *MenuTitle,
	void *MenuUserData,
	void (*Callback)( Widget, Wtt *),
	void *CallbackData,
	ldh_sMenuItem *Items,
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
    if (Items[*idx].Item == ldh_eMenuItem_Cascade) 
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
        wnav_build_menu(Menu, MENU_PULLDOWN, Items[i].Name, MenuUserData, 
		  Callback, CallbackData, Items, idx);
        (*idx)--;
      }
    }
    else 
    {
      if (Items[*idx].Item == ldh_eMenuItem_Separator)
        Class = xmSeparatorGadgetClass;
      else
        Class = xmPushButtonGadgetClass;
 
      W = XtVaCreateManagedWidget(Items[*idx].Name, 
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


Widget wtt_create_popup_menu( Wtt *wtt, pwr_tObjid objid)
{
  pwr_tStatus 	sts;
  int 		i;
  Widget 	popup;
  pwr_sAttrRef	*sel1_list;
  int           *sel1_is_attr;
  int		sel1_cnt;
  pwr_sAttrRef	*sel2_list;
  int           *sel2_is_attr;
  int		sel2_cnt;

  if (mcp == NULL)
    mcp = (ldh_sMenuCall *)XtCalloc(1, sizeof(ldh_sMenuCall));

  mcp->EditorContext = (void *)wtt;
  mcp->WindowContext = (void *)wtt->wnav_form;
  mcp->PointedSet = ldh_eMenuSet_Object;

  mcp->Pointed.Objid = objid;

  mcp->PointedSession = wtt->ldhses;
  mcp->Pointed.Size = 0;
  mcp->Pointed.Offset = 0;
  mcp->Pointed.Flags.m = 0;
  mcp->SelectedSet = ldh_eMenuSet_None;
  mcp->SelectedSession = wtt->ldhses;


  // Fetch selections
  if ( wtt->wnav_mapped)
    wtt->wnav->get_select( &sel1_list, &sel1_is_attr, &sel1_cnt);
  else
    sel1_cnt = 0;
  if ( wtt->wnavnode_mapped)
    wtt->wnavnode->get_select( &sel2_list, &sel2_is_attr, &sel2_cnt);
  else
    sel2_cnt = 0;

  if (sel1_cnt + sel2_cnt != 0) 
  {
    mcp->SelectedSet = sel1_cnt + sel2_cnt > 1 ? ldh_eMenuSet_Many : ldh_eMenuSet_Object;
  }  

  mcp->Selected = (pwr_sAttrRef *) XtCalloc( sel1_cnt + sel2_cnt + 1, sizeof (pwr_sAttrRef));
  if ( sel1_cnt)
    memcpy( mcp->Selected, sel1_list, sel1_cnt * sizeof(pwr_sAttrRef));
  if ( sel2_cnt)
    memcpy( &mcp->Selected[sel1_cnt], sel2_list, sel2_cnt * sizeof(pwr_sAttrRef));
  if ( sel1_cnt) {
    free( sel1_list);
    free( sel1_is_attr);
  }
  if ( sel2_cnt) {
    free( sel2_list);
    free( sel2_is_attr);
  }

  mcp->Selected[sel1_cnt + sel2_cnt].Objid = pwr_cNObjid;
  mcp->SelectCount = sel1_cnt + sel2_cnt;

  sts = ldh_GetMenu( wtt->ldhses, mcp);  
  if (EVEN(sts) || mcp->ItemList[0].Level == 0) {
    return NULL;
  }

  i = 0;
  popup = wnav_build_menu( wtt->wnav_form, MENU_POPUP, "", mcp, 
	wnav_popup_button_cb, (void *) wtt, 
	(ldh_sMenuItem *) mcp->ItemList, &i); 
  if (popup != NULL) 
    XtAddCallback (popup, XmNunmapCallback, 
				(XtCallbackProc)wnav_popup_unmap_cb, wtt);

  return popup;

}/* hied_fun_popup_bc */ 

static void wnav_popup_unmap_cb(Widget w, Wtt *wtt)
{
  XtDestroyWidget(w);
}

static void wnav_popup_button_cb (Widget w, Wtt *wtt)
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
  wtt->set_clock_cursor();
  sts = ldh_CallMenuMethod( mcp, mcp->ChosenItem);
  if (EVEN(sts))
    wtt->message( 'E', wnav_get_message(sts));
  wtt->reset_cursor();

}

