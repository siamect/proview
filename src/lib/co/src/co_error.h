#ifndef co_error_h
#define co_error_h

#include "pwr.h"
#include "co_status.h"

class co_error : public co_status
{
 public:
  co_error() {};
  co_error( pwr_tStatus sts) : co_status(sts) {}
};

class co_error_str : public co_error
{
  string m_error_str;
    
 public:
  co_error_str(string str) : m_error_str(str) {} 
  co_error_str( pwr_tStatus sts, string str): co_error(sts), m_error_str(str) {}
  string what() const
  {
    string s;
    if ( m_sts)
      s = m_error_str + ", " + co_error::what();
    else
      s = m_error_str;
    return s;
  }
};

#endif
