#ifndef brow_widget_h
#define brow_widget_h

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
	} browClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	browClassPart	brow_class;
	} BrowClassRec, *BrowWidgetClass;

typedef struct {
	void		*brow_ctx;
        int		(*init_proc)(FlowCtx *ctx, void *clien_data);
	int		is_navigator;
	void		*client_data;
	Widget		main_brow_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} BrowPart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	BrowPart	brow;
	} BrowRec, *BrowWidget;




Widget BrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget BrowCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_brow);

Widget ScrolledBrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *brow_w
);	

void BrowCtxFromWidget( Widget w, void **ctx);

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
