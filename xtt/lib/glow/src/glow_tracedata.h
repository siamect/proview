#ifndef glow_tracedata_h
#define glow_tracedata_h

#include <string.h>
#include "glow.h"
#include "pwr_privilege.h"

typedef pwr_mAccess glow_mAccess;
#define glow_mAccess_AllRt pwr_mAccess_AllRt

class GlowTraceData {
  public:
    GlowTraceData() : attr_type(glow_eTraceType_Boolean),
	color(glow_eDrawType_Inherit), color2(glow_eDrawType_Inherit),
        p(NULL), access(glow_mAccess_AllRt), cycle(glow_eCycle_Inherit)
	{
	  strcpy( data[0], "");
	  strcpy( data[1], "");
	  strcpy( data[2], "");
	  strcpy( data[3], "");
	  strcpy( data[4], "");
	  strcpy( data[5], "");
	  strcpy( data[6], "");
	  strcpy( data[7], "");
	  strcpy( data[8], "");
	  strcpy( data[9], "");
	  strcpy( ref_object, "");
	};

    char		data[10][120];
    glow_eTraceType	attr_type;
    glow_eDrawType	color;
    glow_eDrawType	color2;
    void		*p;
    glow_mAccess	access;
    glow_eCycle         cycle;
    char                ref_object[120];
};
#endif
