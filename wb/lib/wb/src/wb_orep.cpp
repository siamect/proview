#include "wb_orep.h"

wb_orep::wb_orep() : m_nRef(0) 
{
}

void wb_orep::unref()
{
  if (--m_nRef == 0) {
    delete this;
  }
}

wb_orep *wb_orep::ref()
{
  m_nRef++;
  return this;
}
