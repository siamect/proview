#include <sys/stat.h>
#include <iostream.h>
#include <fstream.h>

#include "pwr.h"
#include "wb_dblock.h"
#include "co_dcli.h"

vector<wb_lockfile> wb_dblock::m_lockfiles;

char *wb_dblock::lockname( char *name)
{
  static pwr_tFileName fname;

  strcpy( fname, name);
  strcat( fname, ".lock");
  dcli_translate_filename( fname, fname);
  return fname;
}

bool wb_dblock::is_locked( char *name, char *user)
{
  struct stat info;
  if ( stat( lockname( name), &info) != -1) {
    if ( user) {
      ifstream fp( lockname( name));
      fp.getline( user, 80);
      fp.close();
    }
    return true;
  }
  return false;
}

void wb_dblock::dblock( char *name)
{
  char *value;

  ofstream fp( lockname( name));
  value = getenv( "USER");
  if ( value)
    fp << value << endl;
  else
    fp << "Unknown" << endl;
  fp.close();
  wb_lockfile lf( lockname( name));
  m_lockfiles.push_back(lf);
}

void wb_dblock::dbunlock( char *name)
{
  pwr_tCmd cmd;
  sprintf( cmd, "rm %s", lockname(name));
  system( cmd);

  for ( int i = 0; i < (int) m_lockfiles.size(); i++) {
    if ( strcmp( m_lockfiles[i].fname, lockname(name)) == 0) {
      m_lockfiles[i].removed = true;
      break;
    }
  }
}

void wb_dblock::dbunlock_all()
{
  pwr_tCmd cmd;
  for ( int i = 0; i < (int) m_lockfiles.size(); i++) {
    if ( !m_lockfiles[i].removed) {
      sprintf( cmd, "rm %s", m_lockfiles[i].fname);
      system( cmd);
    }
  }
  m_lockfiles.clear();
}

