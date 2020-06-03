

#include "pwr.h"
#include "rt_mh_outunit.h"
#include "co_tst_log.h"

#define SUP_ASIZE 5
#define SUP_ISIZE 1
#define SUP_CYCLES 10

typedef struct {
  pwr_tBoolean *dp;
  pwr_tOid oid;
  int active;
  int eactive;
  int prio;
  mh_sEventId eventid;
  mh_mEventStatus status;
  mh_mEventStatus estatus;
  int check;
} sSup;

class ra_mhtest {
  static ra_mhtest *m_mh;
  pwr_tStatus m_sts;
  tst_log *m_log;
  pwr_tOid m_user;
  int m_clean;
  int m_checkstatus;
  sSup m_sup[SUP_ASIZE+SUP_ISIZE][32];
  int m_errcnt;

  static pwr_tStatus mh_ack_bc(mh_sAck* MsgP);
  static pwr_tStatus mh_return_bc(mh_sReturn* MsgP);
  static pwr_tStatus mh_alarm_bc(mh_sMessage* MsgP);
  static pwr_tStatus mh_block_bc(mh_sBlock* MsgP);
  static pwr_tStatus mh_cancel_bc(mh_sReturn* MsgP);
  static pwr_tStatus mh_info_bc(mh_sMessage* MsgP);
  static pwr_tStatus mh_clear_alarmlist_bc(pwr_tNodeIndex nix);
  static pwr_tStatus mh_clear_blocklist_bc(pwr_tNodeIndex nix);
  static pwr_tStatus mh_alarmstatus_bc(mh_sAlarmStatus* MsgP);

  unsigned int rand();
  void clean();
  void receive();
  sSup *find_sup(mh_sEventId *id);

 public:
  ra_mhtest();
  ~ra_mhtest();
  tst_log *log() {return m_log;}


  void OutunitConnect();
  void OutunitTest();
};
