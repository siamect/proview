#ifndef wb_attrname_h
#define wb_attrname_h

#include "wb_name.h"


class wb_attrname : public wb_name
{
 public:
  wb_attrname() : wb_name() {}
  wb_attrname(const char *);
};

#endif
