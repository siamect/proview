#ifndef flow_x_h
#define flow_x_h

#if defined __cplusplus
extern "C" {
#endif

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void flow_SetInputFocus( Widget w);
Boolean flow_IsManaged( Widget w);
Boolean flow_IsShell( Widget w);
Boolean flow_IsRealized( Widget w);
void flow_AddCloseVMProtocolCb( Widget shell, XtCallbackProc callback,
	void *client_data);
void flow_Bell( Widget w);
void flow_UnmapWidget( Widget w);
void flow_MapWidget( Widget w);
Screen *flow_Screen( Widget w);
Display *flow_Display( Widget w);
Window flow_Window( Widget w);
int flow_IsViewable( Widget w);

#if defined __cplusplus
}
#endif
#endif
