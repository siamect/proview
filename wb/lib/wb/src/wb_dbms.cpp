/* 
 * Proview   $Id: wb_dbms.cpp,v 1.9 2008-10-15 06:04:55 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#if defined PWRE_CONF_MYSQL
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "pwr.h"
#include "pwr_class.h"
#include "co_dcli.h"
#include "co_syi.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "wb_destination.h"
#include "wb_dbms.h"
#include "wb_name.h"
#include "wb_export.h"
#include "wb_convert_volume.h"
#include "wb_utl_api.h"
#include "co_cdh.h"
#include <iostream>

wb_dbms::wb_dbms() :
  m_vid(0), m_cid(0), m_env(0), m_con(0), m_txn(0)
{
  initTables();
}

wb_dbms::wb_dbms(pwr_tVid vid) :
  m_vid(0), m_cid(0), m_env(0), m_con(0), m_txn(0)
{
  initTables();
}

int wb_dbms::close()
{
  if (m_txn) {
    m_txn->abort();
  }

  m_t_ohead->close();
  m_t_rbody->close();
  m_t_dbody->close();
  m_t_class->close();
  m_t_name->close();
  m_t_info->close();

  m_env->close();

  return 0;
}

void wb_dbms::copy(wb_export &e, const char *fileName)
{
  pwr_tStatus sts;
  char l_fileName[512];
  dcli_translate_filename(l_fileName, fileName);
  
  if (!m_env) {
    m_env = new wb_dbms_env(l_fileName);
    m_env->open();
  }

  createDb();
  importVolume(e);
  close();
  open();

  try {
    m_env->txn_begin(0, (wb_dbms_txn **)&m_txn);

    wb_dbms_info i(this);
    i.get(m_txn);
    m_vid = i.vid();
    m_cid = i.cid();
    strcpy(m_volumeName, i.name());
    commit(&sts);
  }
  catch (wb_dbms_error &e) {
    m_txn->abort();
    printf("exeption: %s\n", e.what().c_str());
  }

}

void wb_dbms::copy(wb_export &e, wb_dbms_env *env)
{
  pwr_tStatus sts;
  
  m_env = env;
  m_env->open();

  createDb();
  importVolume(e);
  close();
  open();

  try {
    m_env->txn_begin(0, (wb_dbms_txn **)&m_txn);

    wb_dbms_info i(this);
    i.get(m_txn);
    m_vid = i.vid();
    m_cid = i.cid();
    strcpy(m_volumeName, i.name());
    commit(&sts);
  }
  catch (wb_dbms_error &e) {
    m_txn->abort();
    printf("exeption: %s\n", e.what().c_str());
  }

}

void wb_dbms::create(pwr_tVid vid, pwr_tCid cid, const char *volumeName, const char *fileName)
{
  m_vid = vid;
  m_cid = cid;
  pwr_tStatus sts;
  strcpy(m_volumeName, volumeName);
  char l_fileName[512];
  dcli_translate_filename(l_fileName, fileName);
  size_t rbSize = 0;
  pwr_tTime time;
  pwr_tOid oid;
  pwr_mClassDef flags;

  flags.m = pwr_mClassDef_System | pwr_mClassDef_TopObject;

  if (!m_env) {
    m_env = new wb_dbms_env(l_fileName);
    m_env->open();
  }
  
  if (m_env->exists())
    createDb();
  else
    return;
  
  switch (cid) {
  case pwr_eClass_RootVolume:
    rbSize = sizeof(pwr_sRootVolume);
    break;
  case pwr_eClass_SubVolume:
    rbSize = sizeof(pwr_sSubVolume);
    break;
  case pwr_eClass_SystemVolume:
    rbSize = sizeof(pwr_sSystemVolume);
    break;
  case pwr_eClass_ClassVolume:
  case pwr_eClass_DetachedClassVolume:
    rbSize = sizeof(pwr_sClassVolume);
    break;
  case pwr_eClass_WorkBenchVolume:
    rbSize = sizeof(pwr_sWorkBenchVolume);
    break;
  case pwr_eClass_DirectoryVolume:
    rbSize = sizeof(pwr_sDirectoryVolume);
    break;
  case pwr_eClass_SharedVolume:
    rbSize = sizeof(pwr_sSharedVolume);
    break;
  case pwr_eClass_CreateVolume:
  case pwr_eClass_MountVolume:
    flags.m = pwr_mClassDef_System | pwr_mClassDef_NoAdopt;
    break;
  case pwr_eClass_MountObject:
    flags.m = pwr_mClassDef_System | pwr_mClassDef_TopObject | pwr_mClassDef_NoAdopt;
    break;
  case pwr_eClass_VolatileVolume:
  case pwr_eClass_ExternVolume:
    flags.m = pwr_mClassDef_System | pwr_mClassDef_TopObject | pwr_mClassDef_DevOnly;
    break;
  case pwr_eClass_DynamicVolume:
    break;
  default: ;
  }

  oid.vid = vid;
  oid.oix = pwr_cNOix;
  wb_name n(volumeName);

  try {
    m_env->txn_begin(0, (wb_dbms_txn **)&m_txn);

    importHead(oid, cid, pwr_cNOid, pwr_cNOid, pwr_cNOid, pwr_cNOid, pwr_cNOid, n.name(), n.normName(), flags, time, time, time, rbSize, 0);

    if ( rbSize) {
      void *body = calloc( 1, rbSize);

      importRbody(oid, rbSize, body);
      free( body);
    }

    wb_dbms_info i(this);
    i.get(m_txn);
    m_vid = i.vid();
    m_cid = i.cid();
    strcpy(m_volumeName, i.name());
    commit(&sts);
  }
  catch (wb_dbms_error &e) {
    m_txn->abort();
    printf("exeption: %s\n", e.what().c_str());
  }

}

void  wb_dbms::open(const char *fileName)
{
  char l_fileName[512];
  dcli_translate_filename(l_fileName, fileName);
  
  if (!m_env) {
    m_env = new wb_dbms_env(l_fileName);
    m_env->open();
  }
  
  open();
}

void  wb_dbms::open(wb_dbms_env *env)
{
  m_env = env;
  open();
}

int wb_dbms::open()
{
  m_con = m_env->openDb();
  
  try {
    m_env->txn_begin(0, (wb_dbms_txn **)&m_txn);

    wb_dbms_info i(this);
    i.get(m_txn);
    m_vid = i.vid();
    m_cid = i.cid();
    strcpy(m_volumeName, i.name());
    return 0;
  }
  catch (wb_dbms_error &e) {
    m_txn->abort();
    printf("exeption: %s\n", e.what().c_str());
    return 1;
  }
}

int wb_dbms::createDb()
{
  int rc;
  
  m_con = m_env->createDb();
  
  rc = m_t_ohead->create("create table ohead ("
                         "  oid  bigint not null,"
                         "  head longblob not null,"
                         "  primary key (oid)"
                         ") type = innodb;");

  rc = m_t_rbody->create("create table rbody ("
                         "  oid  bigint not null,"
                         "  body longblob,"
                         "  primary key (oid)"
                         ") type = innodb;");

  rc = m_t_dbody->create("create table dbody ("
                         "  oid  bigint not null,"
                         "  body longblob,"
                         "  primary key (oid)"
                         ") type = innodb;");

  rc = m_t_class->create("create table class ("
                         "  cidoix bigint not null,"
                         "  primary key (cidoix)"
                         ") type = innodb;");

  rc = m_t_name->create("create table name ("
                        "  poid     bigint not null,"
                        "  normname tinyblob not null,"
                        "  oid      bigint not null,"
                        "  primary key (poid, normname(31))"
                        ") type = innodb;");

  rc = m_t_info->create("create table info ("
                        "  id     int not null,"
                        "  volume longblob not null,"
                        "  primary key (id)"
                        ") type = innodb;");

#if 0
  try {
    rc = m_env->txn_begin(0, &txn, 0);
  } catch (wb_dbms_error &e) {
    printf("m_env->txn_begin, %s\n", e.what().c_str());
 }
#endif
  return 0;
}


pwr_tOid wb_dbms::new_oid(wb_dbms_txn *txn)
{
  int rc = 0;
  pwr_tOid oid = pwr_cNOid;
  oid.vid = m_vid;
  wb_dbms_rbody b(this, oid);

  rc = b.get(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &oid.oix);
  if (rc)
    printf("wb_db::new_oid, b.get, rc %d\n", rc);
  oid.oix++;
  rc = b.upd(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &oid.oix);
  if (rc)
    printf("wb_dbms::new_oid, b.upd, rc %d\n", rc);

  return oid;
}

pwr_tOid wb_dbms::new_oid(wb_dbms_txn *txn, pwr_tOid oid)
{
  pwr_tOid woid;
  woid.vid = m_vid;
  woid.oix = oid.oix;

  try {
    wb_dbms_ohead o(this, txn, woid);
    return pwr_cNOid;
  } catch (wb_dbms_error &e) {
    pwr_tOix nextoix;
    int rc = 0;
    pwr_tOid oid = pwr_cNOid;
    oid.vid = m_vid;
    wb_dbms_rbody b(this, oid);

    rc = b.get(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &nextoix);
    if (rc)
      printf("wb_db::new_oid, b.get, rc %d\n", rc);
    if ( !rc && nextoix < woid.oix + 1) {
      nextoix = woid.oix + 1;
      rc = b.upd(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &nextoix);
      if (rc)
	printf("wb_dbms::new_oid, b.upd, rc %d\n", rc);
    }
    return woid;
  }
}

int wb_dbms::del_family(wb_dbms_txn *txn, wb_dbms_cursor *cp, pwr_tOid poid)
{
  int ret = 0;
#if 0
  dbName name;
  name.poid = poid;
  name.normname = "";
  pwr_tOid oid = 0;

  FamilyKey  nk(po, );
  FamiltData nd(&oid, sizeof(oid));
  wb_dbms_cursor *ncp;
  cp->dup(*ncp, 0);

  while ((ret = cp->get(&nk, &nd, DB_NEXT)) != DB_NOTFOUND) {
    del_family(txn, ncp, oid);
    cp->del();
    oh_k ok(nd);
    oh_d od();
    m_dbms_ohead->get(txn, &ok, &od, 0);
    wb_DbClistK ck(od);
    m_db_class->del(txn, &ck, 0);
    wb_DbBodyK bk(od);
    m_db_obody->del(txn, &bk, 0);
    m_db_ohead->del(txn, &ok, 0);
  }
  ncp->close();

  ret = m_db_name->del(txn, &key, 0);
#endif
  return ret;
}


/* Save all changes done in the current transaction.  */

bool wb_dbms::commit(pwr_tStatus *sts)
{
  int rc = 0;

  rc = m_txn->commit();
  if (rc)
    printf("wb_dbms::commit, rc %d\n", rc);

  m_env->txn_begin(0, (wb_dbms_txn **)&m_txn);
  if ( rc)
    *sts = LDH__DB;
  else
    *sts = LDH__SUCCESS;

  return true;
}

/* Abort the current transactionm, the database is restored to
   the state it had before the current transaction started.  */

bool wb_dbms::abort(pwr_tStatus *sts)
{
  int rc;

  rc = m_txn->abort();
  m_env->txn_begin(0, (wb_dbms_txn **)&m_txn);

  if ( rc)
    *sts = LDH__DB;
  else
    *sts = LDH__SUCCESS;
  return true;
}

wb_dbms_txn *wb_dbms::begin(wb_dbms_txn *txn) 
{
  m_env->txn_begin(0, (wb_dbms_txn **)&m_txn);
  return txn;
}

wb_dbms_txn *wb_dbms::subBegin(wb_dbms_txn *txn)
{
  txn->subBegin();
  return txn;
}

bool wb_dbms::deleteFamily(pwr_tStatus *sts, wb_dbms_ohead *o)
{
  wb_dbms_txn *txn = 0;

  m_env->txn_begin(m_txn, &txn);

  try {
    //unadopt(txn, wb_Position(o));
    //del_ohead(txn, o);
    //del_clist(txn, o);
    //del_body(txn, o);

    //txn->commit(0);
    //o->mark(is_deleted);

  }
  catch (wb_dbms_error &e) {
    txn->abort();
  }

  return true;
}

#if 0
bool wb_dbms::deleteOset(pwr_tStatus *sts, wb_oset *o)
{
  wb_dbms_txn *txn = 0;

  m_env->txn_begin(m_txn, &txn, 0);

  try {
    //del_family(txn, o);
    //unadopt(txn, wb_Position(o));
    //del_ohead(txn, o);
    //del_clist(txn, o);
    //del_name(txn, o);
    //del_body(txn, o);

    txn->commit(0);
  }
  catch (wb_dbms_error &e) {
    txn->abort();
  }

  return true;
}
#endif


bool wb_dbms::importVolume(wb_export &e)
{
  try {
    e.exportHead(*this);
    e.exportRbody(*this);
    e.exportDbody(*this);
    e.exportMeta(*this);
    return true;
  }
  catch (wb_dbms_error &e) {
    printf("exeption: %s\n", e.what().c_str());
    return false;
  }
}


bool wb_dbms::importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                           pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                           const char *name, const char *normname, pwr_mClassDef flags,
                           pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                           size_t rbSize, size_t dbSize)
{
  wb_dbms_ohead o(this, oid, cid, poid, boid, aoid, foid, loid, name, normname, flags, ohTime, rbTime, dbTime, rbSize, dbSize);
  o.ins(m_txn);
  wb_dbms_name n(this, oid, poid, normname);

  try {
    n.ins(m_txn);
  } catch (wb_dbms_error &e) {
    char newName[50];
    sprintf(newName, "O%u_%s", oid.oix, name);
    newName[31] = '\0';
    wb_name nn(newName);
    o.name(nn);
    o.upd(m_txn);
    n.name(nn);
    n.ins(m_txn);
  }

  wb_dbms_class c(this, cid, oid);
  c.ins(m_txn);
  if (oid.oix == pwr_cNOix) { // This is the volume object
    wb_dbms_info i(this);
    i.cid(cid);
    i.vid(oid.vid);
    i.name(name);
    i.ins(m_txn);
  }

  return true;
}


bool wb_dbms::importRbody(pwr_tOid oid, size_t size, void *body)
{
  wb_dbms_rbody b(this, oid, size, body);
  wb_dbms_ohead oh(this, oid);
  pwr_tTime time;
  clock_gettime(CLOCK_REALTIME, &time);
  oh.get(m_txn);
  oh.rbTime(time);
  oh.upd(m_txn);
  b.ins(m_txn);
  return true;
}

bool wb_dbms::importDbody(pwr_tOid oid, size_t size, void *body)
{
  wb_dbms_dbody b(this, oid, size, body);
  wb_dbms_ohead oh(this, oid);
  pwr_tTime time;
  clock_gettime(CLOCK_REALTIME, &time);
  oh.get(m_txn);
  oh.dbTime(time);
  oh.upd(m_txn);
  b.ins(m_txn);
  return true;
}

bool wb_dbms::importMeta(dbs_sMenv *mep)
{
  pwr_tStatus sts = 1;

  printf("Import meta, my filename: %s\n", m_env->fileName());

  if (mep != 0)
    dbs_Split(&sts, mep, m_env->fileName());

  return true;
}

int wb_dbms::initTables()
{
  m_t_class = new wb_dbms_table(this);
  m_t_class->queryGet(new wb_dbms_get_class(this));
  m_t_class->querySucc(new wb_dbms_succ_class(this));
  m_t_class->queryPred(new wb_dbms_pred_class(this));
  m_t_class->queryIns(new wb_dbms_ins_class(this));
  m_t_class->queryUpd(0);
  m_t_class->queryDel(new wb_dbms_del_class(this));
  m_t_class->queryCursor(new wb_dbms_cursor_class(this));

  m_t_dbody = new wb_dbms_table(this);
  m_t_dbody->queryGet(new wb_dbms_get_dbody(this));
  m_t_dbody->querySucc(0);
  m_t_dbody->queryPred(0);
  m_t_dbody->queryIns(new wb_dbms_ins_dbody(this));
  m_t_dbody->queryUpd(new wb_dbms_upd_dbody(this));
  m_t_dbody->queryDel(new wb_dbms_del_dbody(this));
  m_t_dbody->queryCursor(new wb_dbms_cursor_dbody(this));

  m_t_info = new wb_dbms_table(this);
  m_t_info->queryGet(new wb_dbms_get_info(this));
  m_t_info->querySucc(0);
  m_t_info->queryPred(0);
  m_t_info->queryIns(new wb_dbms_ins_info(this));
  m_t_info->queryUpd(new wb_dbms_upd_info(this));
  m_t_info->queryDel(new wb_dbms_del_info(this));
  m_t_info->queryCursor(NULL);

  m_t_ohead = new wb_dbms_table(this);
  m_t_ohead->queryGet(new wb_dbms_get_ohead(this));
  m_t_ohead->querySucc(0);
  m_t_ohead->queryPred(0);
  m_t_ohead->queryIns(new wb_dbms_ins_ohead(this));
  m_t_ohead->queryUpd(new wb_dbms_upd_ohead(this));
  m_t_ohead->queryDel(new wb_dbms_del_ohead(this));
  m_t_ohead->queryCursor(new wb_dbms_cursor_ohead(this));

  m_t_rbody = new wb_dbms_table(this);
  m_t_rbody->queryGet(new wb_dbms_get_rbody(this));
  m_t_rbody->querySucc(0);
  m_t_rbody->queryPred(0);
  m_t_rbody->queryIns(new wb_dbms_ins_rbody(this));
  m_t_rbody->queryUpd(new wb_dbms_upd_rbody(this));
  m_t_rbody->queryDel(new wb_dbms_del_rbody(this));
  m_t_rbody->queryCursor(new wb_dbms_cursor_rbody(this));

  m_t_name = new wb_dbms_table(this);
  m_t_name->queryGet(new wb_dbms_get_name(this));
  m_t_name->querySucc(0);
  m_t_name->queryPred(0);
  m_t_name->queryIns(new wb_dbms_ins_name(this));
  m_t_name->queryUpd(0);
  m_t_name->queryDel(new wb_dbms_del_name(this));
  m_t_name->queryCursor(new wb_dbms_cursor_name(this));

  return 0;
}

	
wb_dbms_env::wb_dbms_env(const char *v_fileName) : 
  m_con(0), m_fileName(0), m_host(0), m_user(0), m_passwd(0), m_dbName(0), m_port(0), m_socket(0), m_exists(false)
{
  fileName(v_fileName);
}

wb_dbms_env::wb_dbms_env(const char *v_host, const char *v_user, const char *v_passwd,
                             const char *v_dbName, unsigned int v_port, const char *v_socket) :
  m_con(0), m_fileName(0), m_host(0), m_user(0), m_passwd(0), m_dbName(0), m_port(0), m_socket(0), m_exists(false)
{
  host(v_host);
  user(v_user);
  passwd(v_passwd);
  dbName(v_dbName);
  port(v_port);
  socket(v_socket);
}

void wb_dbms_env::host(const char *host)
{
  if (!host)
    return;
  
  m_host = (char *)realloc(m_host, strlen(host));
  strcpy(m_host, host);
}

void wb_dbms_env::user(const char *user)
{
  if (!user)
    return;
  
  m_user = (char *)realloc(m_user, strlen(user));
  strcpy(m_user, user);
}

void wb_dbms_env::passwd(const char *passwd)
{
  if (!passwd)
    return;
  
  m_passwd = (char *)realloc(m_passwd, strlen(passwd));
  strcpy(m_passwd, passwd);
}

void wb_dbms_env::dbName(const char *dbName)
{
  if (!dbName)
    return;
  
  m_dbName = (char *)realloc(m_dbName, strlen(dbName));
  strcpy(m_dbName, dbName);
}

void wb_dbms_env::fileName(const char *fileName)
{
  if (!fileName)
    return;
  
  m_fileName = (char *)realloc(m_fileName, strlen(fileName));
  strcpy(m_fileName, fileName);
}

char *wb_dbms_env::dbName(void) 
{ 
  char pname[80];
  pwr_tStatus sts;
  static char dbname[80];

  sts = utl_get_projectname( pname);
  if ( EVEN(sts)) {
    printf( "** Project is not configured\n");
    return m_dbName;
  }

  strcpy( dbname, "pwrp_");
  strcat( dbname, pname);
  strcat( dbname, "__");
  strcat( dbname, m_dbName);
  cdh_ToLower( dbname, dbname);

  return dbname;
}

char *wb_dbms_env::host(void) 
{ 
  char nodename[80];
  pwr_tStatus sts;
  static char host[80];

  syi_NodeName( &sts, nodename, sizeof(nodename));
  if ( ODD(sts) && cdh_NoCaseStrcmp( nodename, m_host) == 0)
    strcpy( host, "localhost");
  else
    strcpy( host, m_host);

  return host;
}

void wb_dbms_env::port(const unsigned int port)
{
  m_port = port;
}

void wb_dbms_env::socket(const char *socket)
{
  if (!socket)
    return;
  
  m_socket = (char *)realloc(m_socket, strlen(socket));
  strcpy(m_socket, socket);
}

wb_dbms_env::wb_dbms_env() :
  m_con(0), m_fileName(0), m_host(0), m_user(0), m_passwd(0), m_dbName(0), m_port(0), m_socket(0), m_exists(false)
{

};

int wb_dbms_env::close()
{

  return 0;
}

int wb_dbms_env::open(const char *v_host, const char *v_user, const char *v_passwd,
                        const char *v_dbName, unsigned int v_port, const char *v_socket)
{

  host(v_host);
  user(v_user);
  passwd(v_passwd);
  dbName(v_dbName);
  port(v_port);
  socket(v_socket);

  m_con = mysql_init(NULL);
    
  MYSQL *con = mysql_real_connect(m_con, host(), user(), passwd(), dbName(), port(), socket(), 0);
  if (con == 0) {
    printf("Failed to connect to database: Error: %s\n", mysql_error(m_con));
    return 1;
  }

  char sql[255];
      
  sprintf(sql, "use %s", dbName());
  int rc = mysql_query(m_con, sql);
  if (rc) {
    printf("%s\n", mysql_error(m_con));
    printf("%s\n", sql);
    return rc;
  } else {
    printf("database open %s\n", sql);
  }  

  return 0;
}

int wb_dbms_env::create(const char *v_fileName, const char *v_host, const char *v_user,
                          const char *v_passwd, const char *v_dbName, unsigned int v_port,
                          const char *v_socket)
{
  fileName(v_fileName);
  host(v_host);
  user(v_user);
  passwd(v_passwd);
  dbName(v_dbName);
  port(v_port);
  socket(v_socket);
  
  create();
  
  return 0;
}

MYSQL *wb_dbms_env::createDb(void)
{
  m_con = mysql_init(NULL);

  MYSQL *con = mysql_real_connect(m_con, host(), user(), passwd(), 0, port(), socket(), 0);
  // printf("Tried to connect to database, con %x: Status: %s\n", (int)con, mysql_error(m_con));
  if (con == 0) {
    printf("Failed to connect to database: Error: %s\n", mysql_error(m_con));
    return 0;
  }

  char sql[255];
      
  sprintf(sql, "create database %s", dbName());
  int rc = mysql_query(m_con, sql);
  if (rc) {
    printf("%s\n", mysql_error(m_con));
    printf("%s\n", sql);
    return 0;
  }      
      
  sprintf(sql, "use %s", dbName());
  rc = mysql_query(m_con, sql);
  if (rc) {
    printf("%s\n", mysql_error(m_con));
    printf("%s\n", sql);
    return 0;
  }      

  return con;
}

MYSQL *wb_dbms_env::openDb()
{
  m_con = mysql_init(NULL);

  MYSQL *con = mysql_real_connect(m_con, host(), user(), passwd(), dbName(), port(), socket(), 0);
  // printf("Tried to connect to database, con %x: Status: %s\n", (int)con, mysql_error(m_con));
  if (con == 0) {
    printf("Failed to connect to database: Error: %s\n", mysql_error(m_con));
    return 0;
  }

  return con;
}

int wb_dbms_env::create()
{
  struct stat sb;
  char name[512];

  cdh_ToLower(m_fileName, m_fileName);

  printf("wb_dbms_env::create: %s\n", m_fileName);
  /* Create the directory, read/write/access owner and group. */
  if (stat(m_fileName, &sb) != 0) {
    if (mkdir(m_fileName, S_IRWXU | S_IRWXG) != 0) {
      fprintf(stderr, "wb_dbms_env::create: mkdir: %s, %s\n", m_fileName, strerror(errno));
      return errno;
    }
  }

  sprintf(name, "%s/%s", m_fileName, "connection.dmsql");

  if (stat(name, &sb) != 0) {
    FILE *fp;    
    
    fp = fopen(name, "w+b");
    if (fp == NULL) {
      printf("** Cannot open file: %s, %s\n", name, strerror(errno));
      return errno;
    }

    fprintf(fp, "HOST...: %s\n", m_host);
    fprintf(fp, "USER...: %s\n", user());
    if ( !passwd() || strcmp( passwd(), "") == 0)
      fprintf(fp, "PASSWD.: (null)\n");
    else
      fprintf(fp, "PASSWD.: %s\n", passwd());
    fprintf(fp, "DB_NAME: %s\n", m_dbName);
    fprintf(fp, "PORT...: %d\n", port());
    fprintf(fp, "SOCKET.: %s\n", socket());

    fclose(fp);
  }

  return 0;
}

int wb_dbms_env::open(const char* v_fileName)
{
  fileName(v_fileName);
  
  return open();
}

int wb_dbms_env::open(void)
{
  char var[32];
  char value[32];
  char buf[512];
  char *s;
  char *valp;
  int rc;
  
  cdh_ToLower(m_fileName, m_fileName);

  sprintf(buf, "%s/%s", m_fileName, "connection.dmsql");

  FILE *fp = fopen(buf, "r");
  if (fp == NULL) {
    printf("** Cannot open file: %s, %s\n", buf, strerror(errno));
    return errno;
  }

  while ((s = fgets(buf, sizeof(buf) - 1, fp))) {
    if (*s == '#')
      continue;
    
    rc = sscanf(s, " %[A-Z_] %*[^ ] %s", var, value);
    if (rc < 1)
      continue;
    if (rc == 1)
      valp = 0;
    else
      valp = value;
    
    if (strcmp(valp, "(null)") == 0)
      valp = 0;
    
    if (strcmp(var, "HOST") == 0) {
      host(valp);
    }
    else if (strcmp(var, "USER") == 0) {
      user(valp);
    }
    else if (strcmp(var, "PASSWD") == 0) {
      passwd(valp);
    }
    else if (strcmp(var, "DB_NAME") == 0) {
      dbName(valp);
    }
    else if (strcmp(var, "PORT") == 0) {
      if (valp == 0)
        port(0);
      else
        port(atoi(valp));
    }
    else if (strcmp(var, "SOCKET") == 0) {
      socket(valp);
    }
    else {
      printf("Unknown connection parameter! : %s\n", var);
    }
  }
  
  fclose(fp);

  m_exists = true;
  
  return 0;
}

int wb_dbms_env::txn_begin(wb_dbms_txn *pid, wb_dbms_txn **tid)
{
  int rc = mysql_rollback(m_con);
  if (rc)
    printf("wb_dbms::abort, rc %d\n", rc);

  rc = mysql_autocommit(m_con, 0);
  if (rc)
    printf("mysql_autocommit(m_con, 0) : %d\n", rc);

  if (tid && !*tid) {
    *tid = new wb_dbms_txn(this);
  }

  return rc;
}


wb_dbms_info::wb_dbms_info(wb_dbms *db) :
  m_db(db), m_data(&m_volume, sizeof(m_volume))
{
}

void wb_dbms_info::get(wb_dbms_txn *txn)
{
  int index = 1;
  int ret;
  wb_dbms_qe key(&index, sizeof(index));
  wb_dbms_qe data(&m_volume, sizeof(m_volume));

  try {
    ret = m_db->m_t_info->get(txn, &key, &data);
    // printf("info get: %d\n", ret);
  } catch (wb_dbms_error &e) {
    printf("info get Error, %d\n", ret);
    std::cout << e.what().c_str();
  }
}

void wb_dbms_info::ins(wb_dbms_txn *txn)
{
  int index = 1;
  int ret;

  m_key.data(&index);
  m_key.size(sizeof(index));
  m_key.bSize(sizeof(index));

  ret = m_db->m_t_info->ins(txn, &m_key, &m_data);
  printf("info ins: %d\n", ret);
}

void wb_dbms_info::upd(wb_dbms_txn *txn)
{
  int index = 1;
  int ret;

  m_key.data(&index);
  m_key.size(sizeof(index));
  m_key.bSize(sizeof(index));

  ret = m_db->m_t_info->upd(txn, &m_key, &m_data);
  printf("info ins: %d\n", ret);
}

wb_dbms_class::wb_dbms_class(wb_dbms *db) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_dbc(0)
{
}

wb_dbms_class::wb_dbms_class(wb_dbms *db, wb_dbms_txn *txn, pwr_tCid cid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_dbc(0)
{
  m_k.oix = pwr_cNOix;
  m_k.cid = cid;
}

wb_dbms_class::wb_dbms_class(wb_dbms *db, pwr_tCid cid, pwr_tOid oid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_dbc(0)
{
  m_k.oix = oid.oix;
  m_k.cid = cid;
}

wb_dbms_class::wb_dbms_class(wb_dbms *db, wb_dbms_ohead &o) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_dbc(0)
{
  m_k.oix = o.oid().oix;
  m_k.cid = o.cid();
}

bool wb_dbms_class::succ(pwr_tOid oid)
{
  m_k.oix = oid.oix;

  int rc = m_db->m_t_class->succ(m_db->m_txn, &m_key, 0);

  return rc == 0;
}

bool wb_dbms_class::succClass(pwr_tCid cid)
{
  m_k.cid = cid + 1;
  m_k.oix = pwr_cNOix;

  int rc = m_db->m_t_class->succ(m_db->m_txn, &m_key, 0);
  
  return rc == 0;
}

bool wb_dbms_class::pred(pwr_tOid oid)
{
  m_k.oix = oid.oix;

  int rc = m_db->m_t_class->pred(m_db->m_txn, &m_key, 0);

  return rc == 0;
}

int wb_dbms_class::ins(wb_dbms_txn *txn)
{
  return m_db->m_t_class->ins(txn, &m_key, 0);
}

int wb_dbms_class::upd(wb_dbms_txn *txn)
{
  return m_db->m_t_class->upd(txn, &m_key, 0);
}

int wb_dbms_class::del(wb_dbms_txn *txn)
{
  return m_db->m_t_class->del(txn, &m_key, 0);
}

void wb_dbms_class::iter(void (*func)(pwr_tOid oid, pwr_tCid cid))
{
  
  m_db->m_t_class->cursor(m_db->m_txn, &m_key, 0, &m_dbc);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_k, 0, sizeof(m_k));

  m_key.data(&m_k);
  m_key.size(sizeof(m_k));
  m_key.bSize(sizeof(m_k));

  /* Walk through the database. */

  while (m_dbc->get() == 0) {
    pwr_tOid oid;
    oid.oix = m_k.oix;
    oid.vid = m_db->vid();
    
    func(oid, m_k.cid);
  }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

wb_dbms_class::~wb_dbms_class()
{  
  if (m_dbc)
    m_dbc->close();
}

wb_dbms_class_iterator::wb_dbms_class_iterator(wb_dbms *db) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_atEnd(false), m_rc(0)
{
  memset(&m_k, 0, sizeof(m_k));
  m_atEnd = (m_rc != 0);
}

wb_dbms_class_iterator::wb_dbms_class_iterator(wb_dbms *db, pwr_tCid cid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_atEnd(false), m_rc(0)
{
  memset(&m_k, 0, sizeof(m_k));
  m_atEnd = (m_rc != 0);  
  m_k.oix = pwr_cNOix;
  m_k.cid = cid;
}

wb_dbms_class_iterator::wb_dbms_class_iterator(wb_dbms *db, pwr_tCid cid, pwr_tOid oid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_atEnd(false), m_rc(0)
{
  memset(&m_k, 0, sizeof(m_k));
  m_atEnd = (m_rc != 0);  
  m_k.oix = oid.oix;
  m_k.cid = cid;
}

bool wb_dbms_class_iterator::first()
{
  m_k.oix = pwr_cNOix;
  m_k.cid = pwr_cNCid;

  m_rc = m_db->m_t_class->succ(m_db->m_txn, &m_key, 0);
  m_atEnd = (m_rc != 0);

  return !m_atEnd;
}

bool wb_dbms_class_iterator::succObject()
{
  
  if (!m_atEnd) {
    m_rc = m_db->m_t_class->succ(m_db->m_txn, &m_key, 0);
    m_atEnd = (m_rc != 0);
  }  
 
  return !m_atEnd;
}

bool wb_dbms_class_iterator::succClass()
{
  m_k.oix = pwr_cNOix;
  m_k.cid++;

  m_rc = m_db->m_t_class->succ(m_db->m_txn, &m_key, 0);
  m_atEnd = (m_rc != 0);

  return !m_atEnd;
}

bool wb_dbms_class_iterator::succClass(pwr_tCid cid)
{
  m_k.oix = pwr_cNOix;
  m_k.cid = cid + 1;

  m_rc = m_db->m_t_class->succ(m_db->m_txn, &m_key, 0);
  m_atEnd = (m_rc != 0);

  return !m_atEnd;
}

wb_dbms_class_iterator::~wb_dbms_class_iterator()
{
}

wb_dbms_name::wb_dbms_name(wb_dbms *db, wb_dbms_txn *txn) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  m_k.poid = pwr_cNOid;
}

wb_dbms_name::wb_dbms_name(wb_dbms *db, wb_dbms_ohead &o) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  m_k.poid = o.poid();
  strcpy(m_k.normname, o.normname());
  m_d.oid = o.oid();
}

wb_dbms_name::wb_dbms_name(wb_dbms *db, pwr_tOid oid, pwr_tOid poid, const char *name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  wb_name n(name);

  memset(&m_k, 0, sizeof(m_k));

  m_k.poid = poid;
  strcpy(m_k.normname, n.normName(cdh_mName_object));
  m_d.oid = oid;
}

wb_dbms_name::wb_dbms_name(wb_dbms *db, wb_dbms_txn *txn, pwr_tOid poid, wb_name &name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  m_k.poid = poid;
  strcpy(m_k.normname, name.normName(cdh_mName_object));
  m_d.oid = pwr_cNOid;
}

wb_dbms_name::wb_dbms_name(wb_dbms *db, pwr_tOid poid, const char *name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  memset(&m_d, 0, sizeof(m_d));

  m_k.poid = poid;
  strcpy(m_k.normname, name);
}

int wb_dbms_name::get(wb_dbms_txn *txn)
{
  m_data.bSize(sizeof(m_d));
  int rc = m_db->m_t_name->get(txn, &m_key, &m_data);
  return rc;
}

int wb_dbms_name::ins(wb_dbms_txn *txn)
{
  return m_db->m_t_name->ins(txn, &m_key, &m_data);
}

int wb_dbms_name::upd(wb_dbms_txn *txn)
{
  return m_db->m_t_name->upd(txn, &m_key, &m_data);
}

int wb_dbms_name::del(wb_dbms_txn *txn)
{
  return m_db->m_t_name->del(txn, &m_key, 0);
}

void wb_dbms_name::name(wb_name &name)
{
  memset(m_k.normname, 0, sizeof(m_k.normname));
  strcpy(m_k.normname, name.normName(cdh_mName_object));
}

void wb_dbms_name::iter(void (*func)(pwr_tOid poid, pwr_tObjName name, pwr_tOid oid))
{
  int rc = 0;

  m_db->m_t_name->cursor(m_db->m_txn, &m_key, &m_data, &m_dbc);

  memset(&m_k, 0, sizeof(m_k));
  memset(&m_d, 0, sizeof(m_d));

  /* Walk through the database and print out the key/data pairs. */

  while ((rc = m_dbc->get()) == 0) {
    func(m_k.poid, m_k.normname, m_d.oid);
  }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

wb_dbms_ohead::wb_dbms_ohead() :
  m_db(0), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
}

wb_dbms_ohead::wb_dbms_ohead(wb_dbms *db) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
}

wb_dbms_ohead::wb_dbms_ohead(wb_dbms *db, pwr_tOid oid) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
  m_oid = m_o.oid = oid;
}

wb_dbms_ohead::wb_dbms_ohead(wb_dbms *db, wb_dbms_txn *txn, pwr_tOid oid) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
  m_oid = m_o.oid = oid;
  get(txn);
}

wb_dbms_ohead::wb_dbms_ohead(wb_dbms *db, pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                         pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                         const char *name, const char *normname, pwr_mClassDef flags,
                         pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                         size_t rbSize, size_t dbSize) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{

  memset(&m_o, 0, sizeof(m_o));
  m_oid = m_o.oid = oid;
  m_o.cid = cid;
  m_o.poid = poid;
  strcpy(m_o.name, name);
  strcpy(m_o.normname, normname);
  m_o.time = ohTime;
  m_o.boid = boid;
  m_o.aoid = aoid;
  m_o.foid = foid;
  m_o.loid =loid;

  m_o.flags = flags;

  m_o.body[0].time = rbTime;
  m_o.body[0].size = rbSize;
  m_o.body[1].time = dbTime;
  m_o.body[1].size = dbSize;
}


wb_dbms_ohead &wb_dbms_ohead::get(wb_dbms_txn *txn)
{
  int rc = 0;
  m_data.size(sizeof(m_o));
  m_data.bSize(sizeof(m_o));

  rc = m_db->m_t_ohead->get(txn, &m_key, &m_data);
  if (rc)
    throw wb_dbms_error(m_db, "wb_dbms_ohead::get(txn, &m_key, &m_data)");
  return *this;
}

int wb_dbms_ohead::ins(wb_dbms_txn *txn)
{
  return m_db->m_t_ohead->ins(txn, &m_key, &m_data);
}

int wb_dbms_ohead::upd(wb_dbms_txn *txn)
{
  return m_db->m_t_ohead->upd(txn, &m_key, &m_data);
}

wb_dbms_ohead &wb_dbms_ohead::get(wb_dbms_txn *txn, pwr_tOid oid)
{
  int rc = 0;
  m_oid = oid;

  rc = m_db->m_t_ohead->get(txn, &m_key, &m_data);
  if ( rc /*////== DB_NOTFOUND*/)
    throw wb_dbms_error(m_db, "wb_dbms_ohead::get(wb_dbms_txn *txn, pwr_tOid oid)");
  if (rc)
    printf("wb_db_ohead::get(txn, oid = %d.%d), get, rc %d\n", oid.vid, oid.oix, rc);

  if (oid.oix != m_o.oid.oix)
    printf("oid.oix (%d.%d) != m_o.oid.oix (%d.%d), %s\n", oid.vid, oid.oix, m_o.oid.vid, m_o.oid.oix, m_o.name);
  return *this;
}

int wb_dbms_ohead::del(wb_dbms_txn *txn)
{
  return m_db->m_t_ohead->del(txn, &m_key, 0);
}

void wb_dbms_ohead::name(wb_name &name)
{
  memset(m_o.name, 0, sizeof(m_o.name));
  memset(m_o.normname, 0, sizeof(m_o.normname));
  strcpy(m_o.name, name.name(cdh_mName_object));
  strcpy(m_o.normname, name.normName(cdh_mName_object));
}

void wb_dbms_ohead::name(pwr_tOid &oid)
{
  memset(m_o.name, 0, sizeof(m_o.name));
  memset(m_o.normname, 0, sizeof(m_o.normname));
  sprintf(m_o.name, "O%u", oid.oix);
  strcpy(m_o.normname, m_o.name);
}

void wb_dbms_ohead::clear()
{
  memset(&m_o, 0, sizeof(m_o));
}

void wb_dbms_ohead::iter(void (*func)(pwr_tOid oid, dbms_sObject *op))
{
  int rc = 0;

  rc = m_db->m_t_ohead->cursor(m_db->m_txn, &m_key, &m_data, &m_dbc);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_oid, 0, sizeof(m_oid));
  memset(&m_o, 0, sizeof(m_o));

  /* Walk through the database and print out the key/data pairs. */

  while ((rc = m_dbc->get()) == 0) {
    func(m_oid, &m_o);
  }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

void wb_dbms_ohead::iter(wb_import &i)
{
  int rc = 0;

  rc = m_db->m_t_ohead->cursor(m_db->m_txn, &m_key, &m_data, &m_dbc);

  memset(&m_oid, 0, sizeof(m_oid));
  memset(&m_o, 0, sizeof(m_o));

  while ((rc = m_dbc->get()) == 0) {
    i.importHead(m_o.oid, m_o.cid, m_o.poid, m_o.boid, m_o.aoid, m_o.foid, m_o.loid, m_o.name, m_o.normname,
                 m_o.flags, m_o.time, m_o.body[0].time, m_o.body[1].time, m_o.body[0].size, m_o.body[1].size);
  }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

wb_dbms_rbody::wb_dbms_rbody(wb_dbms *db, pwr_tOid oid, size_t size, void *p) :
  m_db(db), m_oid(oid), m_size(size), m_p(p), m_key(&m_oid, sizeof(m_oid)), m_data(p, size)
{
}

wb_dbms_rbody::wb_dbms_rbody(wb_dbms *db) :
  m_db(db), m_oid(pwr_cNOid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

wb_dbms_rbody::wb_dbms_rbody(wb_dbms *db, pwr_tOid oid) :
  m_db(db), m_oid(oid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

int wb_dbms_rbody::ins(wb_dbms_txn *txn)
{
  return m_db->m_t_rbody->ins(txn, &m_key, &m_data);
}

int wb_dbms_rbody::upd(wb_dbms_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.size(sizeof(m_db->m_buf));
  m_data.bSize(sizeof(m_db->m_buf));
  m_data.data(m_db->m_buf);


  int rc = m_db->m_t_rbody->get(txn, &m_key, &m_data);
  if (rc)
    return rc;

  if (m_data.size() < size + offset)
    printf("*** rbody::upd(offset %d, size %d, oix %d), size: %d\n", offset, size, m_oid.oix, m_data.size());

  memcpy(m_db->m_buf + offset, p, size);  
  
  return m_db->m_t_rbody->upd(txn, &m_key, &m_data);
}

int wb_dbms_rbody::get(wb_dbms_txn *txn, size_t offset, size_t size, void *p)
{

  m_data.bSize(sizeof(m_db->m_buf));
  m_data.size(sizeof(m_db->m_buf));
  m_data.data(m_db->m_buf);

  int rc = m_db->m_t_rbody->get(txn, &m_key, &m_data);
  if (rc)
    return rc;
  
  assert(sizeof(m_db->m_buf) >= size + offset);
  if (m_data.size() < size + offset)
    printf("*** rbody::get(offset %d, size %d, oix %d), size: %d\n", offset, size, m_oid.oix, m_data.size());
  
  
  memcpy(p, m_db->m_buf + offset, size);

  return 0;
}

int wb_dbms_rbody::del(wb_dbms_txn *txn)
{
  return m_db->m_t_rbody->del(txn, &m_key, 0);
}

void wb_dbms_rbody::iter(void (*print)(pwr_tOid oid, size_t size))
{
  int rc = 0;

  memset(&m_oid, 0, sizeof(m_oid));

  m_data.data(m_db->m_buf);
  m_data.size(sizeof(m_db->m_buf));
  m_data.bSize(sizeof(m_db->m_buf));

  m_db->m_t_rbody->cursor(m_db->m_txn, &m_key, &m_data, &m_dbc);

  /* Walk through the database and print out the key/data pairs. */

  while (1) {
    try {
      rc = m_dbc->get();
    }
    catch (wb_dbms_error &e) {
      printf("Exc: %s\n", e.what().c_str());
    }

    if (rc != 0)
      break;

    print(m_oid, m_data.size());
  }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

void wb_dbms_rbody::iter(wb_import &i)
{
  int rc = 0;

  memset(&m_oid, 0, sizeof(m_oid));

  m_data.data(m_db->m_buf);
  m_data.size(sizeof(m_db->m_buf));
  m_data.bSize(sizeof(m_db->m_buf));

  m_db->m_t_rbody->cursor(m_db->m_txn, &m_key, &m_data, &m_dbc);

  while (1) {
    try {
      rc = m_dbc->get();
    }
    catch (wb_dbms_error &e) {
      printf("Exc: %s\n", e.what().c_str());
    }

    if (rc != 0)
      break;

    i.importRbody(m_oid, m_data.size(), m_data.data());
  }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

wb_dbms_dbody::wb_dbms_dbody(wb_dbms *db, pwr_tOid oid, size_t size, void *p) :
  m_db(db), m_oid(oid), m_size(size), m_p(p), m_key(&m_oid, sizeof(m_oid)), m_data(p, size)
{
}

wb_dbms_dbody::wb_dbms_dbody(wb_dbms *db) :
  m_db(db), m_oid(pwr_cNOid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

wb_dbms_dbody::wb_dbms_dbody(wb_dbms *db, pwr_tOid oid) :
  m_db(db), m_oid(oid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

int wb_dbms_dbody::ins(wb_dbms_txn *txn)
{
  return m_db->m_t_dbody->ins(txn, &m_key, &m_data);
}

int wb_dbms_dbody::upd(wb_dbms_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.size(sizeof(m_db->m_buf));
  m_data.bSize(sizeof(m_db->m_buf));
  m_data.data(m_db->m_buf);

  int rc = m_db->m_t_dbody->get(txn, &m_key, &m_data);
  if (rc)
    return rc;
  if (m_data.size() < size + offset)
    printf("*** dbody::upd(offset %d, size %d, oix %d), size: %d\n", offset, size, m_oid.oix, m_data.size());

  memcpy(m_db->m_buf + offset, p, size);  
  
  return m_db->m_t_dbody->upd(txn, &m_key, &m_data);
}

int wb_dbms_dbody::get(wb_dbms_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.size(sizeof(m_db->m_buf));
  m_data.bSize(sizeof(m_db->m_buf));
  m_data.data(m_db->m_buf);

//  printf("--- dbody::get(offset %d, size %d, oix %d)\n", offset, size, m_oid.oix);
  int rc = m_db->m_t_dbody->get(txn, &m_key, &m_data);
  if (rc)
    return rc;
  
  assert(sizeof(m_db->m_buf) >= size + offset);
  if (m_data.size() < size + offset)
    printf("*** dbody::get(offset %d, size %d, oix %d), size: %d\n", offset, size, m_oid.oix, m_data.size());
  
  memcpy(p, m_db->m_buf + offset, size);

  return 0;
}

int wb_dbms_dbody::del(wb_dbms_txn *txn)
{
  return m_db->m_t_dbody->del(txn, &m_key, 0);
}

void wb_dbms_dbody::iter(void (*print)(pwr_tOid oid, size_t size))
{
  int rc = 0;

  memset(&m_oid, 0, sizeof(m_oid));

  m_data.data(m_db->m_buf);
  m_data.size(sizeof(m_db->m_buf));
  m_data.bSize(sizeof(m_db->m_buf));

  m_db->m_t_dbody->cursor(m_db->m_txn, &m_key, &m_data, &m_dbc);

  /* Walk through the database and print out the key/data pairs. */

  while (1) {

    try {
      rc = m_dbc->get();
    }
    catch (wb_dbms_error &e) {
      printf("Exc: %s\n", e.what().c_str());
    }

    if (rc != 0)
      break;

    print(m_oid, m_data.size());
 }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

void wb_dbms_dbody::iter(wb_import &i)
{
  int rc = 0;

  memset(&m_oid, 0, sizeof(m_oid));

  m_data.data(m_db->m_buf);
  m_data.size(sizeof(m_db->m_buf));
  m_data.bSize(sizeof(m_db->m_buf));

  m_db->m_t_dbody->cursor(m_db->m_txn, &m_key, &m_data, &m_dbc);

  /* Walk through the database and print out the key/data pairs. */

  while (1) {

    try {
      rc = m_dbc->get();
    }
    catch (wb_dbms_error &e) {
      printf("Exc: %s\n", e.what().c_str());
    }

    if (rc != 0)
      break;

    i.importDbody(m_oid, m_data.size(), m_data.data());
 }

  m_dbc->close();
  delete m_dbc;
  m_dbc = 0;
}

int wb_dbms_txn::commit()
{
  int rc;
  
  rc = mysql_commit(m_env->con());
  if (rc)
    printf("wb_dbms::commit, rc %d\n", rc);

  return rc;
}

int wb_dbms_txn::abort()
{
  int rc = 0;

  rc = mysql_rollback(m_env->con());
  if (rc)
    printf("wb_dbms::abort, rc %d\n", rc);

  return rc;
}

int wb_dbms_txn::subBegin()
{
  int rc = mysql_query(m_env->con(), "savepoint sub");
  if (rc) goto error;
  return rc;

error:
  printf("%s rc %d, %s\n", "wb_dbms::subBegin()", rc, mysql_error(m_env->con()));
  //throw wb_error(DB__TXNERROR);
  return 1;
}

int wb_dbms_txn::subAbort()
{
  int rc = mysql_query(m_env->con(), "rollback to savepoint sub");
  if (rc) goto error;
  return rc;

error:
  printf("%s rc %d, %s\n", "wb_dbms::subAbort()", rc, mysql_error(m_env->con()));
  //throw wb_error(DB__TXNERROR);
  return 1;
}

int wb_dbms_query::prepare(const char *query, int nResult, int nParam)
{
  int rc = 0;
  static const char *method = "prepare";
  
  if (!m_prepared) {

    m_query = query;
    
    m_stmt = mysql_stmt_init(m_db->con());
    if (m_stmt == 0) error(method, "mysql_stmt_init");
    
    rc = mysql_stmt_prepare(m_stmt, query, strlen(query));
    if (rc) error(rc, method, "mysql_stmt_prepare");
   
    m_nParam  = mysql_stmt_param_count(m_stmt);
    m_nResult = mysql_stmt_field_count(m_stmt);
    
    assert((m_nParam == nParam) && (m_nResult == nResult));
  
    if (m_nParam > 0) {
      m_param.bind    = (MYSQL_BIND *)calloc(m_nParam,  sizeof(MYSQL_BIND));
      m_param.length  = (size_t *)calloc(m_nParam,  sizeof(size_t));
      m_param.is_null = (my_bool *)calloc(m_nParam,  sizeof(my_bool));

      for (int i = m_nParam - 1; i >= 0; i--) {
        m_param.bind[i].is_null = &m_param.is_null[i];
        m_param.bind[i].length  = (long unsigned int *)&m_param.length[i];
      }
    }
    
    if (m_nResult > 0) {
      m_result.bind    = (MYSQL_BIND *)calloc(m_nResult, sizeof(MYSQL_BIND));
      m_result.length  = (size_t *)calloc(m_nParam,  sizeof(size_t));
      m_result.is_null = (my_bool *)calloc(m_nResult, sizeof(my_bool));

      for (int i = m_nResult - 1; i >= 0; i--) {
        m_result.bind[i].is_null = &m_result.is_null[i];
        m_result.bind[i].length  = (long unsigned int *)&m_result.length[i];
      }
    }
    
    m_prepared = true;
  }

  return rc;
}

int wb_dbms_query::bindQuery(void)
{
  int rc = 0;
  static const char *method = "bindQuery";  

  if (m_nParam > 0) {
    rc = mysql_stmt_bind_param(m_stmt, m_param.bind);
    if (rc) error(rc, method, "mysql_stmt_bind_param");
  }
    
  if (m_nResult > 0) {
    rc = mysql_stmt_bind_result(m_stmt, m_result.bind);
    if (rc) error(rc, method, "mysql_stmt_bind_result");
  }

  return rc;
}

void wb_dbms_query::bindParam(int index, enum enum_field_types type, char *var)
{
  m_param.bind[index].buffer_type = type;
  m_param.bind[index].buffer      = var;
}

void wb_dbms_query::bindParam(int index, enum enum_field_types type, char *var, size_t size)
{
  m_param.bind[index].buffer_type   = type;
  m_param.bind[index].buffer        = var;
  m_param.bind[index].buffer_length = size;
  m_param.length[index]             = size;
}

void wb_dbms_query::bindParam(int index, enum enum_field_types type, char *var, size_t size, size_t bsize)
{
  m_param.bind[index].buffer_type   = type;
  m_param.bind[index].buffer        = var;
  m_param.bind[index].buffer_length = size;
  m_param.length[index]             = bsize;
}

void wb_dbms_query::bindParam(int index, enum enum_field_types type, wb_dbms_qe *par)
{
  m_param.bind[index].buffer_type   = type;
  m_param.bind[index].buffer        = par->data();
  m_param.bind[index].buffer_length = par->bSize();
  m_param.bind[index].length        = (long unsigned int *)par->a_size();
  m_param.bind[index].is_null       = par->a_isNull();
}

void wb_dbms_query::bindResult(int index, enum enum_field_types type, char *var)
{
  m_result.bind[index].buffer_type = type;
  m_result.bind[index].buffer      = var;
}

void wb_dbms_query::bindResult(int index, enum enum_field_types type, char *var, size_t size)
{
  m_result.bind[index].buffer_type   = type;
  m_result.bind[index].buffer        = var;
  m_result.bind[index].buffer_length = size;
  m_result.length[index]             = size;
}

void wb_dbms_query::bindResult(int index, enum enum_field_types type, char *var, size_t size, size_t bsize)
{
  m_result.bind[index].buffer_type   = type;
  m_result.bind[index].buffer        = var;
  m_result.bind[index].buffer_length = size;
  m_result.length[index]             = bsize;
}

void wb_dbms_query::bindResult(int index, enum enum_field_types type, wb_dbms_qe *par)
{
  m_result.bind[index].buffer_type   = type;
  m_result.bind[index].buffer        = par->data();
  m_result.bind[index].buffer_length = par->bSize();
  m_result.bind[index].length        = (long unsigned int *)par->a_size();
  m_result.bind[index].is_null       = par->a_isNull();
}

void wb_dbms_query::error(int rc, const char *method, const char *func)
{
  char s[200];
  
  if (m_stmt) mysql_stmt_close(m_stmt);
  m_stmt = 0;
  m_prepared = false;

  sprintf(s, "*** query \"%s\" %s %s %d %s\n", m_query, method, func, rc, mysql_error(m_db->con()));
  throw wb_dbms_error(m_db, s);
}

void wb_dbms_query::error(const char *method, const char *func)
{
  char s[200];
  
  if (m_stmt) mysql_stmt_close(m_stmt);
  m_stmt = 0;
  m_prepared = false;

  sprintf(s, "*** query \"%s\" %s %s %s\n", m_query, method, func,  mysql_error(m_db->con()));
  throw wb_dbms_error(m_db, s);
}

int wb_dbms_table::create(const char *query)
{
  int rc = 0;
    
  rc = mysql_query(m_db->con(), query);
  if (rc) {
    printf("%s\n", mysql_error(m_db->con()));
    printf("%s\n", query);
  }
  return rc;
}

int wb_dbms_table::close()
{
  return 0;
}

int wb_dbms_table::cursor(wb_dbms_txn *txn, wb_dbms_qe *key, wb_dbms_qe *data, wb_dbms_cursor **cp)
{
  int rc = m_q_cursor->execute(txn, key, data);

  if (rc != 0)
    return rc;
    
  *cp = new wb_dbms_cursor(m_q_cursor);

  return 0;
}

wb_dbms_error::wb_dbms_error(wb_dbms *db, string str) : m_error_str(str)
{
  m_errno = errno;
  m_db = db;
  if (db)
    m_sql_error = mysql_error(m_db->con());
} 

string wb_dbms_error::what() const
{
  string s;
  s = m_error_str + ", " + m_sql_error;
  return s;
}
#endif
