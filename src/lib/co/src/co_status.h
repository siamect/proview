#ifndef co_status_h
#define co_status_h

#include <string>
#include "pwr.h"
extern "C" {
#include "co_msg.h"
}
using namespace std;

class co_status
{
 protected:
  pwr_tStatus m_sts;

 public:
  co_status() : m_sts(0) {}
  co_status( pwr_tStatus sts) : m_sts(sts) {}
  virtual ~co_status() {};
  pwr_tStatus sts() const { return m_sts;}
  bool evenSts() const { return EVEN(m_sts);}
  bool oddSts() const { return ODD(m_sts);}
  virtual string what() const
  { 
    char msg[200];

    msg_GetMsg( m_sts, msg, sizeof(msg));
    string s(msg);
    return s;
  }
};

#endif







