!  op_note.sql -- Operator notes database
!
!  PROVIEW/R
!
!  This is the definition file to the Notes database used in the operator
!  environment of PROVIEW/R.
! 
!  The database contains the following tables:
!
!  Resumes	    -	    Contains note on a supervisor object.	
!     
!  Create the database file
! 
create database filename pwr_db:op_note;
!
create table resumes (vid integer, oix integer, resume list of byte varying);
!
!
! Create indexe
!
create unique index
    resume_idx
    on resumes (vid, oix)
    type is sorted
    ;
!
commit;
!
disconnect current;
!
attach 'alias op_note filename pwr_db:op_note';
grant all on database alias op_note to public;
commit;
!
exit
