#ifndef flow_pscript_h
#define flow_pscript_h

#include <stdio.h>
#include "flow.h"

class FlowPscript {
  public:
    FlowPscript( char *filename, void *flow_ctx, int page_border, int *sts);
    ~FlowPscript();
     int print_page( double ll_x, double ll_y, double ur_x, double ur_y);
     int rect( double x, double y, double width, double height, flow_eDrawType type, double idx);
     int filled_rect( double x, double y, double width, double height, flow_eDrawType type, double idx);
     int arc( double x, double y, double width, double height, int angel1, int angel2,
		flow_eDrawType type, double idx);
     int line( double x1, double y1, double x2, double y2, flow_eDrawType type, double idx);
     int text( double x, double y, char *text, int len,  flow_eDrawType type, int size);
     int pixmap( double x, double y, flow_sPixmapDataElem *data, flow_eDrawType type);
     int arrow( double x1, double y1, double x2, double y2, 
	double x3, double y3, flow_eDrawType type, double idx);
     void move( double x, double y);
     void setlinewidth( double idx);
     void rowbreak();
     double offset_x;
     double offset_y;
     FILE *file;
     char fname[80];
     int border;
     void *ctx;
};

#endif
