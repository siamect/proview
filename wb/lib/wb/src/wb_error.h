#ifndef wb_error_h
#define wb_error_h

#include "pwr.h"
#include "wb_status.h"

class wb_error : public wb_status
{
 public:
  wb_error() {};
  wb_error( pwr_tStatus sts) : wb_status(sts) {}
};

class wb_error_str : public wb_error
{
  string m_error_str;
    
 public:
  wb_error_str(string str) : m_error_str(str) {} 
  wb_error_str( pwr_tStatus sts, string str): wb_error(sts), m_error_str(str) {}
  string what() const
  {
    string s;
    if ( m_sts)
      s = m_error_str + ", " + wb_error::what();
    else
      s = m_error_str;
    return s;
  }
};

#endif
