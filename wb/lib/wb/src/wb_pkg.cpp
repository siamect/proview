#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "wb_pkg.h"
#include "wb_error.h"

#include "co_msgwindow.h"
#include "co_dcli.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_load.h"

wb_pkg::wb_pkg( char *nodelist)
{
  if ( nodelist) {
    char node_str[32][20];
    int num;
    
    cdh_ToLower( nodelist, nodelist);
    num = dcli_parse( nodelist, " 	,", "", (char *)node_str,
		     sizeof(node_str)/sizeof(node_str[0]),
		     sizeof(node_str[0]), 0);
    m_allnodes = false;
    
    for ( int i = 0; i < num; i++) {
      pkg_node n( node_str[i]);
      m_nodelist.push_back( n);
    }
  }
  else
    m_allnodes = true;

  readConfig();

  for ( int i = 0; i < (int)m_nodelist.size(); i++)
    printf( "%d %s\n", i, m_nodelist[i].name());
  
  fetchFiles();
}

void wb_pkg::readConfig()
{
  char fname[200];
  char line[200];
  char line_item[6][80];
  int num;
  int sts;

  dcli_translate_filename( fname, load_cNameDistribute);
  ifstream is( fname);

  while ( is.getline( line, sizeof(line))) {
    dcli_trim( line, line);
    if ( line[0] == '#' || line[0] == '!')
      continue;

    num = dcli_parse( line, " 	", "", (char *)line_item,
		     sizeof(line_item)/sizeof(line_item[0]),
		     sizeof(line_item[0]), 0);
    if ( !num)
      continue;

    if ( strcmp( cdh_Low(line_item[0]), "node") == 0) {
      pwr_mOpSys opsys;
      int bus;
      lfu_eDistrSts dstatus;

      if ( num != 5)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      sts = sscanf( line_item[2], "%d", (int *)&opsys);
      if ( sts != 1)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      sts = sscanf( line_item[3], "%d", &bus);
      if ( sts != 1)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      sts = sscanf( line_item[4], "%d", (int *)&dstatus);
      if ( sts != 1)
	throw wb_error_str("File corrupt " load_cNameDistribute);

      if ( m_allnodes) {
	pkg_node node( line_item[1], opsys, bus, dstatus);
	m_nodelist.push_back( node);
      }
      else {
	bool found = false;
	for ( int i = 0; i < (int) m_nodelist.size(); i++) {
	  if ( strcmp( m_nodelist[i].name(), cdh_Low(line_item[1])) == 0) {
	    found = true;
	    m_nodelist[i].setOpsys( opsys);
	    m_nodelist[i].setBus( bus);
	    m_nodelist[i].setDStatus( dstatus);
	    m_nodelist[i].setValid();
	    break;
	  }
	}
      }
    }
    else if ( strcmp( cdh_Low(line_item[0]), "appl") == 0) {
      if ( !(num == 3 || num == 4))
	throw wb_error_str("File corrupt " load_cNameDistribute);

      try {
	pkg_node& n = getNode( line_item[1]);
	if ( num == 3) {
	  pkg_pattern p( line_item[2]);
	  n.push_back( p);
	}
	else {
	  pkg_pattern p( line_item[2], line_item[3]);
	  n.push_back( p);
	}
      } catch ( wb_error &e) {
	continue;
      }
    }
  }

  is.close();

  // Check that all nodes are valid
  for ( int i = 0; i < (int) m_nodelist.size(); i++) {
    if ( !m_nodelist[i].valid())
      throw wb_error_str( "Unknown node name");
  }
}

pkg_node& wb_pkg::getNode( char *name)
{
  for ( int i = 0; i < (int) m_nodelist.size(); i++) {
    if ( strcmp( m_nodelist[i].name(), cdh_Low(name)) == 0)
      return m_nodelist[i];
  }
  throw wb_error_str("No such node");
}

void pkg_node::fetchFiles() 
{
  char	dev[80];
  char	dir[80];
  char	file[80];
  char	type[80];
  int 	version;
  char  pack_fname[200];
  char  fname[200];

  for ( int i = 0; i < (int)m_pattern.size(); i++) 
    m_pattern[i].fetchFiles();

  // Put all files in a single list
  for ( int i = 0; i < (int)m_pattern.size(); i++) {
    for ( int j = 0; j < (int)m_pattern[i].m_filelist.size(); j++) {
      pkg_file f = m_pattern[i].m_filelist[j];

      dcli_parse_filename( f.m_source, dev, dir, file, type, &version);
      strcpy( f.m_arname, file);
      strcat( f.m_arname, type);

      // Check that this name is unic
      bool new_name = false;
      for (;;) {
	for ( int k = 0; k < (int)m_filelist.size(); k++) {
	  if ( strcmp( m_filelist[k].m_arname, f.m_arname) == 0) {
	    strcat( f.m_arname, "x");
	    new_name = true;
	    break;
	  }
	}
	if ( !new_name)
	  break;
      }

      m_filelist.push_back( f);
    }
  }

  for ( int i = 0; i < (int)m_filelist.size(); i++)
    printf( "%s %s %s\n", m_filelist[i].m_source, m_filelist[i].m_target,
	    m_filelist[i].m_arname);

  // Read and increment package version
  sprintf( fname, "$pwrp_load/pkg_v_%s.dat", m_name);
  dcli_translate_filename( fname, fname);
  ifstream ifv( fname);
  if ( ifv) {
    ifv >> version;
    ifv.close();
    version++;
  }
  else
    version = 1;

  ofstream ofv( fname);
  ofv << version;
  ofv.close();

  // Create a script that copies files to build directory
  char pkg_name[80];
  sprintf( pkg_name, "pwrp_pkg_%s_%04d.tgz", m_name, version);

  dcli_translate_filename( pack_fname, "$pwrp_tmp/pkg_pack.sh");
  ofstream of( pack_fname);

  of << 
    "if [ ! -e $pwrp_tmp/pkg_build ]; then" << endl <<
    "  mkdir $pwrp_tmp/pkg_build" << endl <<
    "else" << endl <<
    "  rm -r $pwrp_tmp/pkg_build/*" << endl <<
    "fi" << endl;
  
  for ( int i = 0; i < (int)m_filelist.size(); i++)
    of << 
      "cp " <<  m_filelist[i].m_source << " $pwrp_tmp/pkg_build/" << m_filelist[i].m_arname << endl;

  of <<
    "#mv $pwrp_tmp/pkg_unpack.sh $pwrp_tmp/pkg_build" << endl <<
    "cd $pwrp_tmp" << endl <<
    "tar -czf " << pkg_name << " pwr_pkg.dat pkg_unpack.sh pkg_build" << endl <<
    "ftp -vin " << m_name << " << EOF &>$pwrp_tmp/ftp_" << m_name << ".log" << endl <<
    "user pwrp pwrp" << endl <<
    "binary" << endl <<
    "put " << pkg_name << endl <<
    "quit" << endl <<
    "EOF" << endl <<
    "rsh -l pwrp " << m_name << " \\$pwr_exe/pwr_pkg.sh -i " << pkg_name << endl;
    
  of.close();

  // Create a script that unpackes the archive and moves files to the target directories
  dcli_translate_filename( fname, "$pwrp_tmp/pkg_unpack.sh");
  ofstream ofu( fname);
  if ( !ofu) 
    throw wb_error_str("Unable to open file");
  
  ofu << 
    "cd /tmp" << endl <<
    "echo \"-- Unpack package " << pkg_name << "\"" << endl <<
    "tar -xzf /home/pwrp/" << pkg_name << endl <<
    "echo \"-- Move files to target directories\"" << endl;
  
  for ( int i = 0; i < (int)m_filelist.size(); i++)
    ofu << 
      "mv /tmp/pkg_build/" <<  m_filelist[i].m_arname << " " << m_filelist[i].m_target << endl;

  ofu <<
    "mv pwr_pkg.dat $pwrp_load" << endl <<
    "rm -r /tmp/pkg_build" << endl;
  
  ofu.close();

  // Create a data file with description and all installed files
  dcli_translate_filename( fname, "$pwrp_tmp/pwr_pkg.dat");
  ofstream ofd( fname);
  if ( !ofd) 
    throw wb_error_str("Unable to open file");

  pwr_tTime time;
  char time_str[32];
  clock_gettime(CLOCK_REALTIME, &time);
  time_AtoAscii( &time, time_eFormat_DateAndTime, time_str, sizeof(time_str));
  
  ofd << 
    "%Package: " << endl << 
    pkg_name << endl <<
    "%Brief:" << endl <<
    "Proview package " << m_name << " Version " << version << " " << time_str << endl <<
    "%Description:" << endl <<
    "Proview package:	" << pkg_name << endl <<
    "Node:		" << m_name << endl <<
    "Version:		" << version << endl <<
    "Created:		" << time_str << endl <<
    "%Files:" << endl;
  
  for ( int i = 0; i < (int)m_filelist.size(); i++) {
    char timestr[32];
    time_AtoAscii( &m_filelist[i].m_date, time_eFormat_DateAndTime,
		   timestr, sizeof(timestr));
    
    ofd << 
      m_filelist[i].m_target << " " << timestr << endl;
  }
  ofd <<
    "%End:" << endl;
  ofd.close();

  // Execute the pack file
  char cmd[200];
  sprintf( cmd, "source %s", pack_fname);
  system( cmd);

}

void pkg_pattern::fetchFiles()
{
  char found_file[200];
  int sts;
  char	dev[80];
  char	dir[80];
  char	file[80];
  char	type[80];
  int 	version;
  char  file_target[80];

  if ( strchr( m_source, ':')) {
    // Source is a search path separated by colon
    char source[10][120];
    int num;

    num = dcli_parse( m_source, ":", "", (char *)source,
		     sizeof(source)/sizeof(source[0]),
		     sizeof(source[0]), 0);

    for ( int i = 0; i < num; i++) {
      sts = dcli_search_file( source[i], found_file, DCLI_DIR_SEARCH_INIT);
      dcli_search_file( source[i], found_file, DCLI_DIR_SEARCH_END);
      if ( ODD(sts)) {
	if ( hasTarget()) {
	  dcli_parse_filename( m_target, dev, dir, file, type, &version);
	  strcpy( file_target, dev);
	  strcpy( file_target, dir);
	  if ( strcmp( file, "") == 0 && strcmp( type, "") == 0)
	    dcli_parse_filename( found_file, dev, dir, file, type, &version);
	  strcat( file_target, file);
	  strcat( file_target, type);
	}
	else {
	  dcli_parse_filename( source[0], dev, dir, file, type, &version);
	  strcpy( file_target, dev);
	  strcat( file_target, dir);
	  dcli_parse_filename( found_file, dev, dir, file, type, &version);
	  strcat( file_target, file);
	  strcat( file_target, type);
	}
	pkg_file file( found_file, file_target);
	m_filelist.push_back( file);
	break;
      }
    }
  }
  else {
    sts = dcli_search_file( m_source, found_file, DCLI_DIR_SEARCH_INIT);
    while ( ODD(sts)) {
      if ( hasTarget()) {
	dcli_parse_filename( m_target, dev, dir, file, type, &version);
	strcpy( file_target, dev);
	strcpy( file_target, dir);
	if ( strcmp( file, "") == 0 && strcmp( type, "") == 0)
	  dcli_parse_filename( found_file, dev, dir, file, type, &version);
	strcat( file_target, file);
	strcat( file_target, type);
      }
      else {
	dcli_parse_filename( m_source, dev, dir, file, type, &version);
	strcpy( file_target, dev);
	strcat( file_target, dir);
	dcli_parse_filename( found_file, dev, dir, file, type, &version);
	strcat( file_target, file);
	strcat( file_target, type);
      }
      pkg_file file( found_file, file_target);
      m_filelist.push_back( file);
      
      sts = dcli_search_file( m_source, found_file, DCLI_DIR_SEARCH_NEXT);
    }
    dcli_search_file( m_source, found_file, DCLI_DIR_SEARCH_END);
  }
}

pkg_file::pkg_file( char *source, char *target) 
{
  struct stat info;

  int sts = stat( source, &info);
  if ( sts == -1)
    throw wb_error_str("Source file doesn't exist");

  m_date.tv_sec = info.st_ctime;
  m_date.tv_nsec = 0;  
  strcpy( m_source, source);
  strcpy( m_target, target);
}






