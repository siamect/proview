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

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_syi.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_load.h"
#include "sev_dbms.h"
#include "rt_sev_msg.h"

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

  return con;
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
	   "%s, value %s %s);",
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

  clock_gettime( CLOCK_REALTIME, &creatime);
  time_AtoAscii( &creatime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  sprintf( query, "insert into items (id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,vtype,vsize,description,unit,scantime,deadband,options) "
	   "values (0,'%s',%d,%d,'%s','%s','%s','%s',%d,%d,%d,'%s','%s',%f,%f,%d);",
	   tablename, oid.vid, oid.oix, oname, aname, timestr, timestr, storagetime.tv_sec, vtype, 
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

    row = mysql_fetch_row( result);
    if (!row) break;

    printf( "%d %s %s\n", i, row[0], row[1]);

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

    m_items.push_back( item);
  }
  mysql_free_result( result);
  
  for ( int i = 0; i < (int)m_items.size(); i++)
    printf( "Item: %d %s\n", m_items[i].id, m_items[i].tablename);

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::store_value( pwr_tStatus *sts, int item_idx, int attr_idx,
			   pwr_tTime time, void *buf, unsigned int size)
{
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
	       oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
	       m_items[item_idx].last_id);
      int rc = mysql_query( m_env->con(), query);
      if (rc)
	printf( "Update jump: %s\n", mysql_error(m_env->con()));
    }
  }
  else
    m_items[item_idx].first_storage = 0;

  

  *sts = time_AtoAscii( &time, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
  if ( EVEN(*sts)) return 0;
  timstr[19] = 0;
  *sts = cdh_AttrValueToString( m_items[item_idx].attr[attr_idx].type, buf, bufstr, sizeof(bufstr));
  if ( EVEN(*sts)) return 0;

  if ( !update_time_only) {
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_PosixTime) {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
	// Posix time, high resolution
	switch( m_items[item_idx].attr[attr_idx].type) {
	case pwr_eType_String:
	  sprintf( query, "insert into %s (time, ntime, value) values (%lu,%lu,'%s')",
		   oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		   time.tv_sec, time.tv_nsec, bufstr);
	  break;
	default:
	  if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
	    sprintf( query, "insert into %s (time, ntime, value, jump) values (%lu,%lu,%s,%d)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		     time.tv_sec, time.tv_nsec, bufstr, set_jump);
	  else
	    sprintf( query, "insert into %s (time, ntime, value) values (%lu,%lu,%s)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		     time.tv_sec, time.tv_nsec, bufstr);
	}
      }
      else {
	// Posix time, low resolution
	switch( m_items[item_idx].attr[attr_idx].type) {
	case pwr_eType_String:
	  sprintf( query, "insert into %s (time, value) values (%lu,'%s')",
		   oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), time.tv_sec, bufstr);
	  break;
	default:
	  if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
	    sprintf( query, "insert into %s (time, value, jump) values (%lu,%s,%d)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), time.tv_sec, bufstr, set_jump);
	  else
	    sprintf( query, "insert into %s (time, value) values (%lu,%s)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), time.tv_sec, bufstr);
	}
      }
    }
    else {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
	// Sql time, high resolution
	switch( m_items[item_idx].attr[attr_idx].type) {
	case pwr_eType_String:
	  sprintf( query, "insert into %s (time, ntime, value) values ('%s',%lu,'%s')",
		   oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		   timstr, time.tv_nsec, bufstr);
	  break;
	default:
	  if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
	    sprintf( query, "insert into %s (time, ntime, value, jump) values ('%s',%lu,%s,%d)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		     timstr, time.tv_nsec, bufstr, set_jump);
	  else
	    sprintf( query, "insert into %s (time, ntime, value) values ('%s',%lu,%s)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		     timstr, time.tv_nsec, bufstr);
	}
      }
      else {
	// Sql time, low resolution
	switch( m_items[item_idx].attr[attr_idx].type) {
	case pwr_eType_String:
	  sprintf( query, "insert into %s (time, value) values ('%s','%s')",
		   oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), timstr, bufstr);
	  break;
	default:
	  if ( m_items[item_idx].options & pwr_mSevOptionsMask_UseDeadBand)
	    sprintf( query, "insert into %s (time, value, jump) values ('%s',%s,%d)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), timstr, bufstr, set_jump);
	  else
	    sprintf( query, "insert into %s (time, value) values ('%s',%s)",
		     oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), timstr, bufstr);
	}
      }
    }
    int rc = mysql_query( m_env->con(), query);
    if (rc) {
      printf( "Store value: %s\n", mysql_error(m_env->con()));
      *sts = SEV__DBERROR;
      return 0;
    }
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_ReadOptimized)
      m_items[item_idx].last_id = mysql_insert_id( m_env->con());
  }
  else {
    if ( m_items[item_idx].options & pwr_mSevOptionsMask_PosixTime) {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
	// Posix time, high resolution
	sprintf( query, "update %s set time = %lu, ntime = %lu where id = %d",
		 oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		 time.tv_sec, time.tv_nsec, m_items[item_idx].last_id);
      }
      else {
	// Posix time, low resolution
	sprintf( query, "update %s set time = %lu where id = %d",
		 oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), time.tv_sec, 
		 m_items[item_idx].last_id);
      }
    }
    else {
      if ( m_items[item_idx].options & pwr_mSevOptionsMask_HighTimeResolution) {
	// Sql time, high resolution
	sprintf( query, "update %s set time = '%s', ntime = %lu where id = %d",
		 oid_to_table(m_items[item_idx].oid, m_items[item_idx].attr[attr_idx].aname), 
		 timstr, time.tv_nsec, m_items[item_idx].last_id);
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
      printf( "Update value: %s\n", mysql_error(m_env->con()));
      *sts = SEV__DBERROR;
      return 0;
    }
  }

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::get_values( pwr_tStatus *sts, pwr_tOid oid, pwr_tMask options, float deadband, 
			  char *aname, pwr_eType type, 
			  unsigned int size, pwr_tFloat32 scantime, 
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
    
    if ( time_Acomp( &create_time, &stime) == 1)
      stime = create_time;
      
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
	  sprintf( where_part, "where time >= %lu and time <= %lu", starttime->tv_sec, endtime->tv_sec);
	else
	  sprintf( where_part, "where time >= '%s' and time <= '%s'", starttimstr, endtimstr);
      }
      else {
	if ( options & pwr_mSevOptionsMask_PosixTime)
	  sprintf( where_part, "where (id %% %d = 0 %s) and time >= %lu and time <= %lu", 
		   div, jumpstr, starttime->tv_sec, endtime->tv_sec);
	else
	  sprintf( where_part, "where (id %% %d = 0 %s) and time >= '%s' and time <= '%s'", 
		   div, jumpstr, starttimstr, endtimstr);
      }
    }
    else if ( starttime) {
      if ( div == 1) {
	if ( options & pwr_mSevOptionsMask_PosixTime)
	  sprintf( where_part, "where time >= %lu", starttime->tv_sec);
	else
	  sprintf( where_part, "where time >= '%s'", starttimstr);
      }
      else {
	if ( options & pwr_mSevOptionsMask_PosixTime)
	  sprintf( where_part, "where (id %% %d = 0 %s) and time >= %lu", div, jumpstr, starttime->tv_sec);
      else
	sprintf( where_part, "where (id %% %d = 0 %s) and time >= '%s'", div, jumpstr, starttimstr);
      }
    }
    else if ( endtime) {
      if ( div == 1) {
	if ( options & pwr_mSevOptionsMask_PosixTime)
	  sprintf( where_part, "where time <= %lu", endtime->tv_sec);
	else
	  sprintf( where_part, "where time <= '%s'", endtimstr);
      }
      else {
	if ( options & pwr_mSevOptionsMask_PosixTime)
	  sprintf( where_part, "where (id %% %d = 0 %s) and time <= %lu", div, jumpstr, endtime->tv_sec);
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
	sprintf( where_part, "where time >= %lu and time <= %lu", starttime->tv_sec, endtime->tv_sec);
      else
	sprintf( where_part, "where time >= '%s' and time <= '%s'", starttimstr, endtimstr);
    }
    else if ( starttime) {
      if ( options & pwr_mSevOptionsMask_PosixTime)
	sprintf( where_part, "where time >= %lu", starttime->tv_sec);
      else
	sprintf( where_part, "where time >= '%s'", starttimstr);
    }
    else if ( endtime) {
      if ( options & pwr_mSevOptionsMask_PosixTime)
	sprintf( where_part, "where time <= %lu", endtime->tv_sec);
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
      
      cdh_StringToAttrValue( type, row[j++], ((char *)*vbuf)+ bcnt * size);
      
      bcnt++;
      if ( options & pwr_mSevOptionsMask_HighTimeResolution)
	printf( "%5d %5d %s.%9s %s\n", i, bcnt, row[0], row[1], row[2]);
      else
	printf( "%5d %5d %s %s\n", i, bcnt, row[0], row[1]);
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
      
      cdh_StringToAttrValue( type, row[j++], ((char *)*vbuf)+ bcnt * size);
      
      bcnt++;
      if ( options & pwr_mSevOptionsMask_HighTimeResolution)
	printf( "%5d %5d %s.%9s %s\n", i, bcnt, row[0], row[1], row[2]);
      else
	printf( "%5d %5d %s %s\n", i, bcnt, row[0], row[1]);
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

  clock_gettime( CLOCK_REALTIME, &uptime);
  time_AtoAscii( &uptime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  for ( unsigned int i = 0; i < m_items.size(); i++) {
    if ( m_items[i].deleted)
      continue;

    if ( cdh_ObjidIsEqual( oid, m_items[i].oid) && 
	 cdh_NoCaseStrcmp( aname, m_items[i].attr[0].aname) == 0) {
      char query[400];

      sprintf( query, "update items set ");
      if ( storagetime.tv_sec != m_items[i].storagetime.tv_sec)
	sprintf( &query[strlen(query)], "storagetime=%d,", storagetime.tv_sec);
      if ( strcmp( oname, m_items[i].oname) != 0)
	sprintf( &query[strlen(query)], "oname=\'%s\',", oname);
      if ( type != m_items[i].attr[0].type)
	sprintf( &query[strlen(query)], "type=%d,", type);
      if ( size != m_items[i].attr[0].size)
	sprintf( &query[strlen(query)], "size=%d", size);
      sprintf( &query[strlen(query)], "uptime=\'%s\' ", timestr);
      sprintf( &query[strlen(query)], "where id=%d;", m_items[i].id);	

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

  item.id = 0;
  strncpy( item.tablename, tablename, sizeof(item.tablename));
  item.oid = oid;
  strncpy( item.oname, oname, sizeof(item.oname));
  strncpy( item.attr[0].aname, aname, sizeof(item.attr[0].aname));
  clock_gettime( CLOCK_REALTIME, &item.creatime);
  item.modtime = item.creatime;
  item.storagetime = storagetime;
  item.attr[0].type = type;
  item.attr[0].size = size;
  strncpy( item.description, description, sizeof(item.description));
  strncpy( item.attr[0].unit, unit, sizeof(item.attr[0].unit));
  item.scantime = scantime;
  item.deadband = deadband;
  item.options = options;

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

  if ( options & pwr_mSevOptionsMask_PosixTime)
    sprintf( query, "delete from %s where time < %lu;",
	     oid_to_table(oid, aname), limit.tv_sec);
  else
    sprintf( query, "delete from %s where time < '%s';",
	     oid_to_table(oid, aname), timstr);

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

#endif
