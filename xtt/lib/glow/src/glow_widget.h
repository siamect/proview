#ifndef glow_widget_h
#define glow_widget_h

#if defined __cplusplus
  extern "C" {
#endif

#if defined OS_VMS
#pragma member_alignment save
#pragma member_alignment
#endif

#include <X11/CoreP.h>
#include <X11/CompositeP.h>


typedef struct {
	XmOffsetPtr	*offset;
	int		reserved;
	} GlowClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	GlowClassPart	glow_class;
	} GlowClassRec, *GlowWidgetClass;

typedef struct {
	void		*glow_ctx;
        int		(*init_proc)(GlowCtx *ctx, void *client_data);
	int		is_navigator;
	void		*client_data;
	int		scrollbars;
	Widget		main_glow_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} GlowPart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	GlowPart	glow;
	} GlowRec, *GlowWidget;

Widget GlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int	(*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget GlowCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_glow);

Widget ScrolledGlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *glow_w
);	

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
