
#include <string.h>
#include "wb_attrname.h"

wb_attrname::wb_attrname( const char *name)
{
  try {
    char str[80];
    if ( *name != '.') {
      strcpy( str, ".");
      strcat( str, name);
    }
    else
      strcpy( str, name);
    m_nrep = new wb_nrep( str);
    m_sts = LDH__SUCCESS;
    m_nrep->ref();
  }
  catch ( wb_error &e) {
    m_sts = e.sts();
    m_nrep = 0;
  }
}

