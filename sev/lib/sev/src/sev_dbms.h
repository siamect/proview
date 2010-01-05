/* 
 * Proview   $Id: sev_dbms.h,v 1.4 2008-10-31 12:51:30 claes Exp $
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
 */

#ifndef sev_dbms_h
#define sev_dbms_h
#if defined PWRE_CONF_MYSQL

#include <vector>

#include "pwr.h"
#include "pwr_class.h"
#include "sev_db.h"
#include <mysql/mysql.h>

using namespace std;

class sev_dbms_env;


class sev_dbms_env
{
 public:
  MYSQL *m_con;

  char *m_fileName;
  static char m_systemName[40];
  
  char *m_host;
  char *m_user;
  char *m_passwd;
  char *m_dbName;
  unsigned int m_port;
  char *m_socket;
  bool m_exists;
  

  sev_dbms_env();
  sev_dbms_env(const char *fileName);
  sev_dbms_env(const char *host, const char *user, const char *passwd,
                const char *dbName, unsigned int port, const char *socket);

  sev_dbms_env(const sev_dbms_env &);
  void operator = (const sev_dbms_env &);
  virtual ~	sev_dbms_env() { close();}

  int create(const char *fileName, const char *host, const char *user, const char *passwd,
             const char *dbName, unsigned int port, const char *socket);  
  
  int open(void);
  int open(const char *fileName);
  int open(const char *host, const char *user, const char *passwd,
	   const char *dbName, unsigned int port, const char *socket);

  int checkAndUpdateVersion(unsigned int version);
  int updateDB_to_SevVersion2(void);
  MYSQL *createDb(void);
  MYSQL *openDb(void);
  bool exists() { return m_exists;}    
  int close(void);
  static int get_systemname();

  void fileName(const char *fileName);
  void host(const char *host);
  void user(const char *user);
  void passwd(const char *passwd);
  void dbName(const char *dbName);
  void port(unsigned int port);  
  void socket(const char *socket);
  
  static char *dbName(void);
  inline char *fileName(void) { return m_fileName;}
  char *host(void);
  inline char *user(void) { return m_user;}
  inline char *passwd(void) { return m_passwd;}
  inline unsigned int port(void) { return m_port;}
  inline char *socket(void) { return m_socket;}  
  

  inline MYSQL *con(void) {
    return m_con;
  }
  
 private:
  int create();
};

class sev_dbms : public sev_db {
 public:

  sev_dbms_env *m_env;

  sev_dbms( sev_dbms_env *env) : m_env(env) {}
  ~sev_dbms();

  int check_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
		  pwr_tDeltaTime storagetime, pwr_eType type, unsigned int size, 
		  char *description, char *unit, pwr_tFloat32 scantime, 
		  pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx);
  int add_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
		pwr_tDeltaTime storagetime, pwr_eType type, unsigned int size, 
		char *description, char *unit, pwr_tFloat32 scantime, 
		pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx);  
  int store_value( pwr_tStatus *sts, int item_idx, int attr_idx,
		   pwr_tTime time, void *buf, unsigned int size);
  int get_values( pwr_tStatus *sts, pwr_tOid oid, pwr_tMask options, float deadband, char *aname, 
		  pwr_eType type, unsigned int size, pwr_tFloat32 scantime, pwr_tTime *creatime,
		  pwr_tTime *starttime, 
		  pwr_tTime *endtime, int maxsize, pwr_tTime **tbuf, void **vbuf, unsigned int *bsize);
  int delete_old_data( pwr_tStatus *sts, pwr_tOid oid, char *aname, 
		       pwr_tMask options, pwr_tTime limit);
  int delete_item( pwr_tStatus *sts, pwr_tOid oid, char *aname);

  int get_items( pwr_tStatus *sts);
  int create_table( pwr_tStatus *sts, pwr_tOid oid, char *aname, pwr_eType type, unsigned int size,
		    pwr_tMask options, float deadband);
  int delete_table( pwr_tStatus *sts, pwr_tOid oid, char *aname);
  int store_item( pwr_tStatus *sts, char *tabelname, pwr_tOid oid, char *oname, char *aname, 
		  pwr_tDeltaTime storagetime, pwr_eType vtype, unsigned int vsize, 
		  char *description, char *unit, pwr_tFloat32 scantime, 
		  pwr_tFloat32 deadband, pwr_tMask options);
  int remove_item( pwr_tStatus *sts, pwr_tOid oid, char *aname);
  char *oid_to_table( pwr_tOid oid, char *aname);
  char *pwrtype_to_type( pwr_eType type, unsigned int size);
  static int timestr_to_time( char *tstr, pwr_tTime *ts);
  int check_objectitem( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, char *aname, 
                         pwr_tDeltaTime storagetime,
                         char *description, pwr_tFloat32 scantime, 
                         pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx);
  int add_objectitem( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, char *aname, 
                       pwr_tDeltaTime storagetime, 
                       char *description, pwr_tFloat32 scantime, 
                       pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx);  
  int store_objectitem( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, char *aname, 
      pwr_tDeltaTime storagetime, char *description, pwr_tFloat32 scantime, 
      pwr_tFloat32 deadband, pwr_tMask options);
  int create_objecttable( pwr_tStatus *sts, char *tablename, pwr_tMask options, float deadband);
  int store_objectvalue( pwr_tStatus *sts, int item_idx, int attr_idx,
                             pwr_tTime time, void *buf,  void *oldbuf, unsigned int size);
  int get_item( pwr_tStatus *sts, sev_item *item, char *tablename);
  int get_objectitem( pwr_tStatus *sts, sev_item *item, char *tablename);
  int get_objectitems( pwr_tStatus *sts);
  int get_objectitemattributes( pwr_tStatus *sts, sev_item *item, char *tablename);
  int check_objectitemattr( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *aname, char *oname, 
																	pwr_eType type, unsigned int size, unsigned int *idx);
  int delete_old_objectdata( pwr_tStatus *sts, char *tablename, 
                             pwr_tMask options, pwr_tTime limit);
  int check_deadband(pwr_eType type, unsigned int size, pwr_tFloat32 deadband, void *value, void *oldvalue);
  int get_objectvalues( pwr_tStatus *sts, sev_item *item, unsigned int size, pwr_tTime *starttime, pwr_tTime *endtime, 
			                  int maxsize, pwr_tTime **tbuf, void **vbuf, unsigned int *bsize);
  pwr_tUInt64 get_minFromIntegerColumn( char *tablename, char *colname );
  pwr_tUInt64 get_maxFromIntegerColumn( char *tablename, char *colname );
  int handle_itemchange(pwr_tStatus *sts, char *tablename, unsigned int item_idx);
  int handle_objectchange(pwr_tStatus *sts, char *tablename, unsigned int item_idx, bool newObject);
};
#endif
#endif
