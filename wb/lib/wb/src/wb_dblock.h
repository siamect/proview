#ifndef wb_dblock_h
#define wb_dblock_h

#include <vector.h>

class wb_lockfile
{
 public:
  wb_lockfile( char *name) : removed(false)
    { strcpy( fname, name); }
  pwr_tFileName fname;
  bool removed;
};

class wb_dblock
{
 private:
  static vector<wb_lockfile> m_lockfiles;

  static char *lockname( char *name);

 public:
  static bool is_locked( char *name, char *user = 0);
  static void dblock( char *name);
  static void dbunlock( char *name);
  static void dbunlock_all();
};

#endif
