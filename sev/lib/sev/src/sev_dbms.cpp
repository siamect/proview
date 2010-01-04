/* 
 * Proview   $Id: sev_dbms.cpp,v 1.4 2008-11-10 07:57:59 claes Exp $
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

#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_syi.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_load.h"
#include "sev_dbms.h"
#include "rt_sev_msg.h"
#include "rt_errh.h"

using namespace std;
char sev_dbms_env::m_systemName[40];

sev_dbms_env::sev_dbms_env(const char *v_fileName) : 
  m_con(0), m_fileName(0), m_host(0), m_user(0), m_passwd(0), m_dbName(0), m_port(0), m_socket(0), m_exists(false)
{
  fileName(v_fileName);
  strcpy( m_systemName, "");

  get_systemname();
}

sev_dbms_env::sev_dbms_env(const char *v_host, const char *v_user, const char *v_passwd,
			   const char *v_dbName, unsigned int v_port, const char *v_socket) :
  m_con(0), m_fileName(0), m_host(0), m_user(0), m_passwd(0), m_dbName(0), m_port(0), 
  m_socket(0), m_exists(false)
{
  strcpy( m_systemName, "");
  host(v_host);
  user(v_user);
  passwd(v_passwd);
  dbName(v_dbName);
  port(v_port);
  socket(v_socket);
  get_systemname();
}

void sev_dbms_env::host(const char *host)
{
  if (!host)
    return;
  
  m_host = (char *)realloc(m_host, strlen(host)+1);
  strcpy(m_host, host);
}

void sev_dbms_env::user(const char *user)
{
  if (!user)
    return;
  
  m_user = (char *)realloc(m_user, strlen(user)+1);
  strcpy(m_user, user);
}

void sev_dbms_env::passwd(const char *passwd)
{
  if (!passwd)
    return;
  
  m_passwd = (char *)realloc(m_passwd, strlen(passwd));
  strcpy(m_passwd, passwd);
}

void sev_dbms_env::dbName(const char *dbName)
{
  if (!dbName)
    return;
  
  m_dbName = (char *)realloc(m_dbName, strlen(dbName)+1);
  strcpy(m_dbName, dbName);
}

void sev_dbms_env::fileName(const char *fileName)
{
  if (!fileName)
    return;
  
  m_fileName = (char *)realloc(m_fileName, strlen(fileName)+1);
  strcpy(m_fileName, fileName);
}

char *sev_dbms_env::dbName(void) 
{ 
  static char dbname[80];

  get_systemname();
  strcpy( dbname, "pwrp__");
  strcat( dbname, m_systemName);
  cdh_ToLower( dbname, dbname);

  return dbname;
}

char *sev_dbms_env::host(void) 
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

void sev_dbms_env::port(const unsigned int port)
{
  m_port = port;
}

void sev_dbms_env::socket(const char *socket)
{
  if (!socket)
    return;
  
  m_socket = (char *)realloc(m_socket, strlen(socket)+1);
  strcpy(m_socket, socket);
}

sev_dbms_env::sev_dbms_env() :
  m_con(0), m_fileName(0), m_host(0), m_user(0), m_passwd(0), m_dbName(0), m_port(0), m_socket(0), m_exists(false)
{

};

int sev_dbms_env::close()
{

  return 0;
}

int sev_dbms_env::open(const char *v_host, const char *v_user, const char *v_passwd,
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

int sev_dbms_env::create(const char *v_fileName, const char *v_host, const char *v_user,
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

MYSQL *sev_dbms_env::createDb(void)
{
  m_con = mysql_init(NULL);

  MYSQL *con = mysql_real_connect(m_con, host(), user(), passwd(), 0, port(), socket(), 0);
  // printf("Tried to connect to database, con %x: Status: %s\n", (int)con, mysql_error(m_con));
  if (con == 0) {
    printf("Failed to connect to database: Error: %s\n", mysql_error(m_con));
    return 0;
  }

  char query[400];
      
  sprintf(query, "create database %s", dbName());
  int rc = mysql_query(m_con, query);
  if (rc) {
    printf("%s\n", mysql_error(m_con));
    printf("%s\n", query);
    return 0;
  }      
      
  sprintf(query, "use %s", dbName());
  rc = mysql_query(m_con, query);
  if (rc) {
    printf("%s\n", mysql_error(m_con));
    printf("%s\n", query);
    return 0;
  }      

  sprintf( query, "create table items ("
	   "id int unsigned not null primary key auto_increment,"
	   "tablename varchar(255),"
	   "vid int unsigned,"
	   "oix int unsigned,"
	   "oname varchar(255),"
	   "aname varchar(255),"
	   "uptime datetime,"
	   "cretime datetime,"
	   "storagetime int unsigned,"
	   "deadband float,"
	   "options int unsigned,"
	   "scantime float,"
	   "description varchar(80),"
	   "vtype int unsigned,"
	   "vsize int unsigned,"
	   "unit varchar(16));");

  rc = mysql_query( m_con, query);
  if (rc) printf( "Create items table: %s\n", mysql_error(m_con));

  updateDB_to_SevVersion2();

  return con;
}

int sev_dbms_env::checkAndUpdateVersion(unsigned int version)
{
  unsigned int old_version=1; //Proview 4.6.0.0 was first release with sev
  int rc = mysql_query( m_con, "select * from sev_version");
  if (rc) {
    printf( "table sev_version do no exist\n");
  }
  else {
  
    MYSQL_ROW row;
    MYSQL_RES *result = mysql_store_result( m_con);
  
    if ( !result) {
      printf( "selecting sev_version Error\n");
      return 0;
    }
  
    row = mysql_fetch_row( result);
    if (!row) {
      printf( "selecting sev_version Error\n");
      return 0;
    }
  
    old_version= strtoul( row[0], 0, 10);
    mysql_free_result( result);
  }

  printf("old sev_version: %d, new sev_version: %d\n", old_version, version);

  //add code for new versions here
  if(old_version < 2 ) {
    printf("Updating database tables to sev version 2\n");
    updateDB_to_SevVersion2();
  }

  if(old_version != version) {
    char query[100];
    sprintf( query, "update sev_version set version = %d", version);
    rc = mysql_query( m_con, query);
    if (rc) printf( "Update sev_version: %s\n", mysql_error(m_con));
  }
  return 1;
}

int sev_dbms_env::updateDB_to_SevVersion2(void)
{
  char query[400];

  sprintf( query, "create table sev_version ("
     "version int unsigned not null primary key);");
  int rc = mysql_query( m_con, query);
  if (rc) printf( "Create sev_version table: %s\n", mysql_error(m_con));

  sprintf( query, "insert into sev_version (version) values(2)");
  rc = mysql_query( m_con, query);
  if (rc) printf( "Insert into table sev_version: %s\n", mysql_error(m_con));


	sprintf( query, "create table objectitems ("
		 "id int unsigned not null primary key auto_increment,"
		 "tablename varchar(255),"
		 "vid int unsigned,"
		 "oix int unsigned,"
		 "oname varchar(255),"
		 "aname varchar(255),"
		 "uptime datetime,"
		 "cretime datetime,"
		 "storagetime int unsigned,"
		 "deadband float,"
		 "options int unsigned,"
		 "scantime float,"
		 "description varchar(80));");

	rc = mysql_query( m_con, query);
	if (rc) printf( "Create objectitems table: %s\n", mysql_error(m_con));

  sprintf( query, "create table objectitemattributes ("
     "tablename varchar(255) not null,"
     "attributename varchar(255) not null,"
     "attributeidx int unsigned not null,"
     "attributetype int unsigned not null,"
     "attributesize int unsigned not null,"
     "PRIMARY KEY(tablename, attributename));");

  rc = mysql_query( m_con, query);
  if (rc) printf( "Create objectitemattributes table: %s\n", mysql_error(m_con));

  return 1;
}


MYSQL *sev_dbms_env::openDb()
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

int sev_dbms_env::create()
{
  struct stat sb;
  char name[512];

  cdh_ToLower(m_fileName, m_fileName);

  printf("sev_dbms_env::create: %s\n", m_fileName);
  /* Create the directory, read/write/access owner and group. */
  if (stat(m_fileName, &sb) != 0) {
    if (mkdir(m_fileName, S_IRWXU | S_IRWXG) != 0) {
      fprintf(stderr, "sev_dbms_env::create: mkdir: %s, %s\n", m_fileName, strerror(errno));
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

int sev_dbms_env::open(const char* v_fileName)
{
  fileName(v_fileName);
  
  return open();
}

int sev_dbms_env::open(void)
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

int sev_dbms_env::get_systemname()
{
  FILE 	*file;
  pwr_tFileName	fname;
  char  nodename[40];
  char	*bus_str;
  int bus;
  char line[200];
  pwr_tStatus sts;

  if ( strcmp( m_systemName, "") != 0)
    return 1;

  syi_NodeName( &sts, nodename, sizeof(nodename));
  if ( EVEN(sts)) return 0;

  bus_str = getenv( "PWR_BUS_ID");
  if ( !bus_str)
    return 0;
  if ( sscanf( bus_str, "%d", &bus) != 1)
    return 0;
  
  sprintf( fname, load_cNameBoot, load_cDirectory, nodename, bus);
  dcli_translate_filename( fname, fname);
  file = fopen( fname, "r");
  if ( file == 0) {
    printf("** Warning, systemname not found\n");
    return 0;
  }

  if ( !dcli_read_line( line, sizeof(line), file))
    return 0;
  if ( !dcli_read_line( line, sizeof(line), file))
    return 0;

  strcpy( m_systemName, line);
  return 1;
}

int sev_dbms::create_table( pwr_tStatus *sts, pwr_tOid oid, char *aname, pwr_eType type, 
			    unsigned int size, pwr_tMask options, float deadband)
{
  char query[200];
  char timeformatstr[80];
  char jumpstr[80];
  char idtypestr[20];
  char readoptstr[80];

  if ( options & pwr_mSevOptionsMask_PosixTime) {
    if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
      // Posix time, high resolution
      strcpy( timeformatstr, "time int unsigned, ntime int unsigned");
      strcpy( idtypestr, "bigint");
    }
    else {
      // Posix time, low resolution
      strcpy( timeformatstr, "time int unsigned");
      strcpy( idtypestr, "int");
    }
  }
  else {
    if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
      // Sql time, high resolution
      strcpy( timeformatstr, "time datetime not null, ntime int unsigned");
      strcpy( idtypestr, "bigint");
    }
    else {
      // Sql time, low resolution
      strcpy( timeformatstr, "time datetime not null");
      strcpy( idtypestr, "int");
    }
  }

  if ( options & pwr_mSevOptionsMask_ReadOptimized)
    sprintf( readoptstr, "id %s unsigned not null primary key auto_increment,", idtypestr);
  else
    strcpy( readoptstr, "");

  if ( options & pwr_mSevOptionsMask_UseDeadBand)
    strcpy( jumpstr, ",jump tinyint unsigned");
  else
    strcpy( jumpstr, "");

  sprintf( query, "create table %s ( %s"
	   "%s, value %s %s, index (time));",
	   oid_to_table(oid, aname), readoptstr, timeformatstr, pwrtype_to_type( type, size), jumpstr);

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Create table: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
  return 1;
}

int sev_dbms::delete_table( pwr_tStatus *sts, pwr_tOid oid, char *aname)
{
  char query[200];

  sprintf( query, "drop table %s;", 
	   oid_to_table(oid, aname));

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Delete table: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
  return 1;
}

int sev_dbms::store_item( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, 
			  char *aname, pwr_tDeltaTime storagetime, pwr_eType vtype, 
			  unsigned int vsize, char *description, char *unit, pwr_tFloat32 scantime,
			  pwr_tFloat32 deadband, pwr_tMask options)
{
  char query[800];
  char timestr[40];
  pwr_tTime creatime;

  time_GetTime( &creatime);
  time_AtoAscii( &creatime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  sprintf( query, "insert into items (id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,vtype,vsize,description,unit,scantime,deadband,options) "
	   "values (0,'%s',%d,%d,'%s','%s','%s','%s',%ld,%d,%d,'%s','%s',%f,%f,%d);",
	   tablename, oid.vid, oid.oix, oname, aname, timestr, timestr, (long int)storagetime.tv_sec, vtype, 
	   vsize, description, unit, scantime, deadband, options);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Store item: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::remove_item( pwr_tStatus *sts, pwr_tOid oid, char *aname)
{
  char query[800];

  sprintf( query, "delete from items where vid = %u and oix = %u and aname = '%s';",
	   oid.vid, oid.oix, aname);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Remove item: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::get_items( pwr_tStatus *sts)
{
  m_items.clear();
  char query[300];

  sprintf( query, "select id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,vtype,vsize,description,unit,scantime,deadband,options "
	   "from items");

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Get Items: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetValues Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  int rows = mysql_num_rows( result);

  for ( int i = 0; i < rows; i++) {
    sev_item item;
    sev_attr attr;
    item.attr.push_back(attr);
    row = mysql_fetch_row( result);
    if (!row) break;

    //printf( "%d %s %s\n", i, row[0], row[1]);

    item.id = atoi( row[0]);
    strncpy( item.tablename, row[1], sizeof(item.tablename));
    item.oid.vid = strtoul( row[2], 0, 10);
    item.oid.oix = strtoul( row[3], 0, 10);
    strncpy( item.oname, row[4], sizeof(item.oname));
    strncpy( item.attr[0].aname, row[5], sizeof(item.attr[0].aname));
    timestr_to_time( row[6], &item.modtime);
    timestr_to_time( row[7], &item.creatime);
    item.storagetime.tv_sec = strtoul( row[8], 0, 10);
    item.storagetime.tv_nsec = 0;
    item.attr[0].type = (pwr_eType) strtoul( row[9], 0, 10);
    item.attr[0].size = strtoul( row[10], 0, 10);
    strncpy( item.description, row[11], sizeof(item.description));
    strncpy( item.attr[0].unit, row[12], sizeof(item.attr[0].unit));
    item.scantime = atof(row[13]);
    item.deadband = atof(row[14]);
    item.options = strtoul(row[15], 0, 10);

    item.attrnum = 1;

    m_items.push_back( item);
  }
  mysql_free_result( result);
  
  //for ( int i = 0; i < (int)m_items.size(); i++)
  //  printf( "Item: %d %s\n", m_items[i].id, m_items[i].tablename);

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::store_value( pwr_tStatus *sts, int item_idx, int attr_idx,
                           pwr_tTime time, void *buf, unsigned int size)
{
  if(size != m_items[item_idx].value_size) {
    //Something is seriously wrong
    printf("%s expected size:%d recevied size:%d ERROR!!\n", __FUNCTION__, m_items[item_idx].value_size, size);
    *sts = SEV__DBERROR;
    return 0;
  }
  if(m_items[item_idx].attrnum > 1) {
    return store_objectvalue(sts, item_idx, attr_idx, time, buf, m_items[item_idx].old_value, size);
  }
  char query[200];
  char bufstr[512];
  char timstr[40];
  int update_time_only = 0;
  int set_jump = 0;

  if ( !m_items[item_idx].first_storage) {
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand) {
      if ( m_items[item_idx].deadband_active) {
        // Compare current value to old value
        switch ( m_items[item_idx].attr[attr_idx].type) {
          case pwr_eType_Float32:
            if ( fabsf ( *(pwr_tFloat32 *)buf - *(pwr_tFloat32 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tFloat32 *)m_items[item_idx].old_value = *(pwr_tFloat32 *)buf;
            }
            break;
          case pwr_eType_Float64:
            if ( fabsf ( *(pwr_tFloat64 *)buf - *(pwr_tFloat64 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tFloat64 *)m_items[item_idx].old_value = *(pwr_tFloat64 *)buf;
            }
            break;
          case pwr_eType_Int64:
            if ( fabsf ( *(pwr_tInt64 *)buf - *(pwr_tInt64 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tInt64 *)m_items[item_idx].old_value = *(pwr_tInt64 *)buf;
            }
            break;
          case pwr_eType_Int32:
            if ( fabsf ( *(pwr_tInt32 *)buf - *(pwr_tInt32 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tInt32 *)m_items[item_idx].old_value = *(pwr_tInt32 *)buf;
            }
            break;
          case pwr_eType_Int16:
            if ( fabsf ( *(pwr_tInt16 *)buf - *(pwr_tInt16 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tInt16 *)m_items[item_idx].old_value = *(pwr_tInt16 *)buf;
            }
            break;
          case pwr_eType_Int8:
          case pwr_eType_Char:
            if ( fabsf ( *(pwr_tInt8 *)buf - *(pwr_tInt8 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tInt8 *)m_items[item_idx].old_value = *(pwr_tInt8 *)buf;
            }
            break;
          case pwr_eType_UInt64:
            if ( fabsf ( *(pwr_tUInt64 *)buf - *(pwr_tUInt64 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tUInt64 *)m_items[item_idx].old_value = *(pwr_tUInt64 *)buf;
            }
            break;
          case pwr_eType_UInt32:
          case pwr_eType_Boolean:
            if ( fabsf ( *(pwr_tUInt32 *)buf - *(pwr_tUInt32 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tUInt32 *)m_items[item_idx].old_value = *(pwr_tUInt32 *)buf;
            }
            break;
          case pwr_eType_UInt16:
            if ( fabsf ( *(pwr_tUInt16 *)buf - *(pwr_tUInt16 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tUInt16 *)m_items[item_idx].old_value = *(pwr_tUInt16 *)buf;
            }
            break;
          case pwr_eType_UInt8:
            if ( fabsf ( *(pwr_tUInt8 *)buf - *(pwr_tUInt8 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              update_time_only = 1;
            }
            else {
              m_items[item_idx].deadband_active = 0;
              *(pwr_tUInt8 *)m_items[item_idx].old_value = *(pwr_tUInt8 *)buf;
            }
            break;
          default: ;
        }
      }
      else {
        // Compare current value to old value
        switch ( m_items[item_idx].attr[attr_idx].type) {
          case pwr_eType_Float32:
            if ( fabsf ( *(pwr_tFloat32 *)buf - *(pwr_tFloat32 *)m_items[item_idx].old_value) < m_items[item_idx].deadband) {
              m_items[item_idx].deadband_active = 1;
              set_jump = 1;
            }
            *(pwr_tFloat32 *)m_items[item_idx].old_value = *(pwr_tFloat32 *)buf;
            break;
          default: ;
        }
      }
    }
    if ( set_jump && (m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)) {
      sprintf( query, "update %s set jump = 1 where id = %d",
               m_items[item_idx].tablename, 
               m_items[item_idx].last_id);
      int rc = mysql_query( m_env->con(), query);
      if (rc)
        printf( "Update jump: %s\n", mysql_error(m_env->con()));
    }
  }
  else {
    m_items[item_idx].first_storage = 0;
    memcpy(m_items[item_idx].old_value, buf, size);
  }



  *sts = time_AtoAscii( &time, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
  if ( EVEN(*sts)) return 0;
  timstr[19] = 0;
  *sts = cdh_AttrValueToString( m_items[item_idx].attr[attr_idx].type, buf, bufstr, sizeof(bufstr));
  if ( EVEN(*sts)) return 0;

  char colname[255];
  strcpy(colname, "value");
  if ( !update_time_only) {
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_PosixTime) {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Posix time, high resolution
        switch ( m_items[item_idx].attr[attr_idx].type) {
          case pwr_eType_String:
            sprintf( query, "insert into %s (time, ntime, %s) values (%ld,%ld,'%s')",
                     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname,
                     (long int)time.tv_sec, (long int)time.tv_nsec, bufstr);
            break;
          default:
            if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
              sprintf( query, "insert into %s (time, ntime, %s, jump) values (%ld,%ld,%s,%d)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname,
                       (long int)time.tv_sec, (long int)time.tv_nsec, bufstr, set_jump);
            else
              sprintf( query, "insert into %s (time, ntime, %s) values (%ld,%ld,%s)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname,
                       (long int)time.tv_sec, (long int)time.tv_nsec, bufstr);
        }
      }
      else {
        // Posix time, low resolution
        switch ( m_items[item_idx].attr[attr_idx].type) {
          case pwr_eType_String:
            sprintf( query, "insert into %s (time, %s) values (%ld,'%s')",
                     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, (long int)time.tv_sec, bufstr);
            break;
          default:
            if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
              sprintf( query, "insert into %s (time, %s, jump) values (%ld,%s,%d)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, (long int)time.tv_sec, bufstr, set_jump);
            else
              sprintf( query, "insert into %s (time, %s) values (%ld,%s)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, (long int)time.tv_sec, bufstr);
        }
      }
    }
    else {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Sql time, high resolution
        switch ( m_items[item_idx].attr[attr_idx].type) {
          case pwr_eType_String:
            sprintf( query, "insert into %s (time, ntime, %s) values ('%s',%ld,'%s')",
                     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, 
                     timstr, (long int)time.tv_nsec, bufstr);
            break;
          default:
            if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
              sprintf( query, "insert into %s (time, ntime, %s, jump) values ('%s',%ld,%s,%d)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, 
                       timstr, (long int)time.tv_nsec, bufstr, set_jump);
            else
              sprintf( query, "insert into %s (time, ntime, %s) values ('%s',%ld,%s)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, 
                       timstr, (long int)time.tv_nsec, bufstr);
        }
      }
      else {
        // Sql time, low resolution
        switch ( m_items[item_idx].attr[attr_idx].type) {
          case pwr_eType_String:
            sprintf( query, "insert into %s (time, %s) values ('%s','%s')",
                     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, timstr, bufstr);
            break;
          default:
            if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
              sprintf( query, "insert into %s (time, %s, jump) values ('%s',%s,%d)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, timstr, bufstr, set_jump);
            else
              sprintf( query, "insert into %s (time, %s) values ('%s',%s)",
                       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), colname, timstr, bufstr);
        }
      }
    }
    int rc = mysql_query( m_env->con(), query);
    if (rc) {
      // printf( "Store value: %s \"%s\"\n", mysql_error(m_env->con()), query);
      *sts = SEV__DBERROR;
      m_items[item_idx].status = *sts;
      if ( m_items[item_idx].status != m_items[item_idx].logged_status) {
        m_items[item_idx].logged_status = m_items[item_idx].status;
        errh_Error( "Database store error: %s, table: %s object: %s", 
                    mysql_error(m_env->con()),  m_items[item_idx].tablename, m_items[item_idx].oname);
      }
      return 0;
    }
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_ReadOptimized)
      m_items[item_idx].last_id = mysql_insert_id( m_env->con());
  }
  else {
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_PosixTime) {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Posix time, high resolution
        sprintf( query, "update %s set time = %ld, ntime = %ld where id = %d",
                 oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
                 (long int)time.tv_sec, (long int)time.tv_nsec, m_items[item_idx].last_id);
      }
      else {
        // Posix time, low resolution
        sprintf( query, "update %s set time = %ld where id = %d",
                 oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), (long int)time.tv_sec, 
                 m_items[item_idx].last_id);
      }
    }
    else {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Sql time, high resolution
        sprintf( query, "update %s set time = '%s', ntime = %ld where id = %d",
                 oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
                 timstr, (long int)time.tv_nsec, m_items[item_idx].last_id);
      }
      else {
        // Sql time, low resolution
        sprintf( query, "update %s set time = '%s' where id = %d",
                 oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), timstr, 
                 m_items[item_idx].last_id);
      }
    }

    int rc = mysql_query( m_env->con(), query);
    if (rc) {
      // printf( "Update value: %s\n", mysql_error(m_env->con()));
      *sts = SEV__DBERROR;
      m_items[item_idx].status = *sts;
      if ( m_items[item_idx].status != m_items[item_idx].logged_status) {
        m_items[item_idx].logged_status = m_items[item_idx].status;
        errh_Error( "Database update error: %s, table: %s object: %s", 
                    mysql_error(m_env->con()), m_items[item_idx].tablename, m_items[item_idx].oname);
      }
      return 0;
    }
  }

  *sts = SEV__SUCCESS;
  m_items[item_idx].status = *sts;
  m_items[item_idx].logged_status = 1;
  return 1;
}

int sev_dbms::get_values( pwr_tStatus *sts, pwr_tOid oid, pwr_tMask options, float deadband, 
			  char *aname, pwr_eType type, 
			  unsigned int size, pwr_tFloat32 scantime, pwr_tTime *creatime, 
			  pwr_tTime *starttime, pwr_tTime *endtime, 
			  int maxsize, pwr_tTime **tbuf, void **vbuf, unsigned int *bsize)
{
  char query[200];
  char starttimstr[40];
  char endtimstr[40];
  int total_rows;
  int div;
  pwr_tDeltaTime dt;
  pwr_tTime stime, etime;
  char column_part[80];
  char orderby_part[80];
  char jumpstr[40];
  char where_part[200];

  if ( starttime && starttime->tv_sec == 0 && starttime->tv_nsec == 0)
    starttime = 0;
  else
    stime = *starttime;

  if ( endtime && endtime->tv_sec == 0 && endtime->tv_nsec == 0)
    endtime = 0;
  else
    etime = *endtime;

  // Get number of rows
  sprintf( query, "show table status where name = '%s';", oid_to_table(oid, aname));

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "GetValues Query Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }

  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());

  if ( !result) {
    printf( "GetValues Status Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }

  row = mysql_fetch_row( result);
  if (!row) {
    printf( "GetValues Status Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  if ( starttime && endtime) {
    pwr_tTime create_time;
    pwr_tTime update_time;
    timestr_to_time( row[11], &create_time);
    timestr_to_time( row[12], &update_time);

    if ( time_Acomp( creatime, &stime) == 1)
      stime = *creatime;

    if ( time_Acomp( &etime, &update_time) == 1)
      etime = update_time;

    time_Adiff( &dt, &etime, &stime);
    total_rows = int (time_DToFloat( 0, &dt) / scantime);

    div = total_rows / maxsize + 1;
  }
  else if ( starttime) {
    pwr_tTime update_time;
    timestr_to_time( row[12], &update_time);

    if ( time_Acomp( &update_time, starttime) != 1) {
      mysql_free_result( result);
      *sts = SEV__NODATATIME;
      return 0;
    }
    time_Adiff( &dt, &update_time, starttime);
    total_rows = int (time_DToFloat( 0, &dt) / scantime);
  }
  else if ( endtime) {
    pwr_tTime create_time;
    timestr_to_time( row[11], &create_time);

    if ( time_Acomp( endtime, &create_time) != 1) {
      mysql_free_result( result);
      *sts = SEV__NODATATIME;
      return 0;
    }
    time_Adiff( &dt, endtime, &create_time);
    total_rows = int (time_DToFloat( 0, &dt) / scantime);
  }
  else {
    total_rows = atoi(row[4]);
  }
  mysql_free_result( result);

  div = total_rows / maxsize + 1;

  if ( starttime) {
    *sts = time_AtoAscii( &stime, time_eFormat_NumDateAndTime, starttimstr,
                          sizeof(starttimstr));
    if ( EVEN(*sts)) return 0;
    starttimstr[19] = 0;
  }
  if ( endtime) {
    *sts = time_AtoAscii( &etime, time_eFormat_NumDateAndTime, endtimstr, 
                          sizeof(endtimstr));
    if ( EVEN(*sts)) return 0;
    endtimstr[19] = 0;
  }

  // Column part 
  if ( options & pwr_mSevOptionsMask_HighTimeResolution)
    strcpy( column_part, "time,ntime,value");
  else
    strcpy( column_part, "time,value");

  if ( options & pwr_mSevOptionsMask_UseDeadBand)
    strcpy( jumpstr, "or jump = 1");
  else
    strcpy( jumpstr, "");

  // 'order by' part
  if ( options & pwr_mSevOptionsMask_ReadOptimized)
    strcpy( orderby_part, "id");
  else {
    if ( options & pwr_mSevOptionsMask_HighTimeResolution)
      strcpy( orderby_part, "time,ntime");
    else
      strcpy( orderby_part, "time");
  }

  // 'where' part
  if ( options & pwr_mSevOptionsMask_ReadOptimized) {
    if ( starttime && endtime) {
      if ( div == 1) {
        if ( options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where time >= %ld and time <= %ld", (long int)starttime->tv_sec, (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where time >= '%s' and time <= '%s'", starttimstr, endtimstr);
      }
      else {
        if ( options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where (id %% %d = 0 %s) and time >= %ld and time <= %ld", 
                   div, jumpstr, (long int)starttime->tv_sec, (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where (id %% %d = 0 %s) and time >= '%s' and time <= '%s'", 
                   div, jumpstr, starttimstr, endtimstr);
      }
    }
    else if ( starttime) {
      if ( div == 1) {
        if ( options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where time >= %ld", (long int)starttime->tv_sec);
        else
          sprintf( where_part, "where time >= '%s'", starttimstr);
      }
      else {
        if ( options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where (id %% %d = 0 %s) and time >= %ld", div, jumpstr, (long int)starttime->tv_sec);
        else
          sprintf( where_part, "where (id %% %d = 0 %s) and time >= '%s'", div, jumpstr, starttimstr);
      }
    }
    else if ( endtime) {
      if ( div == 1) {
        if ( options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where time <= %ld", (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where time <= '%s'", endtimstr);
      }
      else {
        if ( options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where (id %% %d = 0 %s) and time <= %ld", div, jumpstr, (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where (id %% %d = 0 %s) and time <= '%s'", div, jumpstr, endtimstr);
      }
    }
    else {
      if ( div == 1)
        strcpy( where_part, "");
      else
        sprintf( where_part, "where id %% %d = 0 %s", div, jumpstr);
    }
  }
  else {
    // Not read optimized
    if ( starttime && endtime) {
      if ( options & pwr_mSevOptionsMask_PosixTime)
        sprintf( where_part, "where time >= %ld and time <= %ld", (long int)starttime->tv_sec, (long int)endtime->tv_sec);
      else
        sprintf( where_part, "where time >= '%s' and time <= '%s'", starttimstr, endtimstr);
    }
    else if ( starttime) {
      if ( options & pwr_mSevOptionsMask_PosixTime)
        sprintf( where_part, "where time >= %ld", (long int)starttime->tv_sec);
      else
        sprintf( where_part, "where time >= '%s'", starttimstr);
    }
    else if ( endtime) {
      if ( options & pwr_mSevOptionsMask_PosixTime)
        sprintf( where_part, "where time <= %ld", (long int)endtime->tv_sec);
      else
        sprintf( where_part, "where time <= '%s'", endtimstr);
    }
    else
      strcpy( where_part, "");
  }  

  sprintf( query, "select %s from %s %s order by %s",
           column_part, oid_to_table(oid, aname), where_part, orderby_part);

  rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Get Values: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetValues Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  int rows = mysql_num_rows( result);
  int bufrows = rows;


  if ( options & pwr_mSevOptionsMask_ReadOptimized) {

    *tbuf = (pwr_tTime *) calloc( bufrows, sizeof(pwr_tTime));
    *vbuf = calloc( bufrows, size);

    int bcnt = 0;
    for ( int i = 0; i < rows; i ++) {
      int j = 0;

      // if ( div > 1)
      //   mysql_data_seek( result, i);

      row = mysql_fetch_row( result);
      if (!row) break;

      if ( options & pwr_mSevOptionsMask_PosixTime) {
        if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Posix time, high resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Posix time, low resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
      }
      else {
        if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Sql time, high resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Sql time, low resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
      }

      if(row[j] == 0) {
        //Null value
        switch(type) {
          case pwr_eType_Float32:
          case pwr_eType_Float64:
          case pwr_eType_Int8:
          case pwr_eType_Int16:
          case pwr_eType_Int32:
          case pwr_eType_Int64:
          case pwr_eType_UInt8:
          case pwr_eType_UInt16:
          case pwr_eType_UInt32:
          case pwr_eType_UInt64:
          case pwr_eType_Mask:
          case pwr_eType_Enum:
            cdh_StringToAttrValue( type, "0", ((char *)*vbuf)+ bcnt * size);
            break;
          case pwr_eType_Time:
          case pwr_eType_DeltaTime:
            //TODO deltatime??
            cdh_StringToAttrValue( type, "1970-01-01 00:00:00", ((char *)*vbuf)+ bcnt * size);
            break;
          default:
            cdh_StringToAttrValue( type, " ", ((char *)*vbuf)+ bcnt * size);
            break;
        }
        j++;
      }
      else
        cdh_StringToAttrValue( type, row[j++], ((char *)*vbuf)+ bcnt * size);

      bcnt++;
      //if ( options & pwr_mSevOptionsMask_HighTimeResolution)
      //  printf( "%5d %5d %s.%9s %s\n", i, bcnt, row[0], row[1], row[2]);
      //else
      //  printf( "%5d %5d %s %s\n", i, bcnt, row[0], row[1]);
    }
    printf( "bcnt %d bufrows %d\n", bcnt, bufrows);
    *bsize = bcnt;
    mysql_free_result( result);
  }
  else {

    *tbuf = (pwr_tTime *) calloc( bufrows, sizeof(pwr_tTime));
    *vbuf = calloc( bufrows, size);

    int bcnt = 0;
    for ( int i = 0; i < rows; i += div) {
      int j = 0;

      if ( div > 1)
        mysql_data_seek( result, i);

      row = mysql_fetch_row( result);
      if (!row) break;

      if ( options & pwr_mSevOptionsMask_PosixTime) {
        if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Posix time, high resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Posix time, low resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
      }
      else {
        if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Sql time, high resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Sql time, low resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
      }

      if(row[j] == 0) {
        //Null value
        switch(type) {
          case pwr_eType_Float32:
          case pwr_eType_Float64:
          case pwr_eType_Int8:
          case pwr_eType_Int16:
          case pwr_eType_Int32:
          case pwr_eType_Int64:
          case pwr_eType_UInt8:
          case pwr_eType_UInt16:
          case pwr_eType_UInt32:
          case pwr_eType_UInt64:
          case pwr_eType_Mask:
          case pwr_eType_Enum:
            cdh_StringToAttrValue( type, "0", ((char *)*vbuf)+ bcnt * size);
            break;
          case pwr_eType_Time:
          case pwr_eType_DeltaTime:
            //TODO deltatime??
            cdh_StringToAttrValue( type, "1970-01-01 00:00:00", ((char *)*vbuf)+ bcnt * size);
            break;
          default:
            cdh_StringToAttrValue( type, " ", ((char *)*vbuf)+ bcnt * size);
            break;
        }
        j++;
      }
      else
        cdh_StringToAttrValue( type, row[j++], ((char *)*vbuf)+ bcnt * size);

      bcnt++;
      //if ( options & pwr_mSevOptionsMask_HighTimeResolution)
      //  printf( "%5d %5d %s.%9s %s\n", i, bcnt, row[0], row[1], row[2]);
      //else
      //  printf( "%5d %5d %s %s\n", i, bcnt, row[0], row[1]);
    }
    printf( "bcnt %d bufrows %d\n", bcnt, bufrows);
    *bsize = bcnt;
    mysql_free_result( result);
  }
  *sts = SEV__SUCCESS;
  return 1;
}


int sev_dbms::check_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
                          pwr_tDeltaTime storagetime, pwr_eType type, unsigned int size, 
                          char *description, char *unit, pwr_tFloat32 scantime, 
                          pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx)
{
  char timestr[40];
  pwr_tTime uptime;

  time_GetTime( &uptime);
  time_AtoAscii( &uptime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  for ( unsigned int i = 0; i < m_items.size(); i++) {
    if ( m_items[i].deleted)
      continue;

    if ( cdh_ObjidIsEqual( oid, m_items[i].oid) && 
         cdh_NoCaseStrcmp( aname, m_items[i].attr[0].aname) == 0) {
      char query[600];

      if ( type != m_items[i].attr[0].type ||
           size != m_items[i].attr[0].size)
      {
        //Size or type changed what to do?
        if( !handle_attrchange(sts, m_items[i].tablename, oid, (char *)"value", oname, type, size, i, 0) ) {
          return 1;
        }
      }

      sprintf( query, "update items set ");
      if ( storagetime.tv_sec != m_items[i].storagetime.tv_sec) {
        sprintf( &query[strlen(query)], "storagetime=%ld,", (long int)storagetime.tv_sec);
        m_items[i].storagetime = storagetime;
      }
      if ( strcmp( oname, m_items[i].oname) != 0) {
        sprintf( &query[strlen(query)], "oname=\'%s\',", oname);
        strncpy(m_items[i].oname , oname, sizeof(m_items[i].oname));
      }
      if ( type != m_items[i].attr[0].type) {
        sprintf( &query[strlen(query)], "vtype=%d,", type);
        m_items[i].attr[i].type = type;
      }
      if ( size != m_items[i].attr[0].size) {
        sprintf( &query[strlen(query)], "vsize=%d,", size);
        m_items[i].attr[i].size = size;
      }
      if ( scantime != m_items[i].scantime) {
        sprintf( &query[strlen(query)], "scantime=%.1f,", scantime);
        m_items[i].scantime = scantime;
      }
      if ( deadband != m_items[i].deadband) {
        sprintf( &query[strlen(query)], "deadband=%.4f,", deadband);
        m_items[i].deadband = deadband;
      }
      if ( strcmp( description, m_items[i].description) != 0) {
        sprintf( &query[strlen(query)], "description=\'%s\',", description);
        strncpy(m_items[i].description , description, sizeof(m_items[i].description));
      }
      if ( strcmp( unit, m_items[i].attr[0].unit) != 0) {
        sprintf( &query[strlen(query)], "unit=\'%s\',", unit);
        strncpy(m_items[i].attr[0].unit, unit, sizeof(m_items[i].attr[0].unit));
      }

      sprintf( &query[strlen(query)], "uptime=\'%s\' ", timestr);
      sprintf( &query[strlen(query)], "where id=%d;", m_items[i].id); 

      //This won't work!! We have to alter the table to be able to change this
      //m_items[i].options = options;


      int rc = mysql_query( m_env->con(), query);
      if (rc) {
        printf( "Store item: %s\n", mysql_error(m_env->con()));
        *sts = SEV__DBERROR;
        return 0;
      }
      *idx = i;
      *sts = SEV__SUCCESS;
      return 1;
    }
  }
  *sts = SEV__NOSUCHITEM;
  return 0;
}

int sev_dbms::add_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
			pwr_tDeltaTime storagetime, pwr_eType type, unsigned int size, 
			char *description, char *unit, pwr_tFloat32 scantime, 
			pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx)
{
  char tablename[256];

  strcpy( tablename, oid_to_table( oid, aname));
	  
  store_item( sts, tablename, oid, oname, aname, storagetime, type, size, description, unit,
	      scantime, deadband, options);
  if ( EVEN(*sts)) return 0;
  
  create_table( sts, oid, aname, type, size, options, deadband);
  if ( EVEN(*sts)) return 0;

  sev_item item;
  sev_attr attr;
  item.attr.push_back(attr);
  item.id = 0;
  strncpy( item.tablename, tablename, sizeof(item.tablename));
  item.oid = oid;
  strncpy( item.oname, oname, sizeof(item.oname));
  strncpy( item.attr[0].aname, aname, sizeof(item.attr[0].aname));
  time_GetTime( &item.creatime);
  item.modtime = item.creatime;
  item.storagetime = storagetime;
  item.attr[0].type = type;
  item.attr[0].size = size;
  strncpy( item.description, description, sizeof(item.description));
  strncpy( item.attr[0].unit, unit, sizeof(item.attr[0].unit));
  item.scantime = scantime;
  item.deadband = deadband;
  item.options = options;
  item.attrnum = 1;

  m_items.push_back( item);
  *idx = m_items.size() - 1;

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::delete_item( pwr_tStatus *sts, pwr_tOid oid, char *aname)
{
  char tablename[256];

  strcpy( tablename, oid_to_table( oid, aname));
	  
  remove_item( sts, oid, aname);
  
  if ( ODD(*sts))
    delete_table( sts, oid, aname);

  if ( ODD(*sts)) {
    for ( int i = 0; i < (int)m_items.size(); i++) {
      if ( m_items[i].deleted)
	continue;
      if ( cdh_ObjidIsEqual( m_items[i].oid, oid) &&
	   cdh_NoCaseStrcmp( m_items[i].attr[0].aname, aname) == 0) {
	m_items[i].deleted = 1;
	break;
      }
    }
  }
  return 1;
}

int sev_dbms::delete_old_data( pwr_tStatus *sts, pwr_tOid oid, char *aname, 
			       pwr_tMask options, pwr_tTime limit)
{
  char query[300];
  char timstr[40];

  *sts = time_AtoAscii( &limit, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
  if ( EVEN(*sts)) return 0;
  timstr[19] = 0;


  if(options & pwr_mSevOptionsMask_ReadOptimized) {
    pwr_tUInt64 nbRowsToClean = 2000;
    nbRowsToClean += get_minFromIntegerColumn(oid_to_table(oid, aname), (char *)"id");
    if ( options & pwr_mSevOptionsMask_PosixTime)
      sprintf( query, "delete from %s where id < %llu and time < %ld;",
         oid_to_table(oid, aname), nbRowsToClean, (long int)limit.tv_sec);
    else
      sprintf( query, "delete from %s where id < %llu and time < '%s';",
         oid_to_table(oid, aname), nbRowsToClean, timstr);
  }
  else {
    if ( options & pwr_mSevOptionsMask_PosixTime)
      sprintf( query, "delete from %s where time < %ld;",
         oid_to_table(oid, aname), (long int)limit.tv_sec);
    else
      sprintf( query, "delete from %s where time < '%s';",
         oid_to_table(oid, aname), timstr);
  }
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Delete old data: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  *sts = SEV__SUCCESS;
  return 1;  
}

int sev_dbms::timestr_to_time( char *tstr, pwr_tTime *ts)
{
  struct tm tt;

  int nr = sscanf(tstr, "%4d-%02d-%20d %02d:%02d:%02d", &tt.tm_year,
		  &tt.tm_mon, &tt.tm_mday, &tt.tm_hour, &tt.tm_min, &tt.tm_sec);
  if ( nr != 6) return 0;

  tt.tm_year -= 1900;
  tt.tm_mon--;

  tt.tm_wday = -1;
  tt.tm_yday = -1;
  tt.tm_isdst = -1;

  ts->tv_sec = mktime(&tt);
  ts->tv_nsec = 0;

  return 1;
}

char *sev_dbms::oid_to_table( pwr_tOid oid, char *aname)
{
  static char tbl[40];
  unsigned char	vid[4];

  memcpy( &vid, &oid.vid, sizeof(vid));
  sprintf( tbl, "O%3.3u_%3.3u_%3.3u_%3.3u_%8.8x_%s",
	   vid[3], vid[2], vid[1], vid[0], oid.oix, cdh_Low(aname));

  // Replace '.' in attribute with '_'
  for ( char *s = tbl; *s; s++) {
    if ( *s == '.')
      *s = '_';
    if ( *s == '[')
      *s = '$';
    if ( *s == ']')
      *s = '$';
  }
  return tbl;
}

char *sev_dbms::pwrtype_to_type( pwr_eType type, unsigned int size)
{
  static char stype[40];

  switch ( type) {
  case pwr_eType_Boolean:
    strcpy( stype, "int unsigned");
    break;
  case pwr_eType_Int64:
    strcpy( stype, "bigint");
    break;
  case pwr_eType_UInt64:
    strcpy( stype, "bigint unsigned");
    break;
  case pwr_eType_Int32:
    strcpy( stype, "int");
    break;
  case pwr_eType_UInt32:
    strcpy( stype, "int unsigned");
    break;
  case pwr_eType_Int16:
    strcpy( stype, "smallint");
    break;
  case pwr_eType_UInt16:
    strcpy( stype, "smallint unsigned");
    break;
  case pwr_eType_Int8:
    strcpy( stype, "tinyint");
    break;
  case pwr_eType_UInt8:
    strcpy( stype, "tinyint unsigned");
    break;
  case pwr_eType_Float64:
    strcpy( stype, "double");
    break;
  case pwr_eType_Float32:
    strcpy( stype, "float");
    break;
  case pwr_eType_Char:
    strcpy( stype, "char(1)");
    break;
  case pwr_eType_String:
    sprintf( stype, "varchar(%d)", size);
    break;
  case pwr_eType_Time:
    strcpy( stype, "datetime");
    break;
  default:
    strcpy( stype, "");
  }
  return stype;
}


int sev_dbms::check_objectitem( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, char *aname, 
                                 pwr_tDeltaTime storagetime,
                                 char *description, pwr_tFloat32 scantime, 
                                 pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx)
{
  char timestr[40];
  pwr_tTime uptime;

  time_GetTime( &uptime);
  time_AtoAscii( &uptime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  for ( unsigned int i = 0; i < m_items.size(); i++) {
    if ( m_items[i].deleted)
      continue;

    if ( cdh_ObjidIsEqual( oid, m_items[i].oid) && 
         cdh_NoCaseStrcmp( oname, m_items[i].oname) == 0) {

      char query[400];
      sprintf( query, "update objectitems set ");
      sprintf( &query[strlen(query)], "storagetime=%ld,", (long int)storagetime.tv_sec);
      sprintf( &query[strlen(query)], "description=\'%s\',", description);
      sprintf( &query[strlen(query)], "scantime=%.3f,", scantime);
      sprintf( &query[strlen(query)], "deadband=%.3f,", deadband);
//For now we can't change options. sprintf( &query[strlen(query)], "options=%d,", options);
      sprintf( &query[strlen(query)], "uptime=\'%s\' ", timestr);
      sprintf( &query[strlen(query)], "where vid=%d and oix=%d and tablename='%s';", oid.vid, oid.oix, tablename); 

      //printf("%s query:%s\n", __FUNCTION__, query);

      int rc = mysql_query( m_env->con(), query);
      if (rc) {
        printf( "%s : %s\n", __FUNCTION__, mysql_error(m_env->con()));
        *sts = SEV__DBERROR;
        return 0;
      }
      m_items[i].storagetime = storagetime;
      strncpy(m_items[i].description, description, sizeof(m_items[i].description));
      m_items[i].scantime = scantime;
      m_items[i].deadband = deadband;
      //This won't work!! We have to alter the table to be able to change this
      //m_items[i].options = options;

      *idx = i;
      *sts = SEV__SUCCESS;
      return 1;
    }
  }
  *sts = SEV__NOSUCHITEM;
  return 0;

}

int sev_dbms::add_objectitem( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, char *aname, 
			pwr_tDeltaTime storagetime,
			char *description, pwr_tFloat32 scantime, 
			pwr_tFloat32 deadband, pwr_tMask options, unsigned int *idx)
{
	  
  store_objectitem( sts, tablename, oid, oname, aname, storagetime, description,
	      scantime, deadband, options);
  if ( EVEN(*sts)) return 0;
  
  create_objecttable( sts, tablename, oid, aname, options, deadband);
  if ( EVEN(*sts)) return 0;

  *sts = SEV__SUCCESS;
  sev_item item;
  item.id = 0;
  strncpy( item.tablename, tablename, sizeof(item.tablename));
  item.oid = oid;
  strncpy( item.oname, oname, sizeof(item.oname));
  time_GetTime( &item.creatime);
  item.modtime = item.creatime;
  item.storagetime = storagetime;
  strncpy( item.description, description, sizeof(item.description));
  item.scantime = scantime;
  item.deadband = deadband;
  item.options = options;
  item.attrnum = 0;

  m_items.push_back( item);
  *idx = m_items.size() - 1;

  return 1;
}
int sev_dbms::store_objectitem( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, 
			  char *aname, pwr_tDeltaTime storagetime, char *description, pwr_tFloat32 scantime,
			  pwr_tFloat32 deadband, pwr_tMask options)
{
  char query[800];
  char timestr[40];
  pwr_tTime creatime;

  time_GetTime( &creatime);
  time_AtoAscii( &creatime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  sprintf( query, "insert into objectitems (id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,description,scantime,deadband,options) "
	   "values (0,'%s',%d,%d,'%s','%s','%s','%s',%ld,'%s',%f,%f,%d);",
	   tablename, oid.vid, oid.oix, oname, aname, timestr, timestr, (long int)storagetime.tv_sec, description, scantime, deadband, options);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Store item: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::create_objecttable( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *aname, pwr_tMask options, float deadband)
{
  char query[2000];
	char timeformatstr[80];
  char jumpstr[80];
  char idtypestr[20];
  char readoptstr[80];

  if ( options & pwr_mSevOptionsMask_PosixTime) {
    if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
      // Posix time, high resolution
      strcpy( timeformatstr, "sev__time int unsigned, sev__ntime int unsigned");
      strcpy( idtypestr, "bigint");
    }
    else {
      // Posix time, low resolution
      strcpy( timeformatstr, "sev__time int unsigned");
      strcpy( idtypestr, "int");
    }
  }
  else {
    if ( options & pwr_mSevOptionsMask_HighTimeResolution) {
      // Sql time, high resolution
      strcpy( timeformatstr, "sev__time datetime not null, sev__ntime int unsigned");
      strcpy( idtypestr, "bigint");
    }
    else {
      // Sql time, low resolution
      strcpy( timeformatstr, "sev__time datetime not null");
      strcpy( idtypestr, "int");
    }
  }

  if ( options & pwr_mSevOptionsMask_ReadOptimized)
    sprintf( readoptstr, "sev__id %s unsigned not null primary key auto_increment,", idtypestr);
  else
    strcpy( readoptstr, "");

  if ( options & pwr_mSevOptionsMask_UseDeadBand)
    strcpy( jumpstr, ",sev__jump tinyint unsigned");
  else
    strcpy( jumpstr, "");

  sprintf( query, "create table %s ( %s"
	   "%s %s, index (sev__time) );",
	   tablename, readoptstr, timeformatstr, jumpstr);

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__ ,mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
  return 1;
}

int sev_dbms::check_objectitemattr( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *aname, char *oname, 
																	  pwr_eType type, unsigned int size, unsigned int *idx)
{
  sev_item *item = &m_items[*idx];
  for (size_t j = 0; j < item->attr.size(); j++) {
    if (cdh_NoCaseStrcmp( aname, item->attr[j].aname) == 0) {
      if (type != item->attr[j].type ||
          size != item->attr[j].size) {
        if( !handle_attrchange(sts, tablename, oid, aname,oname,type,size, *idx, j) ) {
          return 1;
        }
        item->attr[j].type = type;
        item->attr[j].size = size;
        update_objectitemattr(sts, tablename, aname, type, size);
      }
      *sts = SEV__SUCCESS;
      return 1;
    }
  }
  *sts = SEV__NOSUCHITEM;
  return 0;
}


int sev_dbms::add_objectitemattr( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *aname, char *oname, 
																	pwr_eType type, unsigned int size, unsigned int *idx)
{
	char query[2000];

	sprintf( query, "alter table %s add `%s` %s;",
		 tablename, aname, pwrtype_to_type( type, size));

	//printf( "%s: %s\n", __FUNCTION__ ,query);

	int rc = mysql_query( m_env->con(), query);
	if (rc) {
		printf( "%s: %s\n", __FUNCTION__,mysql_error(m_env->con()));
		*sts = SEV__DBERROR;
		return 0;
	}

  int aidx = get_nextattridx(sts, tablename);

  sprintf( query, "insert into objectitemattributes (tablename, attributename, attributeidx, attributetype, attributesize) "
                  "values('%s', '%s', %d, %d, %d)", tablename, aname, aidx, type, size);


  //printf( "%s: %s\n", __FUNCTION__ ,query);
  rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__,mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  sev_attr newattr;
  strncpy( newattr.aname, aname, sizeof(newattr.aname));
  newattr.type = type;
  newattr.size = size;
  newattr.elem = 0;
  m_items[*idx].attr.push_back(newattr);
  m_items[*idx].attrnum = m_items[*idx].attr.size();

  *sts = SEV__SUCCESS;
	return 1;
}

int sev_dbms::get_nextattridx( pwr_tStatus *sts, char *tablename )
{
	char query[2000];

  sprintf( query, "select max(attributeidx) from objectitemattributes where tablename='%s'", tablename);

  //printf( "%s: %s\n", __FUNCTION__ ,query);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__,mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetValues Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }

  int rows = mysql_num_rows( result);

  if(rows > 1) {
    printf( "Duplicate items Error\n");
    *sts = SEV__DBERROR;
    mysql_free_result( result);
    return 0;
  }
  int attridx = -1;
  for ( int i = 0; i < rows; i++) {
    row = mysql_fetch_row( result);
    if (!row) break;
    if(row[0] != NULL) {
      attridx = atoi( row[0]);
    }
  }
  mysql_free_result( result);

  *sts = SEV__SUCCESS;
  return attridx+1;
}

pwr_tUInt64 sev_dbms::get_minFromIntegerColumn( char *tablename, char *colname )
{
	char query[2000];
  pwr_tUInt64 retVal = 0;
  sprintf( query, "select min(`%s`) from %s", colname, tablename);

  //printf( "%s: %s\n", __FUNCTION__ ,query);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__,mysql_error(m_env->con()));
    return 0;
  }
  MYSQL_RES *result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetValues Result Error\n");
    return retVal;
  }

  int rows = mysql_num_rows( result);
  if(rows <= 0) {
    mysql_free_result( result);
    return retVal;
  }
  MYSQL_ROW row;
  row = mysql_fetch_row( result);
  if(row[0] != NULL) {
    retVal = strtoull(row[0], 0, 10);
  }
  mysql_free_result( result);
  return retVal;
}


int sev_dbms::store_objectvalue( pwr_tStatus *sts, int item_idx, int attr_idx,
                           pwr_tTime time, void *buf, void *oldbuf, unsigned int size)
{
  void *data = buf;
  void *olddata = oldbuf;

  static const int constQueryLength = 3000;
  string valuesStr;
  string colsStr;
  ostringstream queryOStr;

  char query[constQueryLength];
  char bufstr[512];
  char bufInclEscCharstr[1025];
  char timstr[40];

  *sts = time_AtoAscii( &time, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
  if ( EVEN(*sts)) return 0;
  timstr[19] = 0;

  bool updateOnlyTime = false;
  bool setJump = false;
  if ( !m_items[item_idx].first_storage && 
       (m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand) ) {
    if ( m_items[item_idx].deadband_active) {
      updateOnlyTime = true;
      for (size_t i = 0; i < m_items[item_idx].attr.size(); i++) {
        pwr_tFloat32 deadband = m_items[item_idx].deadband; //TODO should be on attribute
        m_items[item_idx].deadband_active = check_deadband(m_items[item_idx].attr[i].type, m_items[item_idx].attr[i].size, deadband, buf, oldbuf);
        if (!m_items[item_idx].deadband_active) {
          updateOnlyTime = false;
          break;
        }
        buf = (char*)buf + m_items[item_idx].attr[i].size;
        oldbuf = (char*)oldbuf + m_items[item_idx].attr[i].size;
      }
    }
    else {
      setJump = true;
      for (size_t i = 0; i < m_items[item_idx].attr.size(); i++) {
        pwr_tFloat32 deadband = m_items[item_idx].deadband; //TODO should be on attribute
        m_items[item_idx].deadband_active = check_deadband(m_items[item_idx].attr[i].type, m_items[item_idx].attr[i].size, deadband, buf, oldbuf);
        if (!m_items[item_idx].deadband_active) {
          setJump = false;
          break;
        }
        buf = (char*)buf + m_items[item_idx].attr[i].size;
        oldbuf = (char*)oldbuf + m_items[item_idx].attr[i].size;
      }
    }
  }
  buf = data;
  oldbuf = olddata;

  if(!updateOnlyTime) {

    memcpy(oldbuf, buf, size);
  
    for(size_t i = 0; i < m_items[item_idx].attr.size(); i++) {
      if(m_items[item_idx].attr[i].type == pwr_eType_Time)
        *sts = time_AtoAscii( (pwr_tTime*)buf, time_eFormat_NumDateAndTime, bufstr, sizeof(bufstr));
      else
        *sts = cdh_AttrValueToString( m_items[item_idx].attr[i].type, buf, bufstr, sizeof(bufstr));
      if ( EVEN(*sts)) return 0;
      if(m_items[item_idx].attr[i].type == pwr_eType_String ||
         m_items[item_idx].attr[i].type == pwr_eType_Text) {
        mysql_real_escape_string(m_env->con(), bufInclEscCharstr, bufstr, strlen(bufstr) );
        valuesStr.append("'");
        valuesStr.append(bufInclEscCharstr);
        valuesStr.append("',");
      }
      else {  
        valuesStr.append("'");
        valuesStr.append(bufstr);
        valuesStr.append("',");
      }
  
      colsStr.append("`");
      colsStr.append(m_items[item_idx].attr[i].aname);
      colsStr.append("`,");
  
      buf = (char*)buf + m_items[item_idx].attr[i].size;
    }
    //remove last ,
    valuesStr.resize(valuesStr.length()-1);
    colsStr.resize(colsStr.length()-1);
  
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_PosixTime) {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Posix time, high resolution
        queryOStr << "insert into " << m_items[item_idx].tablename << " (sev__time, sev__ntime, " << colsStr << ") values (" << time.tv_sec <<
          "," << time.tv_nsec << "," << valuesStr << ")";
         
        snprintf(query, constQueryLength, "insert into %s (sev__time, sev__ntime, %s) values (%ld,%ld,%s)", 
                m_items[item_idx].tablename, colsStr.c_str(), (long int)time.tv_sec, (long int)time.tv_nsec, valuesStr.c_str());
      }
      else {
        // Posix time, low resolution
        queryOStr << "insert into " << m_items[item_idx].tablename << " (sev__time, " << colsStr << ") values (" << time.tv_sec <<
          "," << valuesStr  << ")";
  
        snprintf(query, constQueryLength, "insert into %s (sev__time, %s) values (%ld,%s)", 
                 m_items[item_idx].tablename, colsStr.c_str(), (long int)time.tv_sec, valuesStr.c_str());
      }
    }
    else {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Sql time, high resolution
        queryOStr << "insert into " << m_items[item_idx].tablename << " (sev__time, sev__ntime, " << colsStr << ") values (" << "'" << timstr << "'" <<
          "," << time.tv_nsec << "," << valuesStr << ")";
  
        snprintf(query, constQueryLength, "insert into %s (sev__time, sev__ntime, %s) values ('%s',%ld,%s)", 
                m_items[item_idx].tablename, colsStr.c_str(), timstr, (long int)time.tv_nsec, valuesStr.c_str());
  
      }
      else {
        // Sql time, low resolution
        queryOStr << "insert into " << m_items[item_idx].tablename << " (sev__time, " << colsStr << ") values (" << "'" << timstr << "'" <<
          "," << valuesStr  << ")";
  
        snprintf(query, constQueryLength, "insert into %s (sev__time, %s) values ('%s',%s)", 
                 m_items[item_idx].tablename, colsStr.c_str(), timstr, valuesStr.c_str());
      }
    }
  } //end insert new values
  else {
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_PosixTime) {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Posix time, high resolution
        queryOStr << "update " << m_items[item_idx].tablename << " set sev__time = " << time.tv_sec << ", sev__ntime = " << time.tv_nsec << " where sev__id = " << m_items[item_idx].last_id;
      }
      else {
        // Posix time, low resolution
        queryOStr << "update " << m_items[item_idx].tablename << " set sev__time = " << time.tv_sec << " where sev__id = " << m_items[item_idx].last_id;
      }
    }
    else {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
        // Sql time, high resolution
        queryOStr << "update " << m_items[item_idx].tablename << " set sev__time = '" << timstr << "', sev__ntime = " << time.tv_nsec << " where sev__id = " << m_items[item_idx].last_id;
      }
      else {
        // Sql time, low resolution
        queryOStr << "update " << m_items[item_idx].tablename << " set sev__time = '" << timstr << "' where sev__id = " << m_items[item_idx].last_id;
      }
    }
  }

  if ( setJump || updateOnlyTime) {
    sprintf( query, "update %s set sev__jump = 1 where sev__id = %d",
             m_items[item_idx].tablename, 
             m_items[item_idx].last_id);
    int rc = mysql_query( m_env->con(), query);
    if (rc)
      printf( "Update jump: %s\n", mysql_error(m_env->con()));
  }

  //printf( "Store_objectvalue: %s\n", queryOStr.str().c_str());

  int rc = mysql_query( m_env->con(), queryOStr.str().c_str());
  if (rc) {
    printf( "Update value: %s\n", mysql_error(m_env->con()));
    printf( "Error in: %s\n", queryOStr.str().c_str());

    *sts = SEV__DBERROR;
    m_items[item_idx].status = *sts;
    if ( m_items[item_idx].status != m_items[item_idx].logged_status) {
      m_items[item_idx].logged_status = m_items[item_idx].status;
      errh_Error( "Database update error: %s, table: %s object: %s", 
                  mysql_error(m_env->con()), m_items[item_idx].tablename, m_items[item_idx].oname);
    }
    return 0;
  }

  if ( (m_items[item_idx].options & pwr_mSevOptionsMask_ReadOptimized) &&
       !updateOnlyTime)
    m_items[item_idx].last_id = mysql_insert_id( m_env->con());

  m_items[item_idx].first_storage = 0;

  *sts = SEV__SUCCESS;
  m_items[item_idx].status = *sts;
  m_items[item_idx].logged_status = 1;
  return 1;
}

int sev_dbms::get_item( pwr_tStatus *sts, sev_item *item, char *tablename)
{
  char query[300];

  sprintf( query, "select id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,vtype,vsize,description,unit,scantime,deadband,options "
	   "from items where tablename='%s'", tablename);

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__, mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetValues Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  int rows = mysql_num_rows( result);

  if(rows > 1) {
    printf( "Duplicate items Error\n");
    *sts = SEV__DBERROR;
    mysql_free_result( result);
    return 0;
  }
  if(rows == 0) {
    printf( "No item Error\n");
    *sts = SEV__DBERROR;
    mysql_free_result( result);
    return 0;
  }

  int col;
  for ( int i = 0; i < rows; i++) {
    row = mysql_fetch_row( result);
    if (!row) break;
    col=0;
    sev_attr attr;
    item->attr.push_back(attr);

    item->id = atoi( row[col++]);
    strncpy( item->tablename, row[col++], sizeof(item->tablename));
    item->oid.vid = strtoul( row[col++], 0, 10);
    item->oid.oix = strtoul( row[col++], 0, 10);
    strncpy( item->oname, row[col++], sizeof(item->oname));
    strncpy( item->attr[0].aname, row[col++], sizeof(item->attr[0].aname));
    timestr_to_time( row[col++], &item->modtime);
    timestr_to_time( row[col++], &item->creatime);
    item->storagetime.tv_sec = strtoul( row[col++], 0, 10);
    item->storagetime.tv_nsec = 0;
    item->attr[0].type = (pwr_eType) strtoul( row[col++], 0, 10);
    item->attr[0].size = strtoul( row[col++], 0, 10);
    strncpy( item->description, row[col++], sizeof(item->description));
    strncpy( item->attr[0].unit, row[col++], sizeof(item->attr[0].unit));
    item->scantime = atof(row[col++]);
    item->deadband = atof(row[col++]);
    item->options = strtoul(row[col++], 0, 10);

    item->attrnum = 1;
  }
  mysql_free_result( result);
  
  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::get_objectitem( pwr_tStatus *sts, sev_item *item, char *tablename)
{
  char query[300];

  sprintf( query, "select id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,description,scantime,deadband,options "
     "from objectitems where tablename='%s'", tablename);

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__, mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "get_objectitem Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  int rows = mysql_num_rows( result);
  if(!rows) {
    mysql_free_result( result);
    printf( "get_objectitem Row Error\n");
    printf( "get_objectitem: %s\n", query);
    *sts = SEV__DBERROR;
    return 0;
  }
  row = mysql_fetch_row( result);
  if (!row) {
    mysql_free_result( result);
    printf( "get_objectitem Row Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }

  int col = 0;
  item->id = atoi( row[col++]);
  strncpy( item->tablename, row[col++], sizeof(item->tablename));
  item->oid.vid = strtoul( row[col++], 0, 10);
  item->oid.oix = strtoul( row[col++], 0, 10);
  strncpy( item->oname, row[col++], sizeof(item->oname));
  col++; //No aname in sev_item TODO add aname to sev_item??
  timestr_to_time( row[col++], &item->modtime);
  timestr_to_time( row[col++], &item->creatime);
  item->storagetime.tv_sec = strtoul( row[col++], 0, 10);
  item->storagetime.tv_nsec = 0;
  strncpy( item->description, row[col++], sizeof(item->description));
  item->scantime = atof(row[col++]);
  item->deadband = atof(row[col++]);
  item->options = strtoul(row[col++], 0, 10);
  //Time to fetch all attributes for this item
  get_objectitemattributes(sts, item, item->tablename);

  item->attrnum = item->attr.size();
  
  mysql_free_result( result);

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::get_objectitems( pwr_tStatus *sts)
{
  char query[300];

  sprintf( query, "select id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,description,scantime,deadband,options "
	   "from objectitems");

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Get ObjectItems: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetObjectItems Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  int rows = mysql_num_rows( result);
  int col;
  for ( int i = 0; i < rows; i++) {
    sev_item item;
    row = mysql_fetch_row( result);
    if (!row) break;

    //printf( "%d %s %s\n", i, row[0], row[1]);
    col=0;
    item.id = atoi( row[col++]);
    strncpy( item.tablename, row[col++], sizeof(item.tablename));
    item.oid.vid = strtoul( row[col++], 0, 10);
    item.oid.oix = strtoul( row[col++], 0, 10);
    strncpy( item.oname, row[col++], sizeof(item.oname));
    col++; //No aname in sev_item TODO add aname to sev_item??
    timestr_to_time( row[col++], &item.modtime);
    timestr_to_time( row[col++], &item.creatime);
    item.storagetime.tv_sec = strtoul( row[col++], 0, 10);
    item.storagetime.tv_nsec = 0;
    strncpy( item.description, row[col++], sizeof(item.description));
    item.scantime = atof(row[col++]);
    item.deadband = atof(row[col++]);
    item.options = strtoul(row[col++], 0, 10);
    //Time to fetch all attributes for this item
    get_objectitemattributes(sts, &item, item.tablename);

    item.attrnum = item.attr.size();

    m_items.push_back( item);
  }
  mysql_free_result( result);
  

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::get_objectitemattributes( pwr_tStatus *sts, sev_item *item, char *tablename)
{
  char query[300];

  sprintf( query, "select attributename, attributetype, attributesize from objectitemattributes where tablename='%s'order by attributeidx asc", tablename);

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Get ObjectItemAttr: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetObjectItemAttr Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  int rows = mysql_num_rows( result);
  int col;
  for ( int i = 0; i < rows; i++) {
    sev_attr attr;
    row = mysql_fetch_row( result);
    if (!row) break;
    col=0;
    strncpy( attr.aname, row[col++], sizeof(attr.aname));
    attr.type = (pwr_eType) strtoul( row[col++], 0, 10);
    attr.size = strtoul( row[col++], 0, 10);
    attr.unit[0] = '\0'; //No unit present TODO add??
    item->value_size += attr.size;
    item->attr.push_back( attr);
  }
  mysql_free_result( result);
  

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::delete_old_objectdata( pwr_tStatus *sts, char *tablename, 
			                               pwr_tMask options, pwr_tTime limit)
{
  char query[300];
  char timstr[40];

  *sts = time_AtoAscii( &limit, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
  if ( EVEN(*sts)) return 0;
  timstr[19] = 0;


  if(options & pwr_mSevOptionsMask_ReadOptimized) {
    pwr_tUInt64 nbRowsToClean = 2000;
    nbRowsToClean += get_minFromIntegerColumn(tablename, (char *)"sev__id");
    if ( options & pwr_mSevOptionsMask_PosixTime)
      sprintf( query, "delete from %s where sev__id < %llu and sev__time < %ld;",
         tablename, nbRowsToClean, (long int)limit.tv_sec);
    else
      sprintf( query, "delete from %s where sev__id < %llu and sev__time < '%s';",
         tablename, nbRowsToClean, timstr);
  }
  else {
    if ( options & pwr_mSevOptionsMask_PosixTime)
      sprintf( query, "delete from %s where sev__time < %ld;",
         tablename, (long int)limit.tv_sec);
    else
      sprintf( query, "delete from %s where sev__time < '%s';",
         tablename, timstr);
  }

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Delete old object data: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  *sts = SEV__SUCCESS;
  return 1;  
}

int sev_dbms::alter_attrcolumn(pwr_tStatus *sts, char *tablename, char *aname, 
                               pwr_eType newtype, unsigned int newsize, 
                               pwr_eType oldtype, unsigned int oldsize)
{
  //TODO
  return 0;
}
int sev_dbms::rename_attrcolumn(pwr_tStatus *sts, char *tablename, char *aname, pwr_eType type, unsigned int size)
{
  char query[2000];
  char newname[500];
  char timestr[40];
  pwr_tTime uptime;

  time_GetTime( &uptime);
  time_AtoAscii( &uptime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;
  

  sprintf(newname, "%s_before_%s", aname, timestr);
  sprintf(query, "alter table %s change `%s` `%s` %s", tablename, aname, newname, pwrtype_to_type(type, size));

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__, mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
  return 1;
}
int sev_dbms::remove_objectitemattr( pwr_tStatus *sts, char *tablename, char *aname)
{
	char query[2000];

  sprintf( query, "delete from objectitemattributes where tablename = '%s' and attributename = '%s'", tablename, aname);

  //printf( "%s: %s\n", __FUNCTION__ ,query);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__,mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
	return 1;
}

int sev_dbms::update_objectitemattr( pwr_tStatus *sts, char *tablename, char *aname, pwr_eType type, unsigned int size)
{
	char query[2000];

  sprintf( query, "update objectitemattributes set attributetype=%d, attributesize=%d where tablename = '%s' and attributename = '%s'", 
           type, size, tablename, aname);

  //printf( "%s: %s\n", __FUNCTION__ ,query);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "%s: %s\n", __FUNCTION__,mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
	return 1;
}

int sev_dbms::check_deadband(pwr_eType type, unsigned int size, pwr_tFloat32 deadband, void *value, void *oldvalue)
{
  int deadband_active = 0;
  switch ( type) {
    case pwr_eType_Float32:
      if ( fabsf ( *(pwr_tFloat32 *)value - *(pwr_tFloat32 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_Float64:
      if ( fabsf ( *(pwr_tFloat64 *)value - *(pwr_tFloat64 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_Int64:
      if ( fabsf ( *(pwr_tInt64 *)value - *(pwr_tInt64 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_Int32:
      if ( fabsf ( *(pwr_tInt32 *)value - *(pwr_tInt32 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_Int16:
      if ( fabsf ( *(pwr_tInt16 *)value - *(pwr_tInt16 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_Int8:
    case pwr_eType_Char:
      if ( fabsf ( *(pwr_tInt8 *)value - *(pwr_tInt8 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_UInt64:
      if ( fabsf ( *(pwr_tUInt64 *)value - *(pwr_tUInt64 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_UInt32:
    case pwr_eType_Boolean:
      if ( fabsf ( *(pwr_tUInt32 *)value - *(pwr_tUInt32 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_UInt16:
      if ( fabsf ( *(pwr_tUInt16 *)value - *(pwr_tUInt16 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_UInt8:
      if ( fabsf ( *(pwr_tUInt8 *)value - *(pwr_tUInt8 *)oldvalue) < deadband) {
        deadband_active = 1;
      }
      break;
    case pwr_eType_String:
    case pwr_eType_Text:
      if ( !memcmp(value, oldvalue, size) ) {
        deadband_active = 1;
      }
      break;
    default: ;
  }
  return deadband_active;
}

int sev_dbms::get_objectvalues( pwr_tStatus *sts, sev_item *item,
			  unsigned int size, pwr_tTime *starttime, pwr_tTime *endtime, 
			  int maxsize, pwr_tTime **tbuf, void **vbuf, unsigned int *bsize)
{
  char query[200];
  string queryStr;
  char starttimstr[40];
  char endtimstr[40];
  int total_rows;
  int div;
  pwr_tDeltaTime dt;
  pwr_tTime stime, etime;
  string colsStr;
  char orderby_part[80];
  char jumpstr[40];
  char where_part[200];

  if ( starttime && starttime->tv_sec == 0 && starttime->tv_nsec == 0)
    starttime = 0;
  else
    stime = *starttime;

  if ( endtime && endtime->tv_sec == 0 && endtime->tv_nsec == 0)
    endtime = 0;
  else
    etime = *endtime;

  // Get number of rows
  sprintf( query, "show table status where name = '%s';", item->tablename);

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "GetValues Query Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }

  MYSQL_ROW row;
  MYSQL_RES *result = mysql_store_result( m_env->con());

  if ( !result) {
    printf( "GetValues Status Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }

  row = mysql_fetch_row( result);
  if (!row) {
    printf( "GetValues Status Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  if ( starttime && endtime) {
    pwr_tTime create_time;
    pwr_tTime update_time;
    timestr_to_time( row[11], &create_time);
    timestr_to_time( row[12], &update_time);

    if ( time_Acomp( &item->creatime, &stime) == 1)
      stime = item->creatime;

    if ( time_Acomp( &etime, &update_time) == 1)
      etime = update_time;

    time_Adiff( &dt, &etime, &stime);
    total_rows = int (time_DToFloat( 0, &dt) / item->scantime);

    div = total_rows / maxsize + 1;
  }
  else if ( starttime) {
    pwr_tTime update_time;
    timestr_to_time( row[12], &update_time);

    if ( time_Acomp( &update_time, starttime) != 1) {
      mysql_free_result( result);
      *sts = SEV__NODATATIME;
      return 0;
    }
    time_Adiff( &dt, &update_time, starttime);
    total_rows = int (time_DToFloat( 0, &dt) / item->scantime);
  }
  else if ( endtime) {
    pwr_tTime create_time;
    timestr_to_time( row[11], &create_time);

    if ( time_Acomp( endtime, &create_time) != 1) {
      mysql_free_result( result);
      *sts = SEV__NODATATIME;
      return 0;
    }
    time_Adiff( &dt, endtime, &create_time);
    total_rows = int (time_DToFloat( 0, &dt) / item->scantime);
  }
  else {
    total_rows = atoi(row[4]);
  }
  mysql_free_result( result);

  div = total_rows / maxsize + 1;

  if ( starttime) {
    *sts = time_AtoAscii( &stime, time_eFormat_NumDateAndTime, starttimstr,
                          sizeof(starttimstr));
    if ( EVEN(*sts)) return 0;
    starttimstr[19] = 0;
  }
  if ( endtime) {
    *sts = time_AtoAscii( &etime, time_eFormat_NumDateAndTime, endtimstr, 
                          sizeof(endtimstr));
    if ( EVEN(*sts)) return 0;
    endtimstr[19] = 0;
  }

  // Column part 
  if ( item->options & pwr_mSevOptionsMask_HighTimeResolution) {
    colsStr.append("sev__time, sev__ntime, ");
  } 
  else {
    colsStr.append("sev__time, ");
  }
  for(size_t i = 0; i < item->attr.size(); i++) {
    colsStr.append("`");
    colsStr.append(item->attr[i].aname);
    colsStr.append("`,");
  }
  //remove last ,
  colsStr.resize(colsStr.length()-1);


  if ( item->options & pwr_mSevOptionsMask_UseDeadBand)
    strcpy( jumpstr, "or sev__jump = 1");
  else
    strcpy( jumpstr, "");

  // 'order by' part
  if ( item->options & pwr_mSevOptionsMask_ReadOptimized)
    strcpy( orderby_part, "sev__id");
  else {
    if ( item->options & pwr_mSevOptionsMask_HighTimeResolution)
      strcpy( orderby_part, "sev__time,sev__ntime");
    else
      strcpy( orderby_part, "sev__time");
  }

  // 'where' part
  if ( item->options & pwr_mSevOptionsMask_ReadOptimized) {
    if ( starttime && endtime) {
      if ( div == 1) {
        if ( item->options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where sev__time >= %ld and sev__time <= %ld", (long int)starttime->tv_sec, (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where sev__time >= '%s' and sev__time <= '%s'", starttimstr, endtimstr);
      }
      else {
        if ( item->options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where (sev__id %% %d = 0 %s) and sev__time >= %ld and sev__time <= %ld", 
                   div, jumpstr, (long int)starttime->tv_sec, (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where (sev__id %% %d = 0 %s) and sev__time >= '%s' and sev__time <= '%s'", 
                   div, jumpstr, starttimstr, endtimstr);
      }
    }
    else if ( starttime) {
      if ( div == 1) {
        if ( item->options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where sev__time >= %ld", (long int)starttime->tv_sec);
        else
          sprintf( where_part, "where sev__time >= '%s'", starttimstr);
      }
      else {
        if ( item->options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where (sev__id %% %d = 0 %s) and sev__time >= %ld", div, jumpstr, (long int)starttime->tv_sec);
        else
          sprintf( where_part, "where (sev__id %% %d = 0 %s) and sev__time >= '%s'", div, jumpstr, starttimstr);
      }
    }
    else if ( endtime) {
      if ( div == 1) {
        if ( item->options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where sev__time <= %ld", (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where sev__time <= '%s'", endtimstr);
      }
      else {
        if ( item->options & pwr_mSevOptionsMask_PosixTime)
          sprintf( where_part, "where (sev__id %% %d = 0 %s) and sev__time <= %ld", div, jumpstr, (long int)endtime->tv_sec);
        else
          sprintf( where_part, "where (sev__id %% %d = 0 %s) and sev__time <= '%s'", div, jumpstr, endtimstr);
      }
    }
    else {
      if ( div == 1)
        strcpy( where_part, "");
      else
        sprintf( where_part, "where sev__id %% %d = 0 %s", div, jumpstr);
    }
  }
  else {
    // Not read optimized
    if ( starttime && endtime) {
      if ( item->options & pwr_mSevOptionsMask_PosixTime)
        sprintf( where_part, "where sev__time >= %ld and sev__time <= %ld", (long int)starttime->tv_sec, (long int)endtime->tv_sec);
      else
        sprintf( where_part, "where sev__time >= '%s' and sev__time <= '%s'", starttimstr, endtimstr);
    }
    else if ( starttime) {
      if ( item->options & pwr_mSevOptionsMask_PosixTime)
        sprintf( where_part, "where sev__time >= %ld", (long int)starttime->tv_sec);
      else
        sprintf( where_part, "where sev__time >= '%s'", starttimstr);
    }
    else if ( endtime) {
      if ( item->options & pwr_mSevOptionsMask_PosixTime)
        sprintf( where_part, "where sev__time <= %ld", (long int)endtime->tv_sec);
      else
        sprintf( where_part, "where sev__time <= '%s'", endtimstr);
    }
    else
      strcpy( where_part, "");
  }  

  queryStr.append("select ");
  queryStr.append(colsStr);
  queryStr.append(" from ");
  queryStr.append(item->tablename);
  queryStr.append(" ");
  queryStr.append(where_part);
  queryStr.append(" order by ");
  queryStr.append(orderby_part);

  printf("%s: %s\n", __FUNCTION__, queryStr.c_str());

  rc = mysql_query( m_env->con(), queryStr.c_str());
  if (rc) {
    printf( "Get Values: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  result = mysql_store_result( m_env->con());
  if ( !result) {
    printf( "GetValues Result Error\n");
    *sts = SEV__DBERROR;
    return 0;
  }
  int rows = mysql_num_rows( result);
  int bufrows = rows;


  if ( item->options & pwr_mSevOptionsMask_ReadOptimized) {

    *tbuf = (pwr_tTime *) calloc( bufrows, sizeof(pwr_tTime));
    *vbuf = calloc( bufrows, item->value_size);

    int bcnt = 0;
    for ( int i = 0; i < rows; i ++) {
      int j = 0;

      // if ( div > 1)
      //   mysql_data_seek( result, i);

      row = mysql_fetch_row( result);
      if (!row) break;

      if ( item->options & pwr_mSevOptionsMask_PosixTime) {
        if ( item->options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Posix time, high resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Posix time, low resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
      }
      else {
        if ( item->options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Sql time, high resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Sql time, low resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
      }
      int read_size = 0;
      for(size_t k = 0; k < item->attr.size(); k++) {
        if(row[j] == 0) {
          //Null value
          switch(item->attr[k].type) {
            case pwr_eType_Float32:
            case pwr_eType_Float64:
            case pwr_eType_Int8:
            case pwr_eType_Int16:
            case pwr_eType_Int32:
            case pwr_eType_Int64:
            case pwr_eType_UInt8:
            case pwr_eType_UInt16:
            case pwr_eType_UInt32:
            case pwr_eType_UInt64:
            case pwr_eType_Mask:
            case pwr_eType_Enum:
              cdh_StringToAttrValue( item->attr[k].type, "0", ((char *)*vbuf)+ bcnt * item->value_size + read_size);
              break;
            case pwr_eType_Time:
            case pwr_eType_DeltaTime:
              //TODO deltatime??
              cdh_StringToAttrValue( item->attr[k].type, "1970-01-01 00:00:00", ((char *)*vbuf)+ bcnt * item->value_size + read_size);
              break;
            default:
              cdh_StringToAttrValue( item->attr[k].type, " ", ((char *)*vbuf)+ bcnt * item->value_size + read_size);
              break;
          }
          j++;
        }
        else {
          cdh_StringToAttrValue( item->attr[k].type, row[j++], ((char *)*vbuf)+ bcnt * item->value_size + read_size);
        }
        read_size += item->attr[k].size;
      }
      bcnt++;
      //if ( item->options & pwr_mSevOptionsMask_HighTimeResolution)
      //  printf( "%5d %5d %s.%9s %s\n", i, bcnt, row[0], row[1], row[2]);
      //else
      //  printf( "%5d %5d %s %s\n", i, bcnt, row[0], row[1]);
    }
    printf( "bcnt %d bufrows %d\n", bcnt, bufrows);
    *bsize = bcnt;
    mysql_free_result( result);
  }
  else {

    *tbuf = (pwr_tTime *) calloc( bufrows, sizeof(pwr_tTime));
    *vbuf = calloc( bufrows, size);

    int bcnt = 0;
    for ( int i = 0; i < rows; i += div) {
      int j = 0;

      if ( div > 1)
        mysql_data_seek( result, i);

      row = mysql_fetch_row( result);
      if (!row) break;

      if ( item->options & pwr_mSevOptionsMask_PosixTime) {
        if ( item->options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Posix time, high resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Posix time, low resolution
          (*tbuf)[bcnt].tv_sec = strtoul( row[j++], 0, 10);
      }
      else {
        if ( item->options & pwr_mSevOptionsMask_HighTimeResolution) {
          // Sql time, high resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
          (*tbuf)[bcnt].tv_nsec = strtoul( row[j++], 0, 10);
        }
        else
          // Sql time, low resolution
          timestr_to_time( row[j++], &(*tbuf)[bcnt]);
      }

      int read_size = 0;
      for(size_t k = 0; k < item->attr.size(); k++) {
        if(row[j] == 0) {
          //Null value
          switch(item->attr[k].type) {
            case pwr_eType_Float32:
            case pwr_eType_Float64:
            case pwr_eType_Int8:
            case pwr_eType_Int16:
            case pwr_eType_Int32:
            case pwr_eType_Int64:
            case pwr_eType_UInt8:
            case pwr_eType_UInt16:
            case pwr_eType_UInt32:
            case pwr_eType_UInt64:
            case pwr_eType_Mask:
            case pwr_eType_Enum:
              cdh_StringToAttrValue( item->attr[k].type, "0", ((char *)*vbuf)+ bcnt * item->value_size + read_size);
              break;
            case pwr_eType_Time:
            case pwr_eType_DeltaTime:
              //TODO deltatime??
              cdh_StringToAttrValue( item->attr[k].type, "1970-01-01 00:00:00", ((char *)*vbuf)+ bcnt * item->value_size + read_size);
              break;
            default:
              cdh_StringToAttrValue( item->attr[k].type, " ", ((char *)*vbuf)+ bcnt * item->value_size + read_size);
              break;
          }
          j++;
        }
        else {
          cdh_StringToAttrValue( item->attr[k].type, row[j++], ((char *)*vbuf)+ bcnt * item->value_size + read_size);
        }
        read_size += item->attr[k].size;
      }
      bcnt++;
      //if ( item->options & pwr_mSevOptionsMask_HighTimeResolution)
      //  printf( "%5d %5d %s.%9s %s\n", i, bcnt, row[0], row[1], row[2]);
      //else
      //  printf( "%5d %5d %s %s\n", i, bcnt, row[0], row[1]);
    }
    printf( "bcnt %d bufrows %d\n", bcnt, bufrows);
    *bsize = bcnt;
    mysql_free_result( result);
  }
  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::handle_attrchange(pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *aname, char *oname, 
													  pwr_eType type, unsigned int size, unsigned int item_idx, unsigned int attr_idx)
{
  //We try to alter the table if it is possible(float->double int->long or little_string->bigger_string probably works).
  //If we can't alter the table we rename the column and create a new one

  sev_item *item = &m_items[item_idx];

  printf("Column:%s in %s has been changed from Type/size %d/%d to %d/%d\n", 
         aname, tablename, item->attr[attr_idx].type, item->attr[attr_idx].size, type, size );
  errh_Warning("Column:%s in %s has been changed from Type/size %d/%d to %d/%d", 
               aname, tablename, item->attr[attr_idx].type, item->attr[attr_idx].size, type, size );

  if (!alter_attrcolumn(sts, tablename, aname, type, size, item->attr[attr_idx].type, item->attr[attr_idx].size)) {
    //We were not able to alter the column then we should rename the column and create a new one
    if (!rename_attrcolumn(sts, tablename, aname, item->attr[attr_idx].type, item->attr[attr_idx].size)) {
      //Very very bad this should not happen
      errh_Error("Could not rename column that has been changed tablename:%s columnname:%s", tablename, aname);
      //item->attr[attr_idx].status = -1; //TODO Skip this attribute during logging
      *sts = SEV__DBERROR;
      return 0;
    }
    errh_Warning("Column:%s in %s renamed to:%s_before_xxxx xx:xx:xx", aname, tablename, aname );

    char query[2000];
    sprintf( query, "alter table %s add `%s` %s;",
             tablename, aname, pwrtype_to_type( type, size));
    int rc = mysql_query( m_env->con(), query);
    if (rc) {
      printf( "%s: %s\n", __FUNCTION__,mysql_error(m_env->con()));
      *sts = SEV__DBERROR;
      return 0;
    }
  }
  return 1;
}

sev_dbms::~sev_dbms()
{
  printf("Freeing memory\n");
  for(size_t idx = 0; idx < m_items.size(); idx++) {
    if( m_items[idx].old_value != 0 ) {
      free(m_items[idx].old_value);
      m_items[idx].old_value = 0;
    }
  }
}

#endif
