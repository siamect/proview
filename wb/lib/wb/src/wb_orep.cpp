#include "wb_orep.h"

wb_orep::wb_orep() : m_nRef(0) 
{
  // printf( "orep C: %x %d\n", this, ++ocnt);
}

void wb_orep::unref()
{
  if (--m_nRef == 0) {
    // printf( "orep  : %x %d\n", this, --ocnt);  
    delete this;
  }
}

wb_orep *wb_orep::ref()
{
  m_nRef++;
  return this;
}
