#ifndef curve_widget_h
#define curve_widget_h

#if defined __cplusplus
  extern "C" {
#endif

/* X Toolkit is compiled with member alignment */

#if defined OS_VMS
#pragma member_alignment save
#pragma member_alignment
#endif

#include <X11/CoreP.h>
#include <X11/CompositeP.h>

typedef struct {
	XmOffsetPtr	*offset;
	int		reserved;
	} curveClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	curveClassPart	curve_class;
	} CurveClassRec, *CurveWidgetClass;

typedef struct {
	void		*curve_ctx;
        int		(*init_proc)(GlowCtx *ctx, void *clien_data);
	int		is_navigator;
	void		*client_data;
	Widget		main_curve_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} CurvePart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	CurvePart	curve;
	} CurveRec, *CurveWidget;




Widget CurveCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget CurveCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_curve);

Widget ScrolledCurveCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *curve_w
);	

void CurveCtxFromWidget( Widget w, void **ctx);

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
