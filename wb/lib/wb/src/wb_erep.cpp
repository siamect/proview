#include <iostream.h>
#include <fstream.h>

#include "pwr.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_vrepwbl.h"  // Should be wb_vrepdbs.h ...
#include "wb_vrepdbs.h"
#include "wb_vrepdb.h"
#include "wb_cdrep.h"
#include "wb_orep.h"
#include "wb_tdrep.h"
#include "wb_adrep.h"
#include "wb_name.h"
#include "wb_ldh_msg.h"

extern "C" {
#include "co_dcli.h"
#include "co_cdh.h"
#include "rt_load.h"
#include "wb_pwrs.h"
}

pwr_dImport pwr_BindClasses(System);
pwr_dImport pwr_BindClasses(Base);

wb_erep::wb_erep() : m_dir_cnt(0), m_volatile_idx(0), m_buffer_max(10)
{
  m_merep = new wb_merep(0);
}

wb_erep::~wb_erep()
{
  delete m_merep;
}

void wb_erep::unref()
{
  if (--m_nRef == 0)
    delete this;
}

wb_erep *wb_erep::ref()
{
  m_nRef++;
  return this;
}


// Get first volume
wb_vrep *wb_erep::volume( pwr_tStatus *sts)
{
  vrep_iterator it = m_vrepdb.begin(); 
  if ( it == m_vrepdb.end()) {
    it = m_vrepdbs.begin();
    if ( it == m_vrepdbs.end()) {
      *sts = LDH__NOSUCHVOL;
      return 0;
    }
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

wb_vrep *wb_erep::volume(pwr_tStatus *sts, pwr_tVid vid)
{
  vrep_iterator it = m_vrepdb.find( vid); 
  if ( it != m_vrepdb.end()) {
    *sts = LDH__SUCCESS;
    return it->second;
  }

  it = m_vrepdbs.find(vid);
  if ( it != m_vrepdbs.end()) {
    *sts = LDH__SUCCESS;
    return it->second;
  }

  it = m_vrepextern.find(vid);
  if ( it != m_vrepextern.end()) {
    *sts = LDH__SUCCESS;
    return it->second;
  }

  for ( buffer_iterator itb = m_vrepbuffer.begin(); itb != m_vrepbuffer.end(); itb++) {
    if ( (*itb)->vid() == vid) {
      *sts = LDH__SUCCESS;
      return *itb;
    }
  }

  *sts = LDH__NOSUCHVOL;
  return 0;
}


wb_vrep *wb_erep::volume(pwr_tStatus *sts, const char *name)
{
  vrep_iterator it;
  for ( it = m_vrepdb.begin(); it != m_vrepdb.end(); it++) {
    if ( cdh_NoCaseStrcmp( it->second->name(), name) == 0) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
  }
  for ( it = m_vrepdbs.begin(); it != m_vrepdbs.end(); it++) {
    if ( cdh_NoCaseStrcmp( it->second->name(), name) == 0) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
  }
  for ( it = m_vrepextern.begin(); it != m_vrepextern.end(); it++) {
    if ( cdh_NoCaseStrcmp( it->second->name(), name) == 0) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
  }
  *sts = LDH__NOSUCHVOL;
  return 0;
}

wb_vrep *wb_erep::externVolume(pwr_tStatus *sts, pwr_tVid vid)
{
  vrep_iterator it = m_vrepextern.find( vid); 
  if ( it == m_vrepextern.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

wb_vrep *wb_erep::bufferVolume(pwr_tStatus *sts)
{
  buffer_iterator it = m_vrepbuffer.begin(); 
  if ( it == m_vrepbuffer.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return *it;
}

wb_vrep *wb_erep::bufferVolume(pwr_tStatus *sts, char *name)
{
  for ( buffer_iterator it = m_vrepbuffer.begin(); it != m_vrepbuffer.end(); it++) {
    if ( cdh_NoCaseStrcmp( name, (*it)->name()) == 0) {
      *sts = LDH__SUCCESS;
      return *it;
    }
  }
  *sts = LDH__NOSUCHVOL;
  return 0;
}

wb_vrep *wb_erep::nextVolume(pwr_tStatus *sts, pwr_tVid vid)
{
  // Search in db
  vrep_iterator it = m_vrepdb.find( vid);
  if ( it != m_vrepdb.end()) {
    it++;
    if ( it != m_vrepdb.end()) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
    else {
      // Next volume in dbs
      it = m_vrepdbs.begin();
      if ( it != m_vrepdbs.end()) {
	*sts = LDH__SUCCESS;
	return it->second;
      }
      else {
	*sts = LDH__NOSUCHVOL;
	return 0;
      }
    }
  }

  // Search in dbs
  it = m_vrepdbs.find(vid);
  if ( it != m_vrepdbs.end()) {
    it++;
    if ( it != m_vrepdbs.end()) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
    else {
      *sts = LDH__NOSUCHVOL;
      return 0;
    }
  }

  // Search in buffer
  for ( buffer_iterator itb = m_vrepbuffer.begin(); itb != m_vrepbuffer.end(); itb++) {
    if ( (*itb)->vid() == vid) {
      itb++;
      if ( itb != m_vrepbuffer.end()) {
	*sts = LDH__SUCCESS;
	return *itb;
      }
      else {
	*sts = LDH__NOSUCHVOL;
	return 0;
      }
    }
  }

  *sts = LDH__NOSUCHVOL;
  return 0;
}

void wb_erep::addDb( pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdb.find( vrep->vid()); 
  if ( it == m_vrepdb.end()) {
    m_vrepdb[vrep->vid()] = vrep;
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__VOLIDALREXI;
}

void wb_erep::addDbs( pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdbs.find( vrep->vid()); 
  if ( it == m_vrepdbs.end()) {
    m_vrepdbs[vrep->vid()] = vrep;
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else {
    *sts = LDH__VOLIDALREXI;
    return;
  }

  if ( vrep->cid() == pwr_eClass_ClassVolume)
    m_merep->addDbs( sts, (wb_mvrep *)vrep);
}

void wb_erep::addExtern( pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepextern.find( vrep->vid()); 
  if ( it == m_vrepextern.end()) {
    m_vrepextern[vrep->vid()] = vrep;
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else {
    *sts = LDH__VOLIDALREXI;
    return;
  }
}

wb_vrep *wb_erep::findBuffer( pwr_tVid vid)
{
  for ( buffer_iterator itb = m_vrepbuffer.begin(); itb != m_vrepbuffer.end(); itb++) {
    if ( (*itb)->vid() == vid)
      return *itb;
  }
  return 0;
}

void wb_erep::addBuffer( pwr_tStatus *sts, wb_vrep *vrep)
{
  wb_vrep *v = findBuffer( vrep->vid());
  if ( !v) {
    // Remove oldest buffer
    if ( (int) m_vrepbuffer.size() >= m_buffer_max) {      
      m_vrepbuffer.erase( m_vrepbuffer.begin());
    }

    m_vrepbuffer.push_back( vrep);
    vrep->ref();
    *sts = LDH__SUCCESS;
  }
  else {
    *sts = LDH__VOLIDALREXI;
    return;
  }
}

void wb_erep::removeDb(pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdb.find( vrep->vid()); 
  if ( it == m_vrepdb.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  it->second->unref();
  m_vrepdb.erase( it);
  *sts = LDH__SUCCESS;
}

void wb_erep::removeDbs(pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepdbs.find( vrep->vid()); 
  if ( it == m_vrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  it->second->unref();
  m_vrepdbs.erase( it);
  *sts = LDH__SUCCESS;
}

void wb_erep::removeExtern(pwr_tStatus *sts, wb_vrep *vrep)
{
  vrep_iterator it = m_vrepextern.find( vrep->vid()); 
  if ( it == m_vrepextern.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  it->second->unref();
  m_vrepextern.erase( it);
  *sts = LDH__SUCCESS;
}

void wb_erep::removeBuffer(pwr_tStatus *sts, wb_vrep *vrep)
{
  for ( buffer_iterator it = m_vrepbuffer.begin(); it != m_vrepbuffer.end(); it++) {
    if ( *it == vrep) {
      vrep->unref();
      m_vrepbuffer.erase( it);
      *sts = LDH__SUCCESS;
      return;
    }
  }
  *sts = LDH__NOSUCHVOL;
  return;
}

void wb_erep::load( pwr_tStatus *sts)
{
  loadDirList( sts);
  if ( EVEN(*sts)) return;
  else if ( *sts == LDH__PROJCONFIG) {
    pwr_tStatus status;
    loadCommonMeta( &status);
    if ( EVEN(status)) {
      *sts = status;
      return;
    }
    bindMethods();
    return;
  }
  loadCommonMeta( sts);
  if ( EVEN(*sts)) return;

  loadMeta( sts);
  bindMethods();
  loadLocalWb( sts);
}

void wb_erep::loadDirList( pwr_tStatus *status)
{
  // Read directory file
  char line[200];
  char fname[200];

  dcli_translate_filename( m_dir_list[0], "$pwr_load");
  strcat( m_dir_list[0], "/");
  m_dir_cnt = 1;

  dcli_translate_filename( fname, load_cNameFilePath);
  ifstream fp( fname, ios::in);
  if ( !fp) {
    *status = LDH__PROJCONFIG;
    return;
  }
  
  while ( fp.getline( line, sizeof(line))) {
    if ( m_dir_cnt > (int) (sizeof(m_dir_list)/sizeof(m_dir_list[0]) - 1))
      break;
    dcli_translate_filename( m_dir_list[m_dir_cnt], line);
    if ( m_dir_list[m_dir_cnt][strlen(m_dir_list[m_dir_cnt])-1] != '/')
      strcat( m_dir_list[m_dir_cnt], "/");
    m_dir_cnt++;
  }
  fp.close();
}

void wb_erep::loadCommonMeta( pwr_tStatus *status)
{
  char fname[200];
  char fdir[200];
  char vname[200];
  char line[200];
  int i;
  pwr_tVid vid;
  pwr_tStatus sts;

  for ( i = 0; i < m_dir_cnt; i++) {

    // Load metavolumes
    strcpy( fdir, m_dir_list[i]);
    strcpy( fname, fdir);
    strcat( fname, load_cNameCmnVolumeList);

    cout << "File : " << fname << endl;
    ifstream fpm( fname, ios::in);
    if ( !fpm) {
      if ( i == 0) {
        *status = LDH__NOBASE;
        return;
      }
      else
        continue;
    }
    while ( fpm.getline( line, sizeof(line))) {
      char vol_array[2][80];
      int nr;

      if ( line[0] == '#')
        continue;

      nr = dcli_parse( line, " ", "", (char *)vol_array,
                       sizeof(vol_array)/sizeof(vol_array[0]),
                       sizeof(vol_array[0]), 0);
      if ( nr != 2)
        cout << "Syntax error in file: " << fname << endl;

      // Load this volume
      cdh_ToLower( vol_array[0], vol_array[0]);
      strcpy( vname, fdir);
      strcat( vname, vol_array[0]);
      strcat( vname, ".dbs");
      cdh_StringToVolumeId( vol_array[1], &vid);
      cout << "Loading volume: " << vname << " " << vid << endl;

      wb_vrepdbs *vrep = new wb_vrepdbs( this, vname);
      try {
        vrep->load();
        addDbs( &sts, vrep);
      }
      catch ( wb_error& e) {
        cout << "** Unable to open volume " << vname << " " << e.what() << endl;
      }
    }
    fpm.close();
  }
  *status = LDH__SUCCESS;
}

void wb_erep::loadMeta( pwr_tStatus *status)
{
  // Load local metavolumes
  char found_file[200];
  char fname[200];
  char vname[200];
  char line[200];
  int i;
  pwr_tVid vid;
  pwr_tStatus sts;
  wb_vrepdbs *vrep;
  int vol_cnt = 0;

  strcpy( fname, load_cNameVolumeList);
  dcli_translate_filename( fname, fname);

  cout << "File : " << fname << endl;

  ifstream fpm( fname, ios::in);
  if ( !fpm) {
    *status = LDH__PROJCONFIG;
    return;
  }

  while ( fpm.getline( line, sizeof(line))) {
    char vol_array[5][80];
    int nr;

    if ( line[0] == '#')
      continue;

    cout << line << endl;
    nr = dcli_parse( line, " ", "", (char *)vol_array,
                     sizeof(vol_array)/sizeof(vol_array[0]),
                     sizeof(vol_array[0]), 0);
    if ( nr != 4)
      cout << "Syntax error in file: " << fname << endl;

    cdh_StringToVolumeId( vol_array[1], &vid);

    if ( (cdh_NoCaseStrcmp( vol_array[2], "ClassVolume") == 0) ||
         (strcmp( vol_array[3], "load") == 0)) {
      // Load dbs for this volume
      cdh_ToLower( vol_array[0], vol_array[0]);

      if ( strcmp( vol_array[3], "cnf") == 0) {
        // Configured in this project, load from pwrp_load
        strcpy( vname, "$pwrp_load/");
        strcat( vname, vol_array[0]);
        strcat( vname, ".dbs");
        dcli_translate_filename( vname, vname);
        cout << "Found: " << found_file << endl;
        // Load...
        try {
          vrep = new wb_vrepdbs( this, vname);
          vrep->load();
          addDbs( &sts, vrep);
          vol_cnt++;
        }
        catch ( wb_error& e) {
          cout << "** Unable to open volume " << vname << " " << e.what() << endl;
        }
      }
      else {
        // Imported loadfile
        for ( i = 0; i < m_dir_cnt; i++) {
          strcpy( vname, m_dir_list[i]);
          strcat( vname, vol_array[0]);
          strcat( vname, ".dbs");
          cout << "Try: " << vname << endl;
          sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
          dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
          if ( ODD(sts)) {
            cout << "Found: " << found_file << endl;
            // Load...
            try {
              vrep = new wb_vrepdbs( this, vname);
              vrep->load();
              // vrep->name( vol_array[0]);
              addDbs( &sts, vrep);
            }
            catch ( wb_error& e) {
              cout << "** Unable to open volume " << vname << " " << e.what() << endl;
            }
            break;
          }
        }
      }
    }
    else {
      // Load db for this volume
      strcpy( vname, "$pwrp_db/");
      strcat( vname, vol_array[0]);
      cdh_ToLower( vname, vname);
      strcat( vname, ".db");
      dcli_translate_filename( vname, vname);

      wb_vrepdb *vrepdb = new wb_vrepdb( this, vname);
      vrepdb->name(vol_array[0]);
      addDb( &sts, vrepdb);
      vol_cnt++;
    }
  }
  fpm.close();

  // Load directory volume

  strcpy( vname, "$pwrp_db/directory.db");
  dcli_translate_filename( vname, vname);

  sts = dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( vname, found_file, DCLI_DIR_SEARCH_END);
  if ( ODD(sts)) {
    wb_vrepdb *vrepdb = new wb_vrepdb( this, vname);
    vrepdb->name("directory");
    addDb( &sts, vrepdb);
  }
  if ( EVEN(sts))
    *status = LDH__PROJCONFIG;
  else if ( !vol_cnt)
    *status = LDH__PROJCONFIG;
  else
    *status = LDH__SUCCESS;
}

void wb_erep::loadLocalWb( pwr_tStatus *rsts)
{
  pwr_tStatus sts;
  char found_file[200];

  sts = dcli_search_file( load_cNameLocalWb, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( load_cNameLocalWb, found_file, DCLI_DIR_SEARCH_END);
  if ( ODD(sts)) {
    try {
      wb_vrepwbl *vrep = new wb_vrepwbl( this);
      sts = vrep->load( found_file);
      if ( ODD(sts))
        addExtern( &sts, vrep);
      else {
        *rsts = sts;
        cout << "** Failiure loading local workbench volume" << endl;
        return;
      }
    }
    catch ( wb_error& e) {
      *rsts = e.sts();
      cout << "** Failiure loading local workbench volume" << endl;
      return ;
    }
    cout << "-- Local workbench volume loaded" << endl;
  }
  *rsts = LDH__SUCCESS;
}

wb_cdrep *wb_erep::cdrep( pwr_tStatus *sts, const wb_orep& o)
{
  wb_vrep *vrep = volume(sts, o.vid());
  if ( EVEN(*sts)) return 0;

  return vrep->merep()->cdrep( sts, o);
}

wb_tdrep *wb_erep::tdrep( pwr_tStatus *sts, const wb_adrep& a)
{
  pwr_tVid vid = cdh_TidToVid(a.type());
  wb_vrep *vrep = volume(sts, vid);
  if ( EVEN(*sts)) return 0;

  return vrep->merep()->tdrep( sts, a);
}

wb_orep *wb_erep::object(pwr_tStatus *sts, pwr_tOid oid)
{
  wb_vrep *vrep = volume( sts, oid.vid);
  if ( EVEN(*sts)) return 0;

  return vrep->object( sts, oid);
}

wb_orep *wb_erep::object( pwr_tStatus *sts, const char *name)
{
  wb_name n(name);
  
  if ( n.evenSts()) {
    *sts = n.sts();
    return 0;
  }

  wb_vrep *vrep = volume( sts, n.volume());
  if ( EVEN(*sts)) return 0;

  return vrep->object( sts, n);
}

wb_orep *wb_erep::object(pwr_tStatus *sts, wb_name &name)
{
  if ( name.evenSts()) {
    *sts = name.sts();
    return 0;
  }

  wb_vrep *vrep = volume( sts, name.volume());
  if ( EVEN(*sts)) return 0;

  return vrep->object( sts, name);
}


void wb_erep::method( pwr_tStatus *sts, char *methodName, wb_tMethod *method)
{
  string key = string( methodName);
  methods_iterator it = m_methods.find( key);
  if ( it == m_methods.end()) {
    *sts = LDH__NOMETHOD;
    return;
  }
  *sts = LDH__SUCCESS;
  *method = it->second;
}

void wb_erep::bindMethods()
{
  int i, j;
  pwr_tStatus sts;
  char str[200];

  for (i = 0;; i++) {
    if (pwr_gSystem_ClassMethods[i].ClassName[0] == '\0')
      break;
    for (j = 0;; j++) {
      if ((*pwr_gSystem_ClassMethods[i].Methods)[j].MethodName[0] == '\0')
	break;
      strcpy( str, pwr_gSystem_ClassMethods[i].ClassName);
      strcat( str, "-");
      strcat( str, (*pwr_gSystem_ClassMethods[i].Methods)[j].MethodName);
      string key = string( str);
      m_methods[ key] = (*pwr_gSystem_ClassMethods[i].Methods)[j].Method;
    }
  }

  for (i = 0;; i++) {
    if (pwr_gBase_ClassMethods[i].ClassName[0] == '\0') break;
    wb_name cname = wb_name( pwr_gBase_ClassMethods[i].ClassName);
    wb_cdrep *cdrep = m_merep->cdrep( &sts, cname);
    if ( EVEN(sts))
      continue;
    
    for (j = 0;; j++) {
      if ((*pwr_gBase_ClassMethods[i].Methods)[j].MethodName[0] == '\0')
	break;
      strcpy( str, pwr_gBase_ClassMethods[i].ClassName);
      strcat( str, "-");
      strcat( str, (*pwr_gBase_ClassMethods[i].Methods)[j].MethodName);
      string key = string( str);
      m_methods[ key] = (*pwr_gBase_ClassMethods[i].Methods)[j].Method;
    }
    delete cdrep;
  }

#if DEBUG
  for (  methods_iterator it = m_methods.begin(); it != m_methods.end(); it++) {
    cout << " Method: " << it->first << endl;
  }
#endif
}

int wb_erep::nextVolatileVid( pwr_tStatus *sts, char *name)
{
  pwr_tVid vid = ldh_cVolatileVolMin + m_volatile_idx++;
  if ( vid > ldh_cVolatileVolMax) {
    // Recycle identities, and hope that the old volumes are history
    m_volatile_idx = 0;
    vid = ldh_cVolatileVolMin + m_volatile_idx++;
  }
  if ( name)
    // Suggest a name
    sprintf( name, "Buffer%03d", vid & 255);

  return vid;
}






