!  wbdb.sql -- work bench database
!
!  PROVIEW/R
!  Copyright (C) 1996 by Comator Process AB.
!
!  This is the definitionfile to the Workbech database used in the develop-
!  ment environment of PROVIEW/R.
! 
!  The database contains the following tables:
!
!  Object	-	Contains the object headers
!

!     
!  Create the database file
! 
create database
    filename pwr_db:wb
    dbkey scope is attach
    number of buffers is 50
    snapshot is enabled deferred

create storage area wb_sys
    filename pwr_db:wb_sys
    allocation is 200 pages

create storage area wb_head
    filename pwr_db:wb_head
    allocation is 1000 pages

create storage area wb_body
    filename pwr_db:wb_body
    allocation is 2000 pages
    ;
!
!  Create domains for the tables
!
create domain  pwr_tObjname  varchar(32);
comment on domain pwr_tObjname is 'Object Name';
!
create domain pwr_tVolumeId integer;
comment on domain pwr_tVolumeId is 'Volume identity';
!
create domain pwr_tOix integer;
comment on domain pwr_tOix is 'Object index within volume';
!
create domain pwr_tClassId integer;
comment on domain pwr_tClassId is 'Class identity';
!
!  Create the tables
!
create table object
    (
    oix		pwr_tOix,	! Object index
    ovid	pwr_tVolumeId,	! Volume identity
    classid	pwr_tClassId,	! ClassId of class of object
    name	pwr_tObjname,	! Object Name
    packname	integer,	! Packed Object Name
    normname	pwr_tObjname,	! Normalized Object Name
    packnname	integer,	! Packed Normalized Object Name
    father	pwr_tOix,	! Oix of father of object
    fvid	pwr_tVolumeId,	! Volume identity
    bwsoix	pwr_tOix,	! Oix of backward sibling of object
    bwsvid	pwr_tVolumeId,	! Volume identity of backward sibling
    fwsoix	pwr_tOix,	! Oix of forward sibling of object
    fwsvid	pwr_tVolumeId,	! Volume identity of forward sibling
    rh_gen	integer,	! Object runtime environment generation
    dh_gen	integer,	! Object development environmnet generation
    rb_gen	integer,	! Generation of runtime body
    db_gen	integer,	! Generation of development body
    rb_siz	integer,	! Size of runtime body
    db_siz	integer,	! Size of development body
    flags	integer,	! Object flags
    rb_body	list of byte varying,
    db_body	list of byte varying
    );

create table dbinfo
    (
    row_id	integer,	! Identification of this row
    rec_ver	integer,	! Version of the structure mapping this table
    pwr_ver	integer		! Version of Proview/R that created this database
    );

create table volume
    (
    vid		pwr_tVolumeId,	! Volume identity
    oix		pwr_tOix	! Object index
    );
!
! Create indexes to the tables
!
create unique index
    object_idx
    on object (ovid, oix)
    type is sorted
    ;
  
create unique index
    volume_idx
    on volume (vid)
    type is sorted
    ;
  
create storage map sys_dbinfo for dbinfo
    store in wb_sys
    ;

create storage map head_map for object
    store in wb_head
    ;

create storage map sys_volume for volume
    store in wb_sys
    ;

create storage map body_map
    store lists in wb_body for (object)
    in rdb$system
    ;
!
! Commit
!
commit;
!
disconnect current;
!
attach 'alias wb filename pwr_db:wb';
grant all on database alias wb to public;
commit;
!
exit
