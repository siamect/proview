/* 
 * Proview   $Id: rt_rtt_helptext.h,v 1.4 2008-06-24 07:39:57 claes Exp $
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
 */

#ifndef rt_rtt_helptext_h
#define rt_rtt_helptext_h

/****************************************************************/
/*								*/
/* 	Filename: 	rs_rtt_helptext.h 		       	*/
/*								*/
/*								*/
/****************************************************************/


static rtt_t_helptext	helptext[] = {
{1,	"MENU","",
"\
In an object menu picture it is possible to select an item and execute\n\
a number of functions that is specific for each item.\n\
The following keys are defined.\n\n\
Arrow keys     	Navigate in the menu and select an item.\n\
Return		Execute a function or display a child menu if there is one.\n\
Ctrl/A or PF1  	Execute some function if specified.\n\
Ctrl/T or PF2  	Execute some function if specified.\n\
Ctrl/E or PF3  	Set the value of the current item.\n\
Ctrl/R or PF4	Go back to previous menu.\n\
Page Down or Ctrl/F Go to next page of the menu.\n\
Page Up or Ctrl/D Go to previous page of the menu.\n\
Ctrl/Z		Back to root menu.\n\
Ctrl/W		Redraw the picture.\n\
Help		Help.\n\
Delete		Delete an item in the menu.\n\
Ctrl/B or DO    Command mode.\n\
Ctrl/K		Acknowledge last alarm.\n\
Ctrl/L		Show or hide description attribute.\n\
"},
{1,	"OBJECT MENU","",
"\
In an object menu picture it is possible to select an object and execute\n\
a number of functions, show children or attributes, set the value of\n\
an attribute. The following keys are defined.\n\n\
Arrow keys     	Navigate in the menu and select an object.\n\
Return		Display the children in the object hierarchy of the object.\n\
Ctrl/A or PF1  	Show the attributes of the object.\n\
Ctrl/T or PF2  	Debug the children of the object.\n\
Ctrl/E or PF3  	Set the value of a parameter.\n\
Ctrl/R or PF4	Go back to previous menu.\n\
Page Down or Ctrl/F Go to next side of the menu.\n\
Page Up or Ctrl/D Go to previous side of the menu.\n\
Ctrl/Z		Back to root menu.\n\
Ctrl/W		Redraw the picture.\n\
Help		Help.\n\
Delete		Delete an item in the menu.\n\
Ctrl/B or DO	Command mode.\n\
Ctrl/K		Acknowledge last alarm.\n\
Ctrl/L		Show or hide description attribute.\n\
Ctrl/V		Insert selected object in collection picture.\n\
Ctrl/N		Show collection picture.\n\
"},
{1,	"COMMAND","",
"\
The command prompt is reached by the 'DO' key or CTRL/B\n\
You exit command mode by PF4\n\n\
add debug /name= /class= /hierarchy=\n\
add parameter/parameter= /name= /class= /hierarchy=\n\
alarm send /text= /priority=\n\
alarm show\n\
alarm list\n\
alarm print /file=\n\
classhier\n\
collect /name=\n\
collect clear\n\
collect show\n\
create object/name= /class=\n\
crossreference /name=\n\
debug object /name= /class= /hierarchy=\n\
debug children /name=\n\
delete object /name=\n\
exit\n\
learn start/file=\n\
learn stop\n\
logging...\n\
login\n\
help\n\
page\n\
plcscan /on /off /hierarchy= /all\n\
print /file= /tsize= /psize= /append /text=\n\
quit\n\
say /text=\n\
search 'string'\n\
set conversion /name= /on /off\n\
set description\n\
set dotest /name= /on /off\n\
set invert /name= /on /off\n\
set parameter /name= /value=\n\
set priority /priority=\n\
set pwrp_alias\n\
set testvalue /name= /on /off\n\
set scan /time=\n\
show alarm\n\
show class /name= /volume=\n\
show error\n\
show event\n\
show file\n\
show grafcet\n\
show hierarchy\n\
show logging\n\
show nodes\n\
show nmps\n\
show object /objdid=\n\
show object /name= /class= /hierarchy=\n\
show object /name= /type= /file=\n\
show parameter/parameter= /name= /class= / hierarchy=\n\
show pid\n\
show plcpgm\n\
show remnodes\n\
show remtrans\n\
show signals\n\
show system\n\
show subcli\n\
show subsrv\n\
show user\n\
show device\n\
store /file= /collect\n\
top\n\
view /file=\n\
wait /time=\n\
"},
{1,	"SAY","",
"\
Print a text on sys$output.\n\
This command is for rtt command files to notify the user of what is\n\
going on.\n\n\
pwr_rtt> say /text=\n\
"},
{1,	"WAIT","",
"\
Wait a specified time or until the plc program is started.\n\
This command is for rtt command files.\n\n\
pwr_rtt> wait /plcpgm\n\
pwr_rtt> wait /time=\n\n\
Time in seconds.\n\
"},
{1,	"SHOW","",
"\
Display information about the system\n\
Allowed qualifiers are:	\n\n\
pwr_rtt> show object /objdid=\n\
pwr_rtt> show object /name= /class= /hierarchy=\n\
pwr_rtt> show object /name= /type= /file= \n\
pwr_rtt> show parameter/parameter= /name= /class= /hierarchy=\n\
pwr_rtt> show hierarchy\n\
pwr_rtt> show class /name= /volume=\n\
pwr_rtt> show signals /name=\n\
pwr_rtt> show conversion [/name=]\n\
pwr_rtt> show invert [/name=]\n\
pwr_rtt> show system\n\
pwr_rtt> show nodes\n\
pwr_rtt> show subcli\n\
pwr_rtt> show subsrv\n\
pwr_rtt> show error\n\
pwr_rtt> show plcpgm\n\
pwr_rtt> show grafcet\n\
pwr_rtt> show device\n\
pwr_rtt> show plcthread\n\
pwr_rtt> show pid\n\
pwr_rtt> show logging\n\
pwr_rtt> show nmps\n\
pwr_rtt> show remnodes\n\
pwr_rtt> show remtrans\n\
pwr_rtt> show time\n\
pwr_rtt> show clock\n\
pwr_rtt> show alarm\n\
pwr_rtt> show event\n\
pwr_rtt> show symbol\n\
pwr_rtt> show user\n\
"},
#ifdef OS_ELN
{1,	"SHOW SYSTEM","",
"\
Display information about the jobs an processes of the system.\n\
The parameters shown is:\n\
- job name.\n\
- job number.\n\
- job priority.\n\
- state.\n\
- cpu time since boot.\n\
- current cpu.\n\
- mean cpu since the picture was started.\n\n\
By selecting a job and pressing PF1 more information about the job will be\n\
displayed:\n\n\
- job name.\n\
- file name.\n\
- options.\n\
- peak cpu since the picture was started.\n\
- kernel stack.\n\
- mode.\n\
- the processes of the job.\n\
For eatch process is displayed:\n\
- process name.\n\
- process number.\n\
- process priority.\n\
- process state.\n\
- process cputime.\n\
- current cpu.\n\
- mean cpu since the picture was started.\n\
"},
#endif
#ifdef OS_VMS
{1,	"SHOW SYSTEM","",
"\
Display information about the proview processes of the system.\n\
"},
#endif
{1,	"SHOW NODES","",
"\
Display the node known by the nethandler.\n\
Data for the node is:\n\
- node name.\n\
- PAMS/DMQ group number.\n\
- proview node number.\n\
- link (Up: nethandler link, Istrt: PAMS/DMQ link, Down: no link)\n\
- upcount\n\
- Time up. Time since nethanler link was up.\n\
- Sent. Messages sent.\n\
- Rcvd. Messages received.\n\
"},
{1,	"SHOW SUBCLI","",
"\
Display the subscriptions served by other systems.\n\
"},
{1,	"SHOW SUBSRV","",
"\
Display the served subscriptions.\n\
"},
{1,	"SHOW ERROR","",
"\
Display devices that has an errorcounter greater than zero.\n\
"},
{1,	"SHOW PLCPGM","",
"\
Display the plcpgm's of the system.\n\n\
Supported actions:\n\n\
RETURN	Go down in the object hierarchy of the plcpgm.\n\
PF1	Show the signals referenced by the plcpgm code.\n\
PF2	Debug the plcpgm.\n\
"},
{1,	"SHOW GRAFCET","",
"\
Display the plcpgm's containing grafcet sequences.\n\
If the text 'Active' is displayed in front of the plcpgm name the grafcet\n\
sequence activity is not in the InitStep.\n\
To monitor grafcet sequences do this:\n\n\
- mark the plcpgm by positioning the cursor on the plcpgm and press return.\n\
  four plcpgm's can be monitored at the same time.\n\
- Then press PF2 to start the grafcet monitor.\n\n\
The grafcet monitor shows the active steps and the active orders of the\n\
grafcet sequence. (By giving the orders proper names it will be easier to\n\
examine the sequence from the monitor).\n\
"},
{1,	"SHOW DEVICE","",
"\
Display the device of the system.\n\
"},
{1,	"SHOW PLCTHREAD","",
"\
Display the PlcThread objects of the system.\n\
The PlcThread object contains statistic.\n\
The following attributes of the PlcThread objects is displayed:\n\
- Object name.\n\
- Count		Loopcount for the thread.\n\
- Scantime	Configured scantime for the thread.\n\
- Mean Scantime	Mean scantime.\n\
- Coverage	Ratio between start and stop of execution of the thread, and\n\
		scantime. The value is in percent.\n\
- Count_1_8	Number of loops where the coverage is less the 1/8\n\
- Count_1_4	Number of loops where the coverage is less the 1/4\n\
"},
{1,	"SHOW PID","",
"\
Display the pid objects of the system.\n\n\
Select a pid object and press RETURN to open the pid object picture\n\
of the pid.\n\
"},
{1,	"SHOW LOGGING","",
"\
Show logging displays the logging entries, and allows you to\n\
set parameters for the logging function, start and stop the logging.\n\
Do this steps to create a logging\n\
- put the attributes you want to log in the collections picture. Max number\n\
  of attributes in eatch entry is 100. Only the 10 first is show in the\n\
  logging picture. If you want to see all, use the 'More' button.\n\
- display the collection picture at least once\n\
- enter the logging picture\n\
- replace the filename if you aren't satisfied with the default name.\n\
- choose logg type (EVENT or CONT).\n\
- select 'INSERT' and press PF1. The attributes of the collection picture will\n\
  now be diplayes in the picture.\n\
- select 'START' and press PF1. The logging will now start.\n\
- select 'STOP' when it's time to terminate the logging.\n\
- select 'SHOW FILE' to examine the loggfile.\n\
- the logging setup can be stored on disk with the 'Store' button, and can\n\
  be restored later with the 'Re' button.\n\
- if another logging is to be don simultaneously, select another logging entry\n\
  with 'Next' or 'Previous' and start the logging in this entry.\n\n\
See the logging function for more information\n\
"},
{1,	"SHOW NMPS","",
"\
Display the content of the NMps cells in the system.\n\
The following data are shown in the picture:\n\
- Name of the cell.\n\
- Fu: Function of the cell.\n\
- Si: Size. Max number of data objects.\n\
- To: Current number of data objects in the cell.\n\
- Nu: Index of the displayed data object.\n\
- Last name segment of the data object.\n\n\
Select a cell or a data object with the arrow keys.\n\
RETURN diplays the object picture of the selected cell.\n\
Ctrl/A or PF1 shows the selected data object.\n\
Ctrl/T or PF2 expands/collapses the cell. Show all or the first data objects of.\n\
 the selected cell\n\n\
In the cell object picture you can examine the content of the cell and\n\
set attributes.\n\
"},
{1,	"SHOW REMNODE","",
"\
Display the RemNode objects.\n\n\
The following data are shown in the picture:\n\
- Name of the RemNode.\n\
- Transport type.\n\
- Description.\n\
If the 'Change Mode' button i activated (PF1) the Address will be displayed.\n\
"},
{1,	"SHOW REMTRANS","",
"\
Display the RemTrans objects.\n\n\
The following data are shown in the picture:\n\
- Name of the RemTrans.\n\
- DataValid.\n\
- Direction (send or receive).\n\
- TransCount, number of transaktions.\n\
- TransTime, the time of the last transaktion.\n\
- ErrorCount, number of errors.\n\
- Status, status of the last transaktion.\n\
"},
{1,	"SHOW OBJECT","",
"\
Display information about objects in the database\n\n\
pwr_rtt> show object /objdid= \n\
pwr_rtt> show object /name= /class= / hierarchy= /global /maxobjects=\n\
pwr_rtt> show object /name= /type= /file= \n\n\n\
Show an object with a specified objid.\n\
pwr_rtt> show object /objdid= \n\n\
/objid=		displays the object with the specified objid.\n\
		Objid should be specified on the format 'A.B.C.D:E'.\n\n\n\
Show objects that fits in the class, name and hierarchy description.\n\
pwr_rtt> show object /name= /class= / hierarchy= /global /maxobjects=\n\n\
/name=		the name of the object. Wildcard is allowed.\n\
		The name can be written without '/name='.\n\
		Ex  pwr_rtt> show object *di*\n\
/class=		displays objects of this class\n\
/hierarchy=	displays objects below this object in the hierarchy.\n\
		If no object is given the hierarchy of the current picture\n\
		is default (show object /hierarchy).\n\
/global		Search is also perfomed in mounted volumes of remote nodes.\n\
/maxobjects	Max number of objects that will be shown. Default is 300.\n\n\n\
Show an object and interpret the content as a struct specified in a c-includefile.\n\
pwr_rtt> show object /name= /type= /file= \n\n\
/name=		the name of the object.\n\
/type=	the name of the struct.\n\
/file=		the name of c-includefile where the struct is defined.\n\
"},
{1,	"SHOW PARAMETER","",
"\
Display the value of a parameter of one or many objects\n\
Object that fits in the class, name and hierarchy description are displayed\n\
on the screen.\n\n\
pwr_rtt> \
show parameter /parameter= /name= /class= /hierarchy= /global /maxobjects=\n\n\
/parameter=	the name of the parameter to be displayed\n\
/name=		the name of the object. Wildcard is allowed.\n\
		The name can be written without '/name='. Ex show object *di*\n\
/class=		displays objects of this class\n\
/hierarchy=	displays objects below this object in the hierarchy.\n\
		If no object is given the hierarchy of the current picture\n\
		is default (show object /hierarchy).\n\
/global		Search is also perfomed in mounted volumes of remote nodes.\n\
/maxobjects	Max number of parameters that will be shown. Default is 300.\n\
"},
{1,	"SHOW CLASS","",
"\
Show class objects.\n\
Display one class (/name) or all classes in a classvolume (/volume).\n\n\
pwr_rtt> show class /name= \n\
pwr_rtt> show class /volume= \n\n\
/name=		name of a class (without pathname).\n\
/volume=	name of a classvolume.\n\
"},
{1,	"SHOW HIERARCHY","",
"\
Display the root objects in the object hierarchy.\n\
"},
{1,	"SHOW SIGNALS","",
"\
Display the signals referenced in a plcpgm or a window.\n\
Allowed qualifiers are:	\n\n\
pwr_rtt> show signals /name= /file=\n\n\
/name		the name of a plcpgm or a window object. Wildcard is allowed.\n\
		If the object is a plcpgm all the signals in the plcpgm are\n\
		displayed, if a window all the signals in the window are\n\
		displayed.\n\
		Default value is the selected object.\n\
/file		Name of the listfile of the plcmodule list.\n\
		Default pwrp_lis:pwrp_plcmodulelist.plis.\n\n\
Example		pwr_rtt> show signals\n\
"},
{1,	"SHOW STEP","",
"\
Display the active steps on the current node.\n\
Displays all active steps or active steps below an specified object.\n\
Allowed qualifiers are:	\n\n\
pwr_rtt> show step /hierarchy= /initstep\n\n\
/hierarchy=	displays active steps below this object in the hierarchy.\n\
		If no object is given the selected object or the hierarchy\n\
		of the current picture is default (show step /hierarchy).\n\
/initstep	The steps and the initsteps are shown. Otherwise only the\n\
		steps are shown.\n\n\
Example		pwr_rtt> show step\n\
"},
{1,	"SHOW CONVERSION","",
"\
Display io conversion of a input channel.\n\
The channel can be specified by name (/name qualifier) or by selecting\n\
the channel, or the connected signal.\n\n\
pwr_rtt> show conversion [/name=]\n\n\
/name		Name of a channel object, or the connected signal object.\n\
"},
{1,	"SHOW INVERT","",
"\
Display io invert of a channel.\n\
The channel can be specified by name (/name qualifier) or by selecting\n\
the channel, or the connected signal.\n\n\
pwr_rtt> show invert [/name=]\n\n\
/name		Name of a channel object, or the connected signal object.\n\
"},
{1,	"SHOW CLOCK","",
"\
Show the current time of the hardware clock.\n\
Implemented in VAXELN only.\n\n\
pwr_rtt> show clock\n\n\n\
Related subjects: set clock, get clock.\n\
"},
{1,	"SHOW FILE","",
"\
Show files.\n\
If no filespecification is given, the rtt commandfiles in the default\n\
directory is shown.\n\n\
To show a picture or execute a commandfile, select the picture or file\n\
and press RETURN.\n\n\
pwr_rtt> show file\n\
pwr_rtt> show file *sto*.rtt_com\n\
"},
{1,	"SHOW ALARM","",
"\
Show the alarm list.\n\
The same function as 'alarm show'. No qualifier is allowed.\n\n\
pwr_rtt> show alarm\n\
"},
{1,	"SHOW EVENT","",
"\
Show the event list.\n\
The same function as 'alarm list'. No qualifier is allowed.\n\n\
pwr_rtt> show event\n\
"},
{1,	"SHOW SYMBOL","",
"\
Show the value of a symbol or show all the defined symbols.\n\n\
pwr_rtt> show symbol 'symbol_name'\n\
pwr_rtt> show symbol [/all]\n\
"},
{1,	"SHOW USER","",
"\
Show the current user with name and privileges.\n\n\
pwr_rtt> show user\n\
"},
{1,	"MONITOR GRAFCET","",
"\
Display the active steps and orders of a grafcet sequence.\n\n\
Select a PlcPgm object that contains a grafcet sequence. The name of the\n\
active steps and orders will be displayed continously. The monitor can\n\
also be started from the 'SHOW GRAFCET' picture.\n\n\
pwr_rtt> monitor grafcet\n\
"},
{0,	"FILE LIST",
"       RETURN execute a file                                    Ctrl/R back",
"\
List of files.\n\n\
A rtt commandfile (with the extention .rtt_com) can be executed by selecting\n\
the commandfile and pressing RETURN.\n\n\
A text file (with the extention .txt) can be displayed by selecting\n\
the commandfile and pressing RETURN.\n\
"},
{1,	"CROSSREFERENCE","",
"\
Display the crossreferences of:\n\
- signals.\n\
	pwr_rtt> crossref [/name=]\n\n\
- other object referenced in GetDp, GetData, StoDp etc.\n\
	pwr_rtt> crossref [/name=]\n\n\
- functions or classes used in CArtithm, DataArithm, Aarithm or DArithm objects.\n\
	pwr_rtt> crossref /function= [/case_sensitive] [/brief]\n\n\
- search for some string used in in CArtithm, DataArithm, Aarithm or DArithm.\n\
	pwr_rtt> crossref /string= [/case_sensitive] [/brief]\n\n\
This command demands that the rtt crossreference lists for the volume has been\n\
created from Utilities or pwr_cmd by the 'create rttfiles' command.\n\n\
"},
{1,	"DEBUG","",
"\
Display the value of the debug parameter of one or many objects.\n\n\
pwr_rtt> debug object /name= /class= /hierarchy =\n\
pwr_rtt> debug children /name=\n\
pwr_rtt> debug signals /name=\n\
"},
{1,	"DEBUG SIGNALS","",
"\
Display the signals and the value of the signalsreferenced in a plcpgm\n\
or a window.\n\
Allowed qualifiers are:	\n\n\
pwr_rtt> debug signals /name= /file=\n\n\
/name		the name of a plcpgm or a window object. Wildcard is allowed.\n\
		If the object is a plcpgm all the signals in the plcpgm are\n\
		displayed, if a window all the signals in the window are\n\
		displayed.\n\
		Default value is the selected object.\n\
/file		Name of the listfile of the plcmodule list.\n\
		Default pwrp_lis:pwrp_plcmodulelist.plis.\n\n\
Example		pwr_rtt> debug signals\n\
"},
{1,	"DEBUG OBJECT","",
"\
Display the value of the debug parameter of one or many objects\n\
Object that fits in the class, name and hierarchy description are displayed\n\
on the screen. Only objects with a defined debugparameter is displayed.\n\n\
pwr_rtt> debug object /name= /class= /hierarchy= /global\n\n\
/name=		the name of the object. Wildcard is allowed.\n\
		The name can be written without '/name='. Ex debug object *di*\n\
/class=		displays objects of this class\n\
/hierarchy=	displays objects below this object in the hierarchy.\n\
		If no object is given the hierarchy of the current picture\n\
		is default (show object /hierarchy).\n\
/global		Search is also perfomed in mounted volumes of remote nodes.\n\
"},
{1,	"DEBUG CHILDREN","",
"\
Display the value of the debug parameter of the children of an object.\n\
Only objects with a defined debugparameter is displayed.\n\n\
pwr_rtt> debug children /name=\n\n\
/name=		the name of the object.\n\
		The name can be written without '/name='.\n\
"},
{1,	"ADD","",
"\
This command adds objects to the current picture.\n\
Allowed qualifiers are:	\n\n\
pwr_rtt> add debug /name=nnn /class=ccc / hierarchy = hhh\n\
pwr_rtt> add parameter/parameter=ppp /name=nnn /class=ccc / hierarchy = hhh\n\
"},
{1,	"ADD PARAMETER","",
"\
This command adds objects to the current picture, and displays the value\n\
of the parameter of the objects.\n\
Object that fits in the class, name and hierarchy description are displayed\n\
on the screen.\n\n\
pwr_rtt> \
add parameter /parameter= /name= /class= /hierarchy= \n\n\
/parameter=	the name of the parameter to be displayed\n\
/name=		the name of the object. Wildcard is allowed.\n\
		The name can be written without '/name='. Ex show object *di*\n\
/class=		displays objects of this class\n\
/hierarchy=	displays objects below this object in the hierarchy.\n\
		If no object is given the hierarchy of the current picture\n\
		is default (show object /hierarchy).\n\
"},
{1,	"ADD DEBUG","",
"\
This command adds objects to the current picture, and displays the value\n\
of the debug parameter of the objects\n\
Object that fits in the class, name and hierarchy description are displayed\n\
on the screen. Only objects with a defined debugparameter is displayed.\n\n\
pwr_rtt> debug object /name= /class= /hierarchy= /global\n\n\
/name=		the name of the object. Wildcard is allowed.\n\
		The name can be written without '/name='. Ex show object *di*\n\
/class=		displays objects of this class\n\
/hierarchy=	displays objects below this object in the hierarchy.\n\
		If no object is given the hierarchy of the current picture\n\
		is default (show object /hierarchy).\n\
/global		Search is also perfomed in mounted volumes of remote nodes.\n\
"},
{1,	"CREATE OBJECT","",
"\
Create an object.\n\
Objects can only be created in dynamic volumes.\n\n\
Syntax:\n\n\
pwr_rtt> create object /name= /class= \n\n\
/name		name of the object including hierarchy\n\
/class		class of the object\n\n\
Example		pwr_rtt> create object /name=Noder-Noden-RttUser /class=User\n\
"},
{1,	"CREATE MENU","",
"\
Create a new menu with one menu item. More items kan be added with the command\n\
'add menu'. A limited selection of meny items types is supported.\n\n\
Syntax:\n\n\
pwr_rtt> create menu /title= /text= [/object=] [/command=]\n\n\
/title		Title of the menu\n\
/text		Text of the menu item\n\
/object		Name of object an object. The menuitem will have the\n\
		characteristics of an object. pf1 will open the object, return\n\
		will show the children and pf2 will debug the children.\n\
/command	The menu item is of type rtt command. The rtt-command that\n\
		will be executed if pf1 is activated should be added.\n\n\
Example\n\
  pwr_rtt> create menu /title=\"My menu\" /text=\"More menues\"\n\
  pwr_rtt> create menu /title=\"Some objects\" /text=\"Obj1\" /object=rt-obj1\n\
  pwr_rtt> create menu /title=\"Cmd menu\" /text=\"cmd1\" /command=@cmd1\n\
"},
{1,	"ADD MENU","",
"\
Add a menu item to a menu.\n\
A limited selection of meny items types is supported.\n\n\
Syntax:\n\n\
pwr_rtt> add menu /text= [/object=] [/command=]\n\n\
/text		Text of the menu item\n\
/object		Name of object an object. The menuitem will have the\n\
		characteristics of an object. pf1 will open the object, return\n\
		will show the children and pf2 will debug the children.\n\
/command	The menu item is of type rtt command. The rtt-command that\n\
		will be executed if pf1 is activated should be added.\n\n\
Example\n\
  pwr_rtt> add menu /text=\"More menues\"\n\
  pwr_rtt> add menu /text=\"Obj1\" /object=rt-obj2\n\
  pwr_rtt> add menu /text=\"cmd1\" /command=@cmd2\n\
"},
{1,	"DELETE OBJECT","",
"\
Delete an object.\n\
Only objects in dynamic volumes can be deleted.\n\n\
Syntax:\n\n\
pwr_rtt> delete object /name= \n\n\
/name		name of the object including hierarchy\n\n\
Example		pwr_rtt> delete object /name=Noder-Noden-RttUser\n\
"},
{1,	"PRINT","",
"\
Print the current picture to a file or printer:\n\
- print the content of a picture or menu to a file.\n\
- print a string into file.\n\
- print a copy of the terminal to a printer.\n\
- print the attributes and values of the current picture toa rtt script-file\n\
    so that the values can be restored when executing the script.\n\n\
Print the content of the current picture or menu into a file.\n\
pwr_rtt> print /file= /append /psize= /tsize=\n\
/file		filename. Default extention is .lis\n\
/append		write in an existing file.\n\
/psize		number of characters for parameter name. Default 25\n\
/tsize		number of characters the text. Default 15\n\n\
Print a copy of the terminal to a printer.\n\
pwr_rtt> print /terminal\n\n\
Print a string to a file.\n\
pwr_rtt> print /text= /append\n\n\
Print the attribues and values of the curren picture to a rtt script-file,\n\
so that the values can be restored when executing the script\n\
pwr_rtt> print /restore /file=\n\
/file		filename. Default extention is .rtt_com\n\
"},
{1,	"PLCSCAN","",
"\
This command set the execution of the plc window on or off.\n\
If no hierarchy object i given, the selected object is chosen\n\
as hierarchy object.\n\n\
/on		set execution on\n\
/off		set execution off\n\
/all		set execution of all plc windows in the system or in the node\n\
/hierarchy=	set execution of all windows below this object in\n\
		the hierarchy.\n\
		If no object is given the hierarchy of the current picture\n\
		is default (show object /hierarchy).\n\
/global		Search is also perfomed in mounted volumes of remote nodes.\n\
"},
{1,	"COLLECT","",
"\
Handles the collection picture.\n\n\
collect			add the selected object to the collection picture\n\
			If the object is collected from a hierarchy menu,\n\
			the debugparameter will be displayed, if a parameter\n\
			is collected in a 'show object' menu the selected\n\
			parameter will be displayed.\n\n\
collect /name=		add the specified object. The object name kan include\n\
			the parametername, otherwise the debug parameter is\n\
			collected.\n\n\
collect clear		clear the collection picture. The collection picture\n\
			has to be active.\n\n\
collect show		show the collection picture.\n\
"},
{1,	"STORE","",
"\
Store a collection picture, a debug picture or the defined symbols in a file.\n\
The format of the file is rtt command file format.\n\n\
Store symbols.\n\
If no file is specified the symbolfile-name in the rttconfig-object will be\n\
used.\n\
pwr_rtt> store /symbols\n\n\
Store a picture.\n\
The picture store stored in the file /file='filename' will be restored\n\
with the command\n\
pwr_rtt> @'filename'\n\n\
pwr_rtt> store /file= [/collect]\n\n\
/file=			filename in which the picture will be stored.\n\
/collect		The picture will be restored in the collection picture.\n\
			If omitted the picture will be restored as a debug\n\
			picture.\n\
"},
{0,	"STORED PICTURES AND COMMANDFILES",
"       RETURN show a picture or execute a command file            Ctrl/R back",
"\
List of the stored pictures and commandfiles in the default directory.\n\n\
To show a picture or execute a commandfile, select the picture or file\n\
and press RETURN.\n\n\n\
Related subjects: store.\n\
"},
{1,	"EXIT","",
"\
Terminates the execution.\n\
"},
{1,	"SEARCH","",
"\
Search for a string in the current menu.\n\
Search direction is forward and start from the current position.\n\
If no search string is given, the last one is taken.\n\n\
pwr_rtt> search 'searchstring'\n\
"},
{1,	"PAGE","",
"\
Display a page n the current menu.\n\n\
pwr_rtt> page 'pagenr'\n\
"},
{1,	"TOP","",
"\
Display the first page and select the first item.\n\n\
pwr_rtt> top\n\
"},
{1,	"CLASSHIER","",
"\
Display the class hierarchy.\n\
"},
{1,	"LOGGING","",
"\
Logg parameters on file (ELN only).\n\n\
Create a logging entry with 'loggin create'. An entry loggs up to ten\n\
parameters on a file. Ten files can be open simultanously.\n\
Insert parameter into the logging entry from the collection picture with\n\
/insert. Specify with /type if values should be logged only when the values\n\
of parameters are changed (/type=EVENT) or continously (/type=CONT).\n\
Modify the logging entry whith 'logging set'.\n\
Start and stop the logging with 'logging start' and 'logging stop'.\n\
Store a logging entry or all logging entries in a rtt commandfile. The\n\
commandfiles can be restored by executing the commandfile.\n\n\
pwr_rtt> logging create /insert /file= /time= /type= /parameter= /condition=\n\
			/buffer_size= /line_size=\n\
pwr_rtt> logging set /entry= /insert /file= /time= /type= /parameter=\n\
		     /condition= /buffer_size= /line_size=\n\
pwr_rtt> logging delete /entry\n\
pwr_rtt> logging start /entry=\n\
pwr_rtt> logging stop /entry=\n\
pwr_rtt> logging show /entry=\n\
pwr_rtt> logging store [/entry=]/file=\n\
pwr_rtt> logging store /all/file=\n\
"},
{1,	"LOGIN",
"",
"\
Login as a Proview user with username and password.\n\
The login command will open the login frame which promps for\n\
username and password.\n\
\n\
pwr_rtt> login\n\
"},
{1,	"SET","",
"\
List of set commands:\n\n\
pwr_rtt> set parameter/name=/value=\n\
pwr_rtt> set priority/priority=n\n\
pwr_rtt> set pwrp_alias\n\
pwr_rtt> set [no]alarmmessage\n\
pwr_rtt> set [no]alarmbeep\n\
pwr_rtt> set conversion [/name][/on][/off]\n\
pwr_rtt> set invert [/name][/on][/off]\n\
pwr_rtt> set dotest [/name][/on][/off]\n\
pwr_rtt> set testvalue [/name][/on][/off]\n\
pwr_rtt> set clock\n\
pwr_rtt> set time\n\
pwr_rtt> set [no]description\n\
pwr_rtt> set scantime/time=\n\
pwr_rtt> set default\n\
pwr_rtt> set [no]message\n\
pwr_rtt> set [no]draw\n\
"},
{1,	"SET PRIORITY","",
"\
Change the job priority of the rtt job.\n\n\
pwr_rtt> set priority/priority=n\n\n\
The priority can be in the range 0 to 31, with 0 representing the highest\n\
priority.\n\
"},
{1,	"SET PWRP_ALIAS","",
"\
Executes the settings in the file pwrp_alias.dat.\n\n\
pwr_rtt> set pwrp_alias\n\
"},
{1,	"SET ALARMMESSAGE","",
"\
Last not acknowledged alarm will be shown in the message line.\n\n\
pwr_rtt> set alarmmessage\n\n\
The eventlist has to be loaded.\n\
"},
{1,	"SET ALARMBEEP","",
"\
A sound signal will occur if there is a a not acknowledged alarm.\n\n\
pwr_rtt> set alarmbeep\n\n\
The eventlist has to be loaded.\n\
"},
{1,	"SET PARAMETER","",
"\
Set the value of a parameter.\n\n\
pwr_rtt> set parameter/name=/value=\n\n\
/name	full name of parameter ('objectname'.'parametername')\n\
/value	the new value of the parameter.\n\n\
Ex	pwr_rtt> set par/name=sh-fsdn-motorstart.actualvalue/value=1\n\n\
"},
{1,	"SET CONVERSION","",
"\
Set io conversion of a input channel.\n\
The channel can be specified by name (/name qualifier) or by selecting\n\
the channel, or the connected signal.\n\n\
pwr_rtt> set conversion /on [/name=] [/on] [/off]\n\
pwr_rtt> set conversion /off [/name=] [/on] [/off]\n\n\
/name		Name of a channel object, or the connected signal object.\n\
/on		Set conversion on.\n\
/off		Set conversion off.\n\
"},
{1,	"SET INVERT","",
"\
Set io invert of a channel.\n\
The channel can be specified by name (/name qualifier) or by selecting\n\
the channel, or the connected signal.\n\n\
pwr_rtt> set invert /on [/name=]\n\
pwr_rtt> set invert /off [/name=]\n\n\
/name		Name of a channel object, or the connected signal object.\n\
/on		Set invert on.\n\
/off		Set invert off.\n\
"},
{1,	"SET DOTEST","",
"\
Set io test on a do channel.\n\
The value of the do will be fetched from the do testvalue, witch can be set\n\
or reset by the 'set testvalue' command\n\
The channel can be specified by name (/name qualifier) or by selecting\n\
the channel, or the connected signal.\n\n\
pwr_rtt> set dotest /on [/name=]\n\
pwr_rtt> set dotest /off [/name=]\n\n\
/name		Name of a channel object, or the connected signal object.\n\
/on		Set dotest on.\n\
/off		Set dotest off.\n\
"},
{1,	"SET TESTVALUE","",
"\
Set testvalue on a do channel.\n\
The testvalue will be set as output if the dotest is on (use the 'set dotest'\n\
command)\n\
The channel can be specified by name (/name qualifier) or by selecting\n\
the channel, or the connected signal.\n\n\
pwr_rtt> set dotest /on [/name=]\n\
pwr_rtt> set dotest /off [/name=]\n\n\
/name		Name of a channel object, or the connected signal object.\n\
/on		Set testvalue to true on.\n\
/off		Set testvalue to false.\n\
"},
{1,	"SET DESCRIPTION","",
"\
Display or hide the description attribute in the object hierarchy.\n\
If description mode is set off, the name and class of an object will be\n\
displayed. If description is set on, the description attribute of the object\n\
will also be displayed if there is one.\n\
The action will only affect new hierarchy-pictures, not the already created.\n\n\
pwr_rtt> set description               display description.\n\
pwr_rtt> set nodescription             hide description.\n\
"},
{1,	"SET CLOCK","",
"\
Set the hardware clock to the same time as the VAXELN system time.\n\
Implemented in VAXELN only.\n\n\
pwr_rtt> set clock\n\n\
Related subjects: show clock, get clock.\n\
"},
{1,	"SET TIME","",
"\
Set the system time.\n\
The time format is dd-mmm-yyyy:hh:mm:ss\n\n\
pwr_rtt> set time dd-mmm-yyyy:hh:mm:ss\n\n\
Example\n\n\
pwr_rtt> set time 30-APR-1997:14:07:00\n\
"},
{1,	"SET SCAN","",
"\
Set the scan time of rtt. This is the time rtt will update\n\
a picture, scan the alarmlist etc.\n\
Time in seconds.\n\n\
pwr_rtt> set scan/time=\n\
"},
{1,	"SET DEFAULT","",
"\
Set the default directory for displaying files with the 'show file' and\n\
the 'directory' commands.\n\n\
pwr_rtt> set default 'directory'\n\
"},
{1,	"SET MESSAGE","",
"\
Enable and disable of rtt messages.\n\
Termination of commandfile mode will always enable rtt messages.\n\n\
pwr_rtt> set message\n\
pwr_rtt> set nomessage\n\
"},
{1,	"SET DRAW","",
"\
Display result of the actions performed in a script on the screen.\n\
The normal condition  when executing a script is that nothing is displayed\n\
on the screen until the script is terminated. Then the current picture is\n\
redrawn. With 'set draw' command, all actions will be displayed on the screen\n\
as if they were preformed in an interactive mode. 'set draw' will also supress\n\
the redrawing of the picture when at termination of the script.\n\
'set nodraw' will return to the concealed mode.\n\n\
set draw\n\
set nodraw\n\
"},
{1,	"GET CLOCK","",
"\
Set the VAXeln system time to the same as the hardware clock.\n\
Implemented in VAXELN only.\n\n\
pwr_rtt>get clock\n\n\
Related subjects: show clock, set clock.\n\
"},
{1,	"ALARM","",
"\
Send an alarm, show the alamlist, show or print the eventlist.\n\n\
Send an alarm with the specified alarm text and alarm priority.\n\
pwr_rtt> alarm send /text=\"...\" /priority=\n\
/priority can be A,B,C or D (default A).\n\n\
Print the eventlist on a file.\n\
pwr_rtt> alarm print/file= [/notext] [/noname]\n\
The alarmlist has to be loaded before alarm print. The alarmlist\n\
will be loaded when the alarm or eventlist is entered.\n\n\
Show alarmlist.\n\
pwr_rtt> alarm show [/user=][/acknowledge][/return][/bell][/maxalarm=]\n\
		[/maxevent=]\n\
Show eventlist.\n\
pwr_rtt> alarm list [/user=][/acknowledge][/return][/bell][/maxalarm=]\n\
		[/maxevent=]\
"},
{1,	"LEARN","",
"\
Store a key sequence on a rtt command file.\n\n\
The learn session is started with 'learn start' and ended with 'learn stop'.\n\
The commands will be executed with 'learn recall'.\n\n\
pwr_rtt> learn start\n\
pwr_rtt> learn stop\n\
pwr_rtt> learn recall\n\n\
If the operations should be stored in a specific commandfile, add the filename\n\
in the 'learn start'-command, and execute the file with @'filename'.\n\n\
pwr_rtt> learn start/file='filename'\n\
pwr_rtt> learn stop\n\
pwr_rtt> @'filename'\n\
"},
{1,	"CLOCK","",
"\
Handle the hardware clock in a VAXELN node.\n\
Implemented in VAXELN only.\n\n\
pwr_rtt> show clock	show the current time of the hardware clock\n\n\
pwr_rtt> set clock	set the hardware clock to the same time as the VAXeln\n\
			system time\n\n\
pwr_rtt> get clock	set the VAXeln system time to the same as the\n\
			hardware clock\n\
"},
{1,	"DEFINE","",
"\
Define a symbol in rtt command language.\n\
The symbol kan be used as a replacement symbol or as a short command.\n\n\
pwr_rtt> define symbol 'expression'\n\n\
Example of short command\n\
pwr_rtt> define valves \"show menu service-valves\"\n\
pwr_rtt> valves\n\
"},
{1,	"SHOW NODES","",
"\
Show nodes displays the content of the node database.\n\
All nodes that is connected to the nethandler will be shown.\n\
"},
{1,	"GRAFCET","",
"\
Display plcprograms that contains grafcet sequences.\n\
A plcprogram that is not in its init state will be indicated with the text\n\
\"Active\".\n\
To get more information about the activity in a plcprogram, navigate to\n\
the plcpgm in the picture and select it with PF1. Press PF2 and active steps\n\
and orders in the plcpgm will be shown.\n\
Max four plc can be shown at the same time.\n\
"},
{0,	"LIST OF SIGNALS",
"Ctrl/A Open object  Ctrl/T Crossreference  Ctrl/E Change value  Ctrl/R Back",
"\
Display the signals referenced in a plcpgm or a window.\n\
Press PF1 to open an object.\n\
Press PF2 to show cross references to an object.\n\
Press PF3 to change value of the signal.\n\
Press PF4 to go back.\n\
"},
{0,	"VIEW WINDOW",
"",
"\
Show the content of a file.\n\n\
Press arrows to scroll up, down, left or right.\n\
Press Ctrl/A or PF1 to show the beginning of the file.\n\
Press Ctrl/T or PF2 to show the end of the file.\n\
Press Ctrl/R PF4 to go back.\n\
Use the command search function to search for a string in the file\n\
Ex pwr_rtt> search \"3.2\"\n\
"},
{0,	"VIEW",
"",
"\
Show the content of a text-file.\n\n\
pwr_rtt> view /file='filespec'\n\
"},
{1,	"DIRECTORY",
"",
"\
Display files. Wildcard can be used.\n\
If no file or directory is specified, the files in the current default\n\
directory will be displayed.\n\n\
pwr_rtt> directory 'filespec'\n\
"},
{1,	"SYMBOLFILE",
"",
"\
The symbolfile is a file that will be executed at the start of the rtt session.\n\
It is possible to define symbols and to save the current defined symbols in\n\
the file.\n\
Symbols can be defined with 'define' command.\n\
Symbols can be displayed with the 'show symbol' command.\n\
Symbols can be stored into the symbolfile with the 'store /symbol' command.\n\n\
Example of symbol file\n\
define FREKVENSOMRIKTARE \"SHOW MENU UNDERHÅLL-FREKVENSOMRIKTARE\"\n\
define PLÅTFÖLJNING \"SHOW MENU UNDERHÅLL-PLÅTFÖLJNING\"\n\
define HYDRAULIK \"SHOW MENU \\\"UNDERHÅLL-HYDRAULIK PV15\\\"\"\n\
define VKVP15 \"SHOW MENU \\\"UNDERHÅLL-VKVP15 KOMMUNIKATION\\\"\"\n\
define GIVARE \"SHOW MENU UNDERHÅLL-GIVARE\"\n\
define END \"SHOW CHILD/NAME=VKV-END\"\n\
define SB15 \"SHOW CHILD/NAME=VKV-END-SB15\"\n\
define S0 \"SHOW CHILD/NAME=VKV-END-SB15-S0\"\n\
define S1 \"SHOW CHILD/NAME=VKV-END-SB15-S1\"\n\
define S2 \"SHOW CHILD/NAME=VKV-END-SB15-S2\"\n\
define S3 \"SHOW CHILD/NAME=VKV-END-SB15-S3\"\n\
"},
{1,	"SETUP",
"",
"\
Show the settings of the current rtt session.\n\n\
Platform	The current platform.\n\
ConfigureObject	The rtt configure object for the current session.\n\
DefaultVMSNode	The DECNET-address for a VMS node where crossreference-files,\n\
		logging-files etc will be opened.\n\
DescriptionOn	If TRUE the content of the description-attribute will be\n\
		diplayed.\n\
DefaultDirectory The default directory for displaying files with the commands\n\
		'directory' and 'show file'.\n\
ScanTime	The rtt scantime for update of pictures, alarmlists etc.\n\
AlarmMessage	If TRUE the last not acknowledged alarm will be displayed in\n\
		the message row (row 23).\n\
AlarmReturn	Return events will be shown in the event-list (this must be set\n\
		before the eventlist is loaded).\n\
AlarmAck	Acknowledge events will be shown in the event-list (this must\n\
		be set before the eventlist is loaded).\n\
SymbolFileName	The name of the symbolfile where symbols can be defined.\n\
Verify		If TRUE the commands executed in commandfiles will be displayed.\n\
SignalTestModeOn If TRUE the pictures for Do- and Ao-channels will contain\n\
		functions to set testvalues.\n\n\
pwr_rtt> setup\n\
"},
{1,	"SCRIPT","",
"\
A script-file will be executed from the command-line with the command\n\n\
pwr_rtt> @'filename'\n\n\n\
For more information about the rtt script syntax use command 'HELP SCRIPT'\n\
"},
{0,	"HELP SUBJECTS",
" Select a subject and press RETURN                               Ctrl/R Back",
"\
List of the help subjects in rtt.\n\n\
To display the help text for a subject, select a subject and press return.\n\
"},
{0,	"",}};

rtt_t_helptext	*rtt_command_helptext = helptext;

static rtt_t_helptext	script_helptext[] = {
{1,	"DATATYPES","",
"\
The datatypes are float, int and string.\n\n\
int	integer value.\n\
float	32-bit float value.\n\
string	80 character string (null terminated).\n\n\
There are three different tables in which a variable can be declared: local, \n\
global and extern. A local variable is known inside a function, a global is \n\
known in all functions in a file (with include-files), an external is known \n\
for all files executed in a rtt session.\n\n\
"},
{1,	"DATATYPE CONVERSIONS","",
"\
If an expression consists of variables and functions of different datatypes\n\
the variables will be converted with the precedence string, float, int. If\n\
two operands in an expression is of type int and float, the result will be float\n\
If two operands is of type float and string, or int and string, the result will\n\
be string. In an assignment the value of an expression will be converted to the\n\
type of the assignment variable, even if the result is a string and the\n\
variable is of type float or int.\n\n\
Example\n\n\
string 	str;\n\
int	i = 35;\n\
str = \"Luthor\" + i;\n\
The value in str will be \"Luthor35\".\n\n\
float 	f;\n\
string 	str = \"3.14\";\n\
int	i = 159;\n\
f = str + i;\n\
The value in f will be 3.14159.\n\
"},
{1,	"VARIABLE DECLARATIONS","",
"\
A variable must be declared before it is used.\n\
A declaration consists of\n\
- the table (global or extern, if local the table is suppressed)\n\
- the datatype (int, float or string)\n\
- the variable name (case sensitive)\n\
- equal mark followed by an init value, if omitted the init value is zero or\n\
  null-string\n\
- semicolon\n\n\
Example\n\n\
int		i;\n\
float		flow = 33.4;\n\
string		str = \"Hello\";\n\
extern	int 	jakob;\n\
global	float	ferdinand = 1234;\n\
"},
{1,	"OPERATORS","",
"\
The operators have the same function as i c, with some limitations. All\n\
operators are not implemented. Some operators (+,=,==) can also operate on\n\
string variables. Precedence of operators is similar to c.\n\n\
Operator	Description			Datatypes\n\
+		plus				int, float, string\n\
-		minus				int, float\n\
*		times				int, float\n\
/		divide				int, float\n\
++		increment, postfix only.	int, float\n\
--		decrement, postfix only		int, float\n\
>>		bits right-shifted		int\n\
<<		bits left-shifted		int\n\
<		less then			int, float\n\
>		greater then			int, float\n\
<=		less equal			int, float\n\
>=		greater equal			int, float\n\
==		equal				int, float, string\n\
!=		not equal			int, float, string\n\
&		bitwise and			int\n\
|		bitwise or			int\n\
&&		logical and			int\n\
||		logical or			int\n\
!		logical not			int\n\
=		assign				int, float, string\n\
+=		add and assign			int, float\n\
-=		minus and assign		int, float\n\
&=		logical and and assign		int\n\
|=		logical or and assign		int\n\
"},
{1,	"main-endmain","",
"\
The main and endmain statements controls where the execution starts and stops\n\
If no main and endmain statements will be found, the execution will start\n\
att the beginning of the file and stop at the end.\n\n\
Example\n\n\
main()\n\
  int	a;\n\n\
  a = p1 + 5;\n\
  printf( \"a = %d\\n\", a);\n\
endmain\n\
"},
{1,	"function-endfunction","",
"\
A function declaration consists of\n\
- the datatype of the return value for the function\n\
- the name of the function\n\
- an argumentlist delimited by comma and surrounded by parenthesis. The\n\
  argumentlist must include a typedeclaration and a name for eatch argument.\n\n\
The arguments supplied by the caller will be converted to the type of the\n\
to the type declared in the argument list. If an argument is changed inside\n\
the function, the new value will be transferred to the caller. In this way\n\
it is possible to return other values then the return value of the function.\n\
A function can contain one or several return statements. The return will hand\n\
over the execution to the caller and return the supplied value.\n\n\
Example\n\n\
function float calculate_flow(float a, float b)\n\
  float	c;\n\
  c = a + b;\n\
  return c;\n\
endfunction\n\n\
...\n\
flow = korr * calculate_flow( v, 35.2);\n\
"},
{1,	"if-else-endif","",
"\
The lines between a if-endif statment will be executed if the expression\n\
in the if-statment is true. The expression should be surrounded by parentheses.\n\
If an else statement is found between the if and endif the lines between else\n\
and endif will be executed if the if-exepression is false.\n\n\
Example\n\n\
if ( i < 10 && i > 5)\n\
  a = b + c;\n\
endif\n\n\
if ( i < 10)\n\
  a = b + c;\n\
else\n\
  a = b - c;\n\
endif\n\
"},
{1,	"while-endwhile","",
"\
The lines between a while-endwhile statment will be executed as long as the\n\
expression in the while-statment is true. The expression should be surrounded\n\
by parentheses.\n\n\
Example\n\n\
while ( i < 10)\n\
  i++;\n\
endwhile\n\
"},
{1,	"for-endfor","",
"\
The lines between a for-endfor statment will be executed as long as the\n\
middle expression in the for-statment is true. The for expression consists\n\
of three expression, delimited by semicolon and surrounded by parentheses.\n\
The first expression will be executed the before the first loop, the third\n\
will be executed after every loop, the middle is executed before every loop\n\
and if true another loop is done, if false the loop is leaved.\n\n\
Example\n\n\
for ( i = 0; i < 10; i++)\n\
  a += b;\n\
endfor\n\
"},
{1,	"break","",
"\
A break statement will search for the next endwhile or endfor statement\n\
continue the execution at the line after.\n\n\
Example\n\n\
for ( i = 0; i < 10; i++)\n\
  a += b;\n\
  if ( a > 100)\n\
    break;\n\
endfor\n\
"},
{1,	"continue","",
"\
A continue statement will search for the previous while or for statement\n\
continue the loop execution.\n\n\
Example\n\n\
for ( i = 0; i < 10; i++)\n\
  b = my_function(i);\n\
  if ( b > 100)\n\
    continue;\n\
  a += b;\n\
endfor\n\
"},
{1,	"goto","",
"\
A goto will cause the execution to jump to a row defined by label.\n\
The label line is terminated with colon.\n\n\
Example\n\n\
  b = attribute(\"MOTOR-ON.ActualValue\", sts);\n\
  if (!sts)\n\
    goto some_error; \n\
  ...\n\
some_error:\n\
  say(\"Something went wrong!\");\n\
"},
{1,	"include","",
"\
An script include-file containing functions can be included with the\n\
#include statement. The default file extention is '.rtt_com'\n\n\
Example\n\n\
#include <my_functions>\n\
"},
{1,	"printf()","",
"\
int printf( string format [, (arbitrary type) arg1, (arbitrary type) arg2])\n\n\
Description\n\n\
Formatted print. C-syntax. Format argument and non, one or two value arguments.\n\
Returns number of printed characters.\n\n\
Arguments\n\n\
string 			format		Format.\n\
arbitrary type		arg1		Value argument. Optional. Can be int,\n\
					float or string.\n\
arbitrary type		arg2		Value argument. Optional. Can be int,\n\
					float or string.\n\n\
Example\n\n\
  printf( \"Watch out!\\n\");\n\
  printf( \"a = %d\n\", a);\n\
  printf( \"a = %d och str = %s\\n\", a, str);\n\
"},
{1,	"scanf()","",
"\
int scanf( string format , (arbitrary type) arg1)\n\n\
Description\n\n\
Formatted input. C-syntax\n\
Returns number of read characters.\n\n\
Arguments\n\n\
string 			format		Format.\n\
arbitrary type		arg1		Value argument. Returned. Can be int,\n\
					float or string.\n\n\
Example\n\n\
  scanf( \"%d\", i);\n\
"},
{1,	"fprintf()","",
"\
int fprintf( int file, string format [, (arbitrary type) arg1,\n\
	(arbitrary type) arg2])\n\n\
Description\n\n\
Formatted print on file. C-syntax. Format argument and non, one or two value\n\
arguments.\n\
Returns number of printed characters.\n\n\
Arguments\n\n\
int 			file		File id retured by fopen.\n\
string 			format		Format.\n\
arbitrary type		arg1		Value argument. Optional. Can be int,\n\
					float or string.\n\
arbitrary type		arg2		Value argument. Optional. Can be int,\n\
					float or string.\n\n\
Example\n\n\
  int outfile;\n\
  outfile = fopen( \"my_file.txt\", \"w\");\n\
  if (!outfile)\n\
    exit();\n\
  fprintf( outfile, \"Some text\\n\");\n\
  fprintf( outfile, \"a = %d\\n\", a);\n\
  fclose( outfile);\n\
"},
{1,	"fgets()","",
"\
int fgets( string str, int file)\n\n\
Description\n\n\
Reads a line from a specified file.\n\
Returns zero if end of file.\n\n\
Arguments\n\n\
string 		str		Read line. Returned. \n\
int		file		file returned by fopen.\n\n\
Example\n\n\
  file = fopen(\"some_file.txt\",\"r\");\n\
  while( fgets( str, file))\n\
    say( str);\n\
  endwhile\n\
  fclose( file);\n\
"},
{1,	"fopen()","",
"\
int fopen( string filespec, string mode)\n\n\
Description\n\n\
Opens a file for read or write.\n\
Returns a file identifier. If the file could not be opened, zero is returned.\n\n\
Arguments\n\n\
string 		filespec	Name of file. \n\
string		mode		Access mode\n\n\
Example\n\n\
  int infile;\n\
  int outfile;\n\n\
  infile = fopen(\"some_file.txt\",\"r\");\n\
  outfile = fopen(\"another_file.txt\",\"w\");\n\
  ...\n\
  fclose( infile);\n\
  fclose( outfile);\n\
"},
{1,	"fclose()","",
"\
int fclose( int file)\n\n\
Description\n\n\
Closes an opened file.\n\n\
Arguments\n\n\
int		file		file-id returned by fopen.\n\n\
Example\n\n\
  int infile;\n\
  infile = fopen(\"some_file.txt\",\"r\");\n\
  ...\n\
  fclose( infile);\n\
"},
{1,	"exit()","",
"\
int exit()\n\n\
Description\n\n\
Terminates executions of the file.\n\n\
Example\n\n\
  exit();\n\
"},
{1,	"verify()","",
"\
int verify( [int mode])\n\n\
Description\n\n\
Sets or shows verification mode. If verification is on all executed lines will\n\
be displayed on the screen.\n\
Returns the current verification mode.\n\n\
Arguments\n\n\
int		mode		verification on (1) or off (0). Optional.\n\n\
Example\n\n\
  verify(1);\n\
"},
{1,	"time()","",
"\
string time()\n\n\
Description\n\n\
Returns the current time in string format.\n\n\
Example\n\n\
  string t;\n\
  t = time();\n\
"},
{1,	"edit()","",
"\
string edit( string str)\n\n\
Description\n\n\
Removes leading and trailing spaces and tabs, and replaces multipel tabs and\n\
spaces with a single space.\n\
Returns the edited string.\n\n\
Arguments\n\n\
string		str		string to be edited.\n\n\
Example\n\n\
  collapsed_str = edit(str);\n\
"},
{1,	"extract()","",
"\
string extract( int start, int length, string str)\n\n\
Description\n\n\
Extracts the specified characters from the specified string.\n\
Returns the extracted characters as a string.\n\n\
Arguments\n\n\
int		start		start position of the first character.\n\
int		length		number of characters to be extracted.\n\
string		str		string from which characters should be extracted.\n\n\
Example\n\n\
  extracted_str = extract( 5, 7, str);\n\
"},
{1,	"element()","",
"\
string element( int number, string delimiter, string str)\n\n\
Description\n\n\
Extracts one element from a string of elements.\n\
Returns the extracted element.\n\n\
Arguments\n\n\
int		number		the number of the element.\n\
string		delimiter	delimiter character.\n\
string		str		string of elements.\n\n\
Example\n\n\
  string str = \"mary, lisa, anna, john\";\n\
  string elem1;\n\
  elem1 = elment( 1, \",\", str);\n\
"},
{1,	"RTT-COMMANDS","",
"\
All the rtt-commands is available in the script code. An rtt-command line\n\
should NOT be ended with a semicolon. Variables can be substituted in the\n\
command line by surrounding them with apostrophes.\n\n\
Example\n\n\
  string 	name = \"PUMP-VALVE-Open.ActualValue\";\n\
  float	value = 2.2;\n\
  set parameter/name='name'/value='value'\n\n\
Example\n\n\
  string name;\n\
  string parname;\n\
  int j;\n\
  int i;\n\
  for ( i = 0; i < 3; i++)\n\
    parname = \"vkv-test-obj\" + (i+1);\n\
    create obj/name='parname'\n\
    for ( j = 0; j < 3; j++)\n\
      name = parname + \"-obj\" + (j+1);\n\
      create obj/name='name'\n\
    endfor\n\
  endfor\n\
"},
{1,	"GetAttribute()","",
"\
(variable type) GetAttribute( string name [, int status])\n\n\
Description\n\n\
Get the value of the specified attribute. The returned type is dependent\n\
of the attribute type. The attribute will be converted to int, float or string.\n\n\
Arguments\n\n\
string		name		name of the attribute to be fetched.\n\
int		status		status of operation. Returned. If zero, the\n\
				attribute could not be fetched. Optional.\n\n\
Example\n\n\
  int alarm;\n\
  int sts;\n\n\
  alarm = GetAttribute(\"Roller-Motor-Alarm.ActualValue\");\n\
  on = GetAttribute(\"Roller-Motor-On.ActualValue\", sts);\n\
  if ( !sts)\n\
    say(\"Could not find motor on attribute!\");\n\
"},
{1,	"GetChild()","",
"\
string GetChild( string name)\n\n\
Description\n\n\
get the first child of an object. The next children can be fetched with\n\
GetNextSibling().\n\
Returns the name of the child. If no child exists a null-string is returned\n\n\
Arguments\n\n\
string		name		name of object.\n\n\
Example\n\n\
  string child;\n\n\
  child = GetChild(\"Roller-Motor\");\n\
"},
{1,	"GetParent()","",
"\
string GetParent( string name)\n\n\
Description\n\n\
Get the parent of an object.\n\
Returns the name of the child. If no parent exists a null-string is returned.\n\n\
Arguments\n\n\
string		name		name of object.\n\n\
Example\n\n\
  string parent;\n\n\
  parent = GetChild(\"Roller-Motor\");\n\
"},
{1,	"GetNextSibling()","",
"\
string GetNextSibling( string name)\n\n\
Description\n\n\
Get the next sibling of an object.\n\
Returns the name of the sibling. If no next sibling exists a null-string is\n\
returned.\n\n\
Arguments\n\n\
string		name		name of object.\n\n\
Example\n\n\
 string name;\n\
 int not_first;\n\n\
 name = GetChild(\"Rt\");\n\
 not_first = 0;\n\
 while ( name != \"\")\n\
   if ( !not_first)\n\
     create menu/title=\"The Rt objects\"/text=\"'name'\"/object=\"'name'\"\n\
   else\n\
     add menu/text=\"'name'\"/object=\"'name'\"\n\
   endif\n\
   not_first = 1;\n\
   name = GetNextSibling(nname);\n\
 endwhile\n\
 if ( !not_first )\n\
   MessageError(\"No objects found\");\n\
"},
{1,	"GetClassList()","",
"\
string GetClassList( string class)\n\n\
Description\n\n\
Get the first object of a specified class. The next object of the class\n\
can be fetched whith GetNextObject().\n\
Returns the name of the first object. If no instances of the class exists a\n\
null-string is returned.\n\n\
Arguments\n\n\
string		name		name of class.\n\n\
Example\n\n\
  string name;\n\n\
  name = GetClassList(\"Dv\");\n\
"},
{1,	"GetNextObject()","",
"\
string GetNextObject( string name)\n\n\
Description\n\n\
Get the next object in a classlist.\n\
Returns the name of the object. If no next object exist a null-string is\n\
returned.\n\n\
Arguments\n\n\
string		name		name of object.\n\n\
Example\n\n\
  string name;\n\n\
  name = GetClassList(\"Di\");\n\
  while ( name != \"\")\n\
    printf(\"Di object found: %s\", name);\n\
    name = GetNextObject(name);\n\
  endwhile\n\
"},
{1,	"GetObjectClass()","",
"\
string GetObjectClass( string name)\n\n\
Description\n\n\
Get the class of an object.\n\
Returns the name of the class.\n\n\
Arguments\n\n\
string		name		name of object.\n\n\
Example\n\n\
  string class;\n\n\
  class = GetObjectClass(\"Motor-Enable\");\n\
"},
{1,	"GetNodeObject()","",
"\
string GetNodeObject()\n\n\
Description\n\n\
Get the node object.\n\
Returns the name of the node object.\n\n\
Example\n\n\
  string node;\n\
  node = GetNodeObject();\n\
"},
{1,	"GetRootList()","",
"\
string GetRootList()\n\n\
Description\n\n\
Get the first object in the root list.\n\
Returns the name of the root object. The next object in the root list can be\n\
fetched with GetNextSibling().\n\n\
Example\n\n\
  string name;\n\n\
  name = GetRootList();\n\
  while( name != \"\")\n\
    printf( \"Root object found: %s\", name);\n\
    name = GetNextSibling(name);\n\
  endwhile\n\
"},
{1,	"CutObjectName()","",
"\
string CutObjectName( string name, int segments)\n\n\
Description\n\n\
Cut the first segments of an object name.\n\
Returns the last segments of an object name. The number of segments left is\n\
specified by the second argument\n\n\
Arguments\n\n\
string		name		Path name of object.\n\
int		segments	Number of segments that should be left.\n\n\
Example\n\n\
  string path_name;\n\
  string object_name;\n\n\
  path_name = GetChild(\"Rt-Motor\");\n\
  object_name = CutObjectName( path_name, 1);\n\
"},
{1,	"MessageError()","",
"\
string MessageError( string message)\n\n\
Description\n\n\
Print an rtt error message on the screen.\n\n\
Example\n\n\
  MessageError(\"Something went wrong\");\n\
"},
{1,	"MessageInfo()","",
"\
string MessageInfo( string message)\n\n\
Description\n\n\
Print an rtt info message on the screen.\n\n\
Example\n\n\
  MessageInfo(\"Everything is all right so far\");\n\
"},
{1,	"GetInput()","",
"\
int GetInput( string format, (arbitrary type) input_variable [, string prompt]\n\
              [, int terminator])\n\n\
Description\n\n\
Formated input of one variable of type string, int or float. GetInput\n\
will update the current picture and alarmtext during the input sequence\n\
(scanf and ask will not).\n\n\
Arguments\n\n\
string		format		format in c-syntax.\n\
(arbitrary type) input_variable	variable to receive the input.\n\
string		prompt		optional, specifies the prompt to be dislayed.\n\n\
int		terminator	optional, returns the ascii-code for the\n\
				terminator character. For PF1 to PF4 the code\n\
				278 to 281 is returned. The code is defined\n\
				in the file pwr_inc:rt_rtt.h RTT_K_...\n\n\
Example\n\n\
  int value;\n\
  string text;\n\n\
  PlaceCursor( 1, 23);\n\
  LineErase();\n\
  PlaceCursor( 1, 23);\n\
  GetInput( \"%d\", value, \"Select coolingbed: \");\n\
  PlaceCursor( 1, 23);\n\
  LineErase();\n\n\
  sprintf( text, \"You have selected coolingbed %d...\", value);\n\
  MessageInfo( text);\n\
"},
{1,	"PlaceCursor()","",
"\
int PlaceCursor( int x, int y)\n\n\
Description\n\n\
Change the cursor position. x is the column and y the row.\n\
The position of the upper left corner is (1,1).\n\n\
Arguments\n\n\
int	x	 column, the range for x is 1-80\n\
int	y	 row, the range for y is 1-24\n\n\
Example\n\n\
  PlaceCursor( 1, 23);\n\
"},
{1,	"LineErase()","",
"\
int LineErase()\n\n\
Description\n\n\
Erase the line from the current cursor position.\n\n\
Example\n\n\
  PlaceCursor( 1, 23);\n\
  LineErase();\n\
"},
{1,	"GetCurrentTitle()","",
"\
string GetCurrentTitle()\n\n\
Description\n\n\
Get the title of the current picture or menu.\n\n\
Example\n\n\
  string title;\n\n\
  title = GetCurrentTitle();\n\
"},
{1,	"GetCurrentText()","",
"\
string GetCurrentText()\n\n\
Description\n\n\
Get the text of the current menu item or update field.\n\n\
Example\n\n\
  string text;\n\n\
  text = GetCurrentText();\n\
"},
{1,	"GetCurrentObject()","",
"\
string GetCurrentObject()\n\n\
Description\n\n\
Get the object associated with the current menu item.\n\
If no object is associated, a null-string i returned.\n\n\
Example\n\n\
  string object;\n\n\
  object = GetCurrentObject();\n\
"},
{0,	"",}};

rtt_t_helptext	*rtt_script_helptext = script_helptext;
#endif
