/* 
 * Proview   $Id: wb_pkg.cpp,v 1.20 2008-11-10 08:05:47 claes Exp $
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

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "wb_pkg.h"
#include "wb_error.h"
#include "wb_erep.h"

#include "cow_msgwindow.h"
#include "co_dcli.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_load.h"
#include "wb_log.h"

wb_pkg::wb_pkg( char *nodelist, bool distribute, bool config_only)
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
  if ( config_only)
    return;

  fetchFiles( distribute);
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
      pwr_tMask dstatus;
      char bootnode[80];

      if ( !(num == 5 || num == 6))
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

      if ( num == 6)
	strcpy( bootnode, line_item[5]);
      else
	strcpy( bootnode, "-");
	
      if ( m_allnodes) {
	pkg_node node( line_item[1], opsys, bus, dstatus, bootnode);
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
	    m_nodelist[i].setBootnode( bootnode);
	    m_nodelist[i].setValid();
	    break;
	  }
	}
      }
    }
    else if ( strcmp( cdh_Low(line_item[0]), "appl") == 0) {
      if ( !(num == 4 || num == 5))
	throw wb_error_str("File corrupt " load_cNameDistribute);

      char severity = line_item[2][0];
      try {
	pkg_node& n = getNode( line_item[1]);
	if ( num == 4) {
	  pkg_pattern p( line_item[3], "", severity);
	  n.push_back( p);
	}
	else {
	  pkg_pattern p( line_item[3], line_item[4], severity);
	  n.push_back( p);
	}
      } catch ( wb_error &e) {
	continue;
      }
    }
    else if ( strcmp( cdh_Low(line_item[0]), "load") == 0) {
      pwr_tVolumeId *vollist;
      pwr_tString40 *volnamelist;
      int       volcount;
      char      plcname[80];
      char     	systemname[80];
      char     	systemgroup[80];
      pwr_tTime date;

      if ( !(num == 2))
	throw wb_error_str("File corrupt " load_cNameDistribute);

      try {
        pkg_node &n = getNode( line_item[1]);

	// Add ld_node file
	sprintf( fname, load_cNameNode, load_cDirectory, n.name(), n.bus());
	pkg_pattern pnode( fname, "", 'E');
	n.push_back( pnode);

	// Add bootfile
	sprintf( fname, load_cNameBoot, load_cDirectory, n.name(), n.bus());
	pkg_pattern pboot( fname, "", 'E');
	n.push_back( pboot);

	// Read bootfile, get plc and volumes
	sts = lfu_ReadBootFile( fname, &date, systemname, 
	      systemgroup, &vollist, &volnamelist, &volcount, plcname);
	if ( EVEN(sts))
	  throw wb_error_str("Bootfile is corrupt");

	// Add plc
	if ( plcname[0] != 0) {
	  pwr_tFileName dir;
	
	  sprintf( dir, "$pwrp_root/bld/%s/exe/", cdh_OpSysToStr( n.opsys()));
	  sprintf( fname, "%s%s", dir, plcname); 
	  sprintf( dir, "$pwrp_root/%s/exe/", cdh_OpSysToStr( n.opsys()));
	  pkg_pattern pplc( fname, dir, 'W');
	  n.push_back( pplc);
	}

	// Add volumes
	for ( int j = 0; j < volcount; j++) {
	  char dir[80];

	  if ( (vollist[j] >= cdh_cManufactClassVolMin && 
		vollist[j] <= cdh_cManufactClassVolMax) ||
	       (vollist[j] >= cdh_cSystemClassVolMin && 
		vollist[j] <= cdh_cSystemClassVolMax)) {
	    // Base volume, skip
	    continue;
	  }	       

	  strcpy( dir, "$pwrp_load/");
	  sprintf( fname, "%s%s.dbs", dir, cdh_Low( (char *)(volnamelist + j))); 
	  n.checkVolume( fname);

	  // Check if there are any rtt-files for Root or Sub Volumes
	  if ( vollist[j] >= cdh_cUserVolMin &&
	       vollist[j] <= cdh_cUserVolMax) {
	    cdh_uVolumeId	vid;
	    vid.pwr = vollist[j];

	    // RttCrr-file
	    strcpy( dir, "$pwrp_load/");
	    sprintf( fname, "%s" load_cNameRttCrr,
		dir, vid.v.vid_3, vid.v.vid_2, vid.v.vid_1, vid.v.vid_0);
	    pkg_pattern rttcrr( fname);
	    n.push_back( rttcrr);

	    // RttCrrObj-file
	    strcpy( dir, "$pwrp_load/");
	    sprintf( fname, "%s" load_cNameRttCrrObj,
		dir, vid.v.vid_3, vid.v.vid_2, vid.v.vid_1, vid.v.vid_0);
	    pkg_pattern rttcrrobj( fname);
	    n.push_back( rttcrrobj);

	    // RttCrrCode-file
	    strcpy( dir, "$pwrp_load/");
	    sprintf( fname, "%s" load_cNameRttCrrCode,
		dir, vid.v.vid_3, vid.v.vid_2, vid.v.vid_1, vid.v.vid_0);
	    pkg_pattern rttcrrcode( fname);
	    n.push_back( rttcrrcode);

	    // RttPlc-file
#if 0
	    strcpy( dir, "$pwrp_load/");
	    sprintf( fname, "%s" load_cNameRttPlc,
		dir, vid.v.vid_3, vid.v.vid_2, vid.v.vid_1, vid.v.vid_0);
	    pkg_pattern rttplc( fname);
	    n.push_back( rttplc);
#endif
	  }
	}

	free( volnamelist);
	free( vollist);

      } catch ( wb_error &e) {
	if  ( e.what() == string("No such node"))
	  continue;
	throw wb_error_str(e.what());
      }
    }
    else if ( strcmp( cdh_Low(line_item[0]), "boot") == 0) {
      // A Sev node, only node and bootfile
      try {
	if ( !(num == 2))
	  throw wb_error_str("File corrupt " load_cNameDistribute);

	pkg_node &n = getNode( line_item[1]);
	// Add ld_node file
	sprintf( fname, load_cNameNode, load_cDirectory, n.name(), n.bus());
	pkg_pattern pnode( fname, "", 'E');
	n.push_back( pnode);
	
	// Add bootfile
	sprintf( fname, load_cNameBoot, load_cDirectory, n.name(), n.bus());
	pkg_pattern pboot( fname, "", 'E');
	n.push_back( pboot);
      } catch ( wb_error &e) {
	if  ( e.what() == string("No such node"))
	  continue;
	throw wb_error_str(e.what());
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

void pkg_node::checkVolume( char *filename)
{
  lfu_t_volref *volref;
  int volref_cnt;
  pwr_tVid vol_vid;
  pwr_tCid vol_cid;
  pwr_tTime vol_time;
  char vol_name[80];
  bool found;
  pwr_tStatus sts;
  char fname[200];
	  
  sts = lfu_GetVolume( filename, vol_name, &vol_vid, &vol_cid, &vol_time);
  if ( EVEN(sts)) throw wb_error(sts);

  if ( vol_cid == pwr_eClass_DetachedClassVolume)
    // No check
    return;

  found = false;
  for ( int i = 0; i < (int)m_volumelist.size(); i++) {
    if ( m_volumelist[i].m_vid == vol_vid) {
      found = true;
      if ( m_volumelist[i].m_time.tv_sec != vol_time.tv_sec) {
	char msg[200];
	sprintf( msg, "Version mismatch volume %s in %s", (volref+i)->name, filename);
	MsgWindow::message( 'E', msg, msgw_ePop_No);
	m_errors++;
      }
      break;
    }
  }
  if ( !found) {
    pkg_volume vol( vol_name, filename, vol_vid, vol_time);
    m_volumelist.push_back( vol);
  }

  sts = lfu_GetVolRef( filename, &volref, &volref_cnt);
  if ( EVEN(sts)) throw wb_error(sts);
	  
  for ( int i = 0; i < volref_cnt; i++) {
    wb_erep::volumeNameToFilename( &sts, (volref+i)->name, fname);
    if ( EVEN(sts)) {
      char msg[200];
      sprintf( msg, "Loadfile not found: %s", (volref+i)->name);
      MsgWindow::message( 'E', msg, msgw_ePop_No);
      m_errors++;
      continue;
    }

    checkVolume( fname);

    for ( int j = 0; j < (int)m_volumelist.size(); j++) {
      if ( m_volumelist[j].m_vid == (volref+i)->vid) {
	if ( m_volumelist[j].m_time.tv_sec != (volref+i)->version.tv_sec) {
	  char msg[200];
	  sprintf( msg, "Version mismatch volume %s in %s", (volref+i)->name, filename);
	  MsgWindow::message( 'E', msg, msgw_ePop_No);
	  m_errors++;
	}
	break;
      }
    }
  }
  free( (char *)volref);
}

void pkg_node::fetchFiles( bool distribute) 
{
  char	dev[80];
  char	dir[80];
  char	file[80];
  char	type[80];
  int 	version;
  char  pack_fname[200];
  char  fname[200];

  // Add volumes to pattern
  for ( int i = 0; i < (int)m_volumelist.size(); i++) {
    if ( !m_volumelist[i].m_isSystem) {
      pkg_pattern vol( m_volumelist[i].m_filename, "$pwrp_load/", 'E');
      push_back( vol);
    }
  }

  for ( int i = 0; i < (int)m_pattern.size(); i++) 
    m_pattern[i].fetchFiles();

  // Put all files in a single list
  for ( int i = 0; i < (int)m_pattern.size(); i++) {
    for ( int j = 0; j < (int)m_pattern[i].m_filelist.size(); j++) {
      try {
	pkg_file f = m_pattern[i].m_filelist[j];

	dcli_parse_filename( f.m_source, dev, dir, file, type, &version);
	strcpy( f.m_arname, file);
	strcat( f.m_arname, type);

	// Check that this name is unic
	for (;;) {
	  bool new_name = false;
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
      } catch ( wb_error &e) {
	MsgWindow::message( 'W', e.what().c_str(), msgw_ePop_No);
	m_warnings++;
      }
    }
  }

  if ( m_errors) {
    char msg[200];
    sprintf( msg, "Distribute errors node %s: %d errors, %d warnings", m_name, m_errors, m_warnings);
    MsgWindow::message( 'E', msg, msgw_ePop_Yes);
    throw wb_error_str( msg);
  }
  else if ( m_warnings) {
    char msg[200];
    sprintf( msg, "Distribute warnings node %s: %d warnings", m_name, m_warnings);
    MsgWindow::message( 'W', msg, msgw_ePop_Yes);
  }

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
  sprintf( pkg_name, load_cNamePkg, m_name, version);

  dcli_translate_filename( pack_fname, "$pwrp_tmp/pkg_pack.sh");
  ofstream of( pack_fname);

  of << 
    "if [ ! -e $pwrp_tmp/pkg_build ]; then" << endl <<
    "  mkdir $pwrp_tmp/pkg_build" << endl <<
    "else" << endl <<
    "  rm -r $pwrp_tmp/pkg_build/*.flw" << endl <<
    "  rm -r $pwrp_tmp/pkg_build/*" << endl <<
    // "  find $pwrp_tmp/pkg_build -name \"*\" | xargs rm -r" << endl <<
    "fi" << endl;
  
  for ( int i = 0; i < (int)m_filelist.size(); i++)
    of << 
      "cp " <<  m_filelist[i].m_source << " $pwrp_tmp/pkg_build/" << m_filelist[i].m_arname << endl;

  of <<
    "#mv $pwrp_tmp/pkg_unpack.sh $pwrp_tmp/pkg_build" << endl <<
    "cd $pwrp_tmp" << endl <<
    "tar -czf $pwrp_load/" << pkg_name << " pwr_pkg.dat pkg_unpack.sh pkg_build" << endl;

#if 0
  if ( distribute)
    of <<
      "cd $pwrp_load" << endl <<
      "ftp -vin " << m_name << " << EOF &>$pwrp_tmp/ftp_" << m_name << ".log" << endl <<
      "user pwrp pwrp" << endl <<
      "binary" << endl <<
      "put " << pkg_name << endl <<
      "quit" << endl <<
      "EOF" << endl <<
      "rsh -l pwrp " << m_name << " \\$pwr_exe/pwr_pkg.sh -i " << pkg_name << endl;
#endif  
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
    "echo \"-- Move files to target directories\"" << endl <<
    "if [ ! -e /home/pwrp/.ssh ]; then" << endl <<
    "  mkdir /home/pwrp/.ssh" << endl <<
    "fi" << endl;
  
  for ( int i = 0; i < (int)m_filelist.size(); i++)
    ofu << 
      "mv /tmp/pkg_build/" <<  m_filelist[i].m_arname << " " << m_filelist[i].m_target << endl;

  ofu <<
    "mv pwr_pkg.dat $pwrp_load" << endl <<
    "rm -r /tmp/pkg_build" << endl;
  
  // Change owner to root of plc, to make modification of thread prio possible
  ofu <<
    "nname=`eval uname -n`" << endl <<
    "tst=`eval sudo -l | grep \" ALL\"`" << endl <<
    "if [ \"$tst\" != \"\" ]; then" << endl <<
    "  sudo chown root $pwrp_exe/plc_$nname_*" << endl <<
    "  sudo chmod g+w $pwrp_exe/plc_$nname_*" << endl <<
    "  sudo chmod u+s $pwrp_exe/plc_$nname_*" << endl <<
    "fi" << endl;

  // Group should not have write access to .rhosts file
  ofu <<
    "if [ \"$tst\" != \"\" ]; then" << endl <<
    "  if [ -e /home/pwrp/.rhosts ]; then" << endl <<
    "    sudo chown pwrp /home/pwrp/.rhosts" << endl <<
    "    sudo chmod g-w /home/pwrp/.rhosts" << endl <<
    "  fi" << endl <<
    "  if [ -e /home/pwrp/.ssh/authorized_keys ]; then" << endl <<
    "    sudo chown pwrp /home/pwrp/.ssh/authorized_keys" << endl <<
    "    sudo chmod g-w /home/pwrp/.ssh/authorized_keys" << endl <<
    "  fi" << endl <<
    "fi" << endl;
  
  ofu.close();

  // Create a data file with description and all installed files
  dcli_translate_filename( fname, "$pwrp_tmp/pwr_pkg.dat");
  ofstream ofd( fname);
  if ( !ofd) 
    throw wb_error_str("Unable to open file");

  pwr_tTime time;
  char time_str[32];
  time_GetTime( &time);
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
  sprintf( cmd, ". %s", pack_fname);
  system( cmd);

  wb_log::log( wlog_eCategory_CreatePackage, m_name, pkg_name);

  if ( distribute) {
    // Copy the package
    copyPackage( pkg_name);
  }

  char msg[200];
  sprintf( msg, "Distribute package for node %s", m_name);
  MsgWindow::message( 'I', msg, msgw_ePop_No);

}

void pkg_node::copyPackage( char *pkg_name)
{
  char pack_fname[200];
  char bootnodes[10][80];
  int bootnode_cnt;

  if ( strcmp( m_bootnode, "-") != 0) {
    bootnode_cnt = dcli_parse( m_bootnode, ",", "", (char *)bootnodes,
		     sizeof(bootnodes)/sizeof(bootnodes[0]),
		     sizeof(bootnodes[0]), 0);
  }    
  else {
    // No bootnodes, copy to the node itself
    strcpy( bootnodes[0], m_name);
    bootnode_cnt = 1;
  }

  for ( int i = 0; i < bootnode_cnt; i++) {
    dcli_translate_filename( pack_fname, "$pwrp_tmp/pkg_pack.sh");
    ofstream of( pack_fname);
    if ( m_dstatus & lfu_mDistrOpt_RSH) {
      // Use ftp and rsh
      of <<
	"cd $pwrp_load" << endl <<
	"ftp -vin " << bootnodes[i] << " << EOF &>$pwrp_tmp/ftp_" << bootnodes[i] << ".log" << endl <<
	"user pwrp pwrp" << endl <<
	"binary" << endl <<
	"put " << pkg_name << endl <<
	"quit" << endl <<
	"EOF" << endl <<
	"rsh -l pwrp " << bootnodes[i] << " \\$pwr_exe/pwr_pkg.sh -i " << pkg_name << endl;
    }
    else {
      // Use scp and SSH
      of <<
	"cd $pwrp_load" << endl <<
	"scp " << pkg_name << " pwrp@" << bootnodes[i] << ":" << endl <<
	"ssh pwrp@"  << bootnodes[i] << " \\$pwr_exe/pwr_pkg.sh -i " << pkg_name << endl;
    }
    of.close();

    // Execute the pack file
    char cmd[200];
    sprintf( cmd, ". %s", pack_fname);
    system( cmd);

    wb_log::log( wlog_eCategory_CopyPackage, m_name, pkg_name);
  }
}

void wb_pkg::copyPackage( char *pkg_name)
{
  char node_name[80];
  char *s;

  // Extract node name from package name
  strcpy( node_name, &pkg_name[9]);
  if ( (s = strrchr( node_name, '_')))
    *s = 0;

  // Find the node and get the bootnode(s)
  wb_pkg pkg(node_name, false, true);
  pkg_node n = pkg.getNode( node_name);
  n.copyPackage( pkg_name);
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
	try {
	  pkg_file file( found_file, file_target);
	  m_filelist.push_back( file);
	} catch ( wb_error &e) {
	  MsgWindow::message( 'W', e.what().c_str(), msgw_ePop_Yes);
	}
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
      try {
	pkg_file file( found_file, file_target);
	m_filelist.push_back( file);
      } catch ( wb_error &e) {
	MsgWindow::message( 'W', e.what().c_str(), msgw_ePop_Yes);
      }
      
      sts = dcli_search_file( m_source, found_file, DCLI_DIR_SEARCH_NEXT);
    }
    dcli_search_file( m_source, found_file, DCLI_DIR_SEARCH_END);
  }
  if ( m_filelist.size() == 0 && m_severity != 'S') {
    char msg[200];
    sprintf( msg, "No file found: %s", m_source);
    MsgWindow::message( m_severity, msg, msgw_ePop_No);
    
    if ( m_node) {
      if ( m_severity == 'E')
	m_node->incrErrors();
      else
	m_node->incrWarnings();
    }
  }
}

pkg_file::pkg_file( char *source, char *target) 
{
  struct stat info;

  int sts = stat( source, &info);
  if ( sts == -1) {
    char msg[256];
    sprintf( msg, "Source file doesn't exist: %s", source);
    throw wb_error_str(msg);
  }

  m_date.tv_sec = info.st_ctime;
  m_date.tv_nsec = 0;  
  strcpy( m_source, source);
  strcpy( m_target, target);
}






