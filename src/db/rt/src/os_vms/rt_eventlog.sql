!  rt_eventlog.sql -- Historical Event Log
!
!  PROVIEW/R
!  Copyright © Comator Process AB 1994.
!
!  This is the definitionfile to the Historical Event Log
! 
!  The database contains the following tables:
!
!   Event		Contains the the events
!
!
     
!
! Create the database file
! 
create database filename pwr_db:rt_eventlog
  number of buffers is 50
  snapshot is enabled deferred

create storage area area_Event
  filename pwr_db:rt_eventlog_events
  allocation is 100 pages
  ;
!
!
! Create domains for the tables
!
!
! Create the Event table
!
create table Event
  (
  StoreIdx	date,
  BirthTime	date,
  Nid		integer,
  Idx		integer,
  EventTime	date,
  DetectTime	date,
  ObjectOix	integer,
  ObjectVid	integer,
  SupObjectOix	integer,
  SupObjectVid	integer,
  EventFlags	integer,
  EventName	varchar(80),
  EventType	integer,
  TargetIdx	integer,
  OutunitOix	integer,
  OutunitVid	integer,
  EventText	varchar(80),
  EventPrio	integer,
  Status	integer,
  SupType	integer,
  AValue	real,
  DValue	integer,
  CtrlLimit	real,
  Hysteres	real,
  High		integer,
  Unit		varchar(16),
  Missing	integer
  );
!
! Create the EventTime index on table Event
!
! This index is used to:
! 1)  facilitate queries sorted by eventtime
!
create index EventTime on Event
  (
  EventTime
  )
  type is sorted
  ;  

create index EventTimeDesc on Event
  (
  EventTime DESC
  )
  type is sorted
  ;  
!
! Create the EventId index on table Event
!
! This index is used to:
! 1)  guaranty unique events
!
! 2)  to find the latest event from a particular node
!
create unique index EventId on Event
  (
  Nid,
  BirthTime,
  Idx
  )
  type is sorted
  ;
! Create the Store index on table Event
!
! This index is used to:
! 1)  to find the oldest events, for deletion
!
create unique index Store on Event
  (
  StoreIdx
  )
  type is sorted
  ;
!
! Create storage map for the Event table
!
create storage map map_Event for Event
  store in area_Event
  ;
!
! Commit
!
commit;
!
disconnect current;
!
attach 'alias heldb filename pwr_db:rt_eventlog';
grant all on database alias heldb to public;
commit;
!
exit
