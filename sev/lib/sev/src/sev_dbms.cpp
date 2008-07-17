/* 
 * Proview   $Id: sev_dbms.cpp,v 1.1 2008-07-17 11:18:31 claes Exp $
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

#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "pwr.h"
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
  
  m_host = (char *)realloc(m_host, strlen(host));
  strcpy(m_host, host);
}

void sev_dbms_env::user(const char *user)
{
  if (!user)
    return;
  
  m_user = (char *)realloc(m_user, strlen(user));
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
  
  m_dbName = (char *)realloc(m_dbName, strlen(dbName));
  strcpy(m_dbName, dbName);
}

void sev_dbms_env::fileName(const char *fileName)
{
  if (!fileName)
    return;
  
  m_fileName = (char *)realloc(m_fileName, strlen(fileName));
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
  
  m_socket = (char *)realloc(m_socket, strlen(socket));
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
	   "vtype int unsigned,"
	   "vsize int unsigned,"
	   "description varchar(80),"
	   "unit varchar(16),"
	   "scantime float);");

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
  char	fname[120];
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
			    unsigned int size)
{
  char query[200];

  sprintf( query, "create table %s ( id int unsigned not null primary key auto_increment,"
	   "time datetime not null, value %s);", 
	   oid_to_table(oid, aname), pwrtype_to_type( type, size));

  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Create table: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }
  return 1;
}

int sev_dbms::store_item( pwr_tStatus *sts, char *tablename, pwr_tOid oid, char *oname, 
			  char *aname, pwr_tDeltaTime storagetime, pwr_eType vtype, 
			  unsigned int vsize, char *description, char *unit, pwr_tFloat32 scantime)
{
  char query[800];
  char timestr[40];
  pwr_tTime cretime;

  clock_gettime( CLOCK_REALTIME, &cretime);
  time_AtoAscii( &cretime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  sprintf( query, "insert into items (id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,vtype,vsize,description,unit,scantime) "
	   "values (0,'%s',%d,%d,'%s','%s','%s','%s',%d,%d,%d,'%s','%s',%f);",
	   tablename, oid.vid, oid.oix, oname, aname, timestr, timestr, storagetime.tv_sec, vtype, 
	   vsize, description, unit, scantime);
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Store item: %s\n", mysql_error(m_env->con()));
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

  sprintf( query, "select id,tablename,vid,oix,oname,aname,uptime,cretime,storagetime,vtype,vsize,description,unit,scantime "
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
    strncpy( item.aname, row[5], sizeof(item.aname));
    timestr_to_time( row[6], &item.uptime);
    timestr_to_time( row[7], &item.cretime);
    item.storagetime.tv_sec = strtoul( row[8], 0, 10);
    item.storagetime.tv_nsec = 0;
    item.vtype = (pwr_eType) strtoul( row[9], 0, 10);
    item.vsize = strtoul( row[10], 0, 10);
    strncpy( item.description, row[11], sizeof(item.description));
    strncpy( item.unit, row[12], sizeof(item.unit));
    item.scantime = atof(row[13]);

    m_items.push_back( item);
  }
  mysql_free_result( result);
  
  for ( int i = 0; i < (int)m_items.size(); i++)
    printf( "Item: %d %s\n", m_items[i].id, m_items[i].tablename);

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::store_value( pwr_tStatus *sts, pwr_tOid oid, char *aname,
			   pwr_eType type, pwr_tTime time, void *buf, unsigned int size)
{
  char query[200];
  char bufstr[512];
  char timstr[40];

  *sts = time_AtoAscii( &time, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
  if ( EVEN(*sts)) return 0;
  timstr[19] = 0;
  *sts = cdh_AttrValueToString( type, buf, bufstr, sizeof(bufstr));
  if ( EVEN(*sts)) return 0;

  switch( type) {
  case pwr_eType_String:
    sprintf( query, "insert into %s (time, value) values ('%s','%s')",
	     oid_to_table(oid, aname), timstr, bufstr);
    break;
  default:
    sprintf( query, "insert into %s (time, value) values ('%s',%s)",
	     oid_to_table(oid, aname), timstr, bufstr);
  }
  int rc = mysql_query( m_env->con(), query);
  if (rc) {
    printf( "Store value: %s\n", mysql_error(m_env->con()));
    *sts = SEV__DBERROR;
    return 0;
  }

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::get_values( pwr_tStatus *sts, pwr_tOid oid, char *aname, pwr_eType type, 
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

  if ( starttime && endtime) {
    if ( div == 1)
      sprintf( query, "select time, value from %s where time >= '%s' and time <= '%s' order by time", 
	     oid_to_table(oid, aname), starttimstr, endtimstr);
    else
      sprintf( query, "select time, value from %s where id %% %d = 0 and time >= '%s' and time <= '%s' order by time", 
	     oid_to_table(oid, aname), div, starttimstr, endtimstr);
  }
  else if ( starttime) {
    if ( div == 1)
      sprintf( query, "select time, value from %s where time >= '%s' order by time", 
	       oid_to_table(oid, aname), starttimstr);
    else
      sprintf( query, "select time, value from %s where id %% %d = 0 and time >= '%s' order by time", 
	       oid_to_table(oid, aname), div, starttimstr);
  }
  else if ( endtime) {
    if ( div == 1)
      sprintf( query, "select time, value from %s where time <= '%s' order by time", 
	       oid_to_table(oid, aname), endtimstr);
    else
      sprintf( query, "select time, value from %s where id %% %d = 0 and time <= '%s' order by time", 
	       oid_to_table(oid, aname), div, endtimstr);
  }
  else {
    if ( div == 1)
      sprintf( query, "select time, value from %s order by time", 
	       oid_to_table(oid, aname));
    else
      sprintf( query, "select time, value from %s where id %% %d = 0 order by time", 
	       oid_to_table(oid, aname), div);
  }
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

  *tbuf = (pwr_tTime *) calloc( bufrows, sizeof(pwr_tTime));
  *vbuf = calloc( bufrows, size);

  int bcnt = 0;
  for ( int i = 0; i < rows; i ++) {
    if ( div > 1)
      mysql_data_seek( result, i);

    row = mysql_fetch_row( result);
    if (!row) break;


    timestr_to_time( row[0], &(*tbuf)[bcnt]);
    cdh_StringToAttrValue( type, row[1], ((char *)*vbuf)+ bcnt * size);

    bcnt++;
    printf( "%5d %5d %s %s\n", i, bcnt, row[0], row[1]);
  }
  printf( "bcnt %d bufrows %d\n", bcnt, bufrows);
  *bsize = bcnt;
  mysql_free_result( result);

  *sts = SEV__SUCCESS;
  return 1;
}


int sev_dbms::check_item( pwr_tStatus *sts, pwr_tOid oid, char *oname, char *aname, 
			  pwr_tDeltaTime storagetime, pwr_eType type, unsigned int size, 
			  char *description, char *unit, pwr_tFloat32 scantime, unsigned int *idx)
{
  char timestr[40];
  pwr_tTime uptime;

  clock_gettime( CLOCK_REALTIME, &uptime);
  time_AtoAscii( &uptime, time_eFormat_NumDateAndTime, timestr, sizeof(timestr));
  timestr[19] = 0;

  for ( unsigned int i = 0; i < m_items.size(); i++) {
    if ( cdh_ObjidIsEqual( oid, m_items[i].oid) && 
	 cdh_NoCaseStrcmp( aname, m_items[i].aname) == 0) {
      char query[400];

      sprintf( query, "update items set ");
      if ( storagetime.tv_sec != m_items[i].storagetime.tv_sec)
	sprintf( &query[strlen(query)], "storagetime=%d,", storagetime.tv_sec);
      if ( strcmp( oname, m_items[i].oname) != 0)
	sprintf( &query[strlen(query)], "oname=\'%s\',", oname);
      if ( type != m_items[i].vtype)
	sprintf( &query[strlen(query)], "type=%d,", type);
      if ( size != m_items[i].vsize)
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
			char *description, char *unit, pwr_tFloat32 scantime, unsigned int *idx)
{
  char tablename[256];

  strcpy( tablename, oid_to_table( oid, aname));
	  
  store_item( sts, tablename, oid, oname, aname, storagetime, type, size, description, unit,
	      scantime);
  if ( EVEN(*sts)) return 0;
  
  create_table( sts, oid, aname, type, size);
  if ( EVEN(*sts)) return 0;

  sev_item item;

  item.id = 0;
  strncpy( item.tablename, tablename, sizeof(item.tablename));
  item.oid = oid;
  strncpy( item.oname, oname, sizeof(item.oname));
  strncpy( item.aname, aname, sizeof(item.aname));
  clock_gettime( CLOCK_REALTIME, &item.cretime);
  item.uptime = item.cretime;
  item.storagetime = storagetime;
  item.vtype = type;
  item.vsize = size;
  strncpy( item.description, description, sizeof(item.description));
  strncpy( item.unit, unit, sizeof(item.unit));
  item.scantime = scantime;

  m_items.push_back( item);
  *idx = m_items.size() - 1;

  *sts = SEV__SUCCESS;
  return 1;
}

int sev_dbms::delete_old_data( pwr_tStatus *sts, pwr_tOid oid, char *aname, pwr_tTime limit)
{
  char query[300];
  char timstr[40];

  *sts = time_AtoAscii( &limit, time_eFormat_NumDateAndTime, timstr, sizeof(timstr));
  if ( EVEN(*sts)) return 0;
  timstr[19] = 0;

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
