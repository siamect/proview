! 
!  Proview   $Id: wb_distr_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
!  Copyright (C) 2005 SSAB Oxelösund AB.
!
!  This program is free software; you can redistribute it and/or 
!  modify it under the terms of the GNU General Public License as 
!  published by the Free Software Foundation, either version 2 of 
!  the License, or (at your option) any later version.
!
!  This program is distributed in the hope that it will be useful 
!  but WITHOUT ANY WARRANTY; without even the implied warranty of 
!  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
!  GNU General Public License for more details.
!
!  You should have received a copy of the GNU General Public License 
!  along with the program, if not, write to the Free Software 
!  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
!

        .facility       DISTR,303 /prefix = DISTR__ ! 

success 	<successful completion>				/succ
parnotfound	<parameter does not exist>			/error
nofile		<unable to open file>				/error
nonumber	<invalid number>				/error
syntax		<syntax error>					/error
error           <undefined error>                               /error
nobootfile	<unable to read bootfile>			/error
noloadfile	<unable to read loadfile>			/error
badvers		<loadfile version discrepency>			/error
badcluster      <can not check cluster membership>		/error
nofile_on_disk	<no matching file on the directory>		/error
maxvers		<higher version of loadfile exist on disk>	/info
bados		<unknown operating system>			/error
bootconf	<boot node is not configured>			/error
maxfiles	<max number of files exceeded>			/warning
internal	<internal disorder>				/error
nodeconf	<node is not configured>			/error
maxnodes	<max number of nodes exceeded>			/error
keeppos		<keep should be a positiv value>		/error
nolink		<unable to open link to node>			/error
FTP421		<Service not available, closing control connection> /error
FTP425		<Can't open data connection> 			/error
FTP426		<Connection closed, tranfer aborted> 		/error
FTP450		<Requested file action aborted. File unavailable> /error
FTP451		<Requested action aborted, local error in processing> /error
FTP452		<Requested action not taken, insufficient storage space in system> /error
FTP500		<Syntax error, command unrecognized> 		/error
FTP501		<Syntax error in parameters or arguments> 	/error
FTP502		<Command not implemented> 			/error
FTP503		<Bad sequence of commands> 			/error
FTP504		<Command not implemented for that parameter> 	/error
FTP530		<User not logged in> 				/error
FTP532		<Need account for storing files> 		/error
FTP550		<Requested action not taken. File unavailable> 	/error
FTP552		<Requested file action aborted, storage allocation exceeded> 	/error
FTP553		<Requested action not take. Illegal file name> 	/error
FTPXXX		<Unknown FTP error message>			/error


