! Create the database file
!
!
! 980224  ML 	ycode1, ycode2 removed. yoix, yvid, ybody, yattroffset added.	
!
create database filename PWRP_DB:OP_HTRE_GROUPS;
!
CREATE TABLE curve_group_rel
      ( c_group_name    varchar(40),
        c_group_no      integer,
        c_description   varchar(80),
        c_creator       varchar(8),

        c_xname         varchar(256),
        c_xmin          integer,
        c_xtmin         date,
        c_xmax          integer,
        c_xtmax         date,
        c_xstep         integer,
        c_xtstep        date,

        c_realtime      integer,
        c_xtype         integer,
        c_xtimebase     integer,

        c_xhassearchclause  integer,
        c_searchclause  varchar(80));                                      

CREATE TABLE y_parameter_rel
      ( c_group_no      integer,
        c_ypar_no       integer,
        c_yname         varchar(256),
        c_ytype         integer,
	c_yoix		integer,
	c_yvid		integer,
	c_ybody		integer,
	c_yattroffset	integer,
        c_ysource       varchar(40),
        c_yaxisno       integer,
        c_yoffset       float,
        c_ytoffset      date,
        c_yhasoffset    integer,
        c_yoffsetneg    integer,
        c_ycolor        integer,
        c_ylinetype     integer);

CREATE TABLE axis_rel
      ( c_group_no      integer,
        c_axisno        integer,
        c_axismin       float,
        c_axismax       float,
        c_axisstep      float,
        c_axisunit      varchar(8),
        c_axisprec      integer);

GRANT ALL ON TABLE CURVE_GROUP_REL TO PUBLIC;
GRANT ALL ON TABLE Y_PARAMETER_REL TO PUBLIC;
GRANT ALL ON TABLE AXIS_REL TO PUBLIC;

COMMIT;

