! 
!  Proview   $Id: rs_rtc_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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

        .facility       RTC,315 /prefix = RTC__ ! 

success 	<successful completion>				/succ
invarg		<invalid argument>				/error
buserr		<Bus error, clock not installed of faulty>	/error
timerr		<TimeValue invalid must be greater than 1-AUG-1995 00:00> /error
timeout		<Timout, no answere from server>		/error
