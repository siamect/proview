# MySQL dump 8.14
#
# Host: localhost    Database: proview
#--------------------------------------------------------
# Server version	3.23.39

#
# Table structure for table 'dbinfo'
#

CREATE TABLE dbinfo (
  row_id int(11) NOT NULL auto_increment,
  rec_ver int(11) NOT NULL default '0',
  pwr_ver int(11) NOT NULL default '0',
  PRIMARY KEY  (row_id)
) TYPE=MyISAM;

#
# Dumping data for table 'dbinfo'
#


#
# Table structure for table 'object'
#

CREATE TABLE object (
  oix int(11) unsigned NOT NULL default '0',
  ovid int(11)unsigned NOT NULL default '0',
  classid int(11) unsigned NOT NULL default '0',
  name varchar(32) NOT NULL default '',
  packname int(11) unsigned default NULL,
  normname varchar(32) default NULL,
  packnname int(11) unsigned default NULL,
  father int(11) unsigned default NULL,
  fvid int(11) unsigned default NULL,
  bwsoix int(11) unsigned default NULL,
  bwsvid int(11) unsigned default NULL,
  fwsoix int(11) unsigned default NULL,
  fwsvid int(11) unsigned default NULL,
  rh_gen int(11) unsigned default NULL,
  dh_gen int(11) unsigned default NULL,
  rb_gen int(11) unsigned default NULL,
  db_gen int(11) unsigned default NULL,
  rb_siz int(11) unsigned default NULL,
  db_siz int(11) unsigned default NULL,
  flags int(11) unsigned default NULL,
  rb_body blob,
  db_body blob,
  PRIMARY KEY  (oix,ovid),
  KEY dbkey (oix,ovid)
) TYPE=MyISAM;

#
# Dumping data for table 'object'
#


#
# Table structure for table 'volume'
#

CREATE TABLE volume (
  vid int(11) unsigned NOT NULL default '0',
  oix int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (vid),
  KEY dbkey (oix,vid)
) TYPE=MyISAM;

#
# Dumping data for table 'volume'
#


