#ifndef grow_widget_h
#define grow_widget_h

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
	} growClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	growClassPart	grow_class;
	} GrowClassRec, *GrowWidgetClass;

typedef struct {
	void		*grow_ctx;
        int		(*init_proc)(GlowCtx *ctx, void *clien_data);
	int		is_navigator;
	void		*client_data;
	Widget		main_grow_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} GrowPart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	GrowPart	grow;
	} GrowRec, *GrowWidget;




Widget GrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget GrowCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_grow);

Widget ScrolledGrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *grow_w
);	

void GrowCtxFromWidget( Widget w, void **ctx);

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
