/* co_mrm_util.h -- Motif utitlities

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

typedef enum {
	mrm_eUtility_Xtt,
	mrm_eUtility_Admin,
	mrm_eUtility_Wtt,
	mrm_eUtility_Ge,
	mrm_eUtility_PlcEditor,
	mrm_eUtility_WAtt,
	mrm_eUtility_Attr,
	mrm_eUtility_Wda
	} mrm_eUtility;

void mrm_TextInit( Widget w, XtActionProc value_changed_cb, mrm_eUtility utility);
int mrm_TextInput( Widget w, XEvent *event, char *recall, int line_size, 
	int recall_size, int *current_recall_line);
int mrm_IsIconicState( Widget W);
void mrm_PositionPopup( Widget popup, Widget parent, int x, int y);

