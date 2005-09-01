! 
!  Proview   $Id: rs_cnv_msg.msg,v 1.2 2005-09-01 14:58:00 claes Exp $
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

        .facility       CNV,314 /prefix = CNV__ ! 

success 	<successful completion>				/succ
syntax		<Syntax error in conversion definition table, '%s'> /error
nopartype	<No such parameter type, '%s'>			/error
convtype	<Type of conversion not supported, Param: '%s'>	/error
nofile		<Unable to open file, '%s'>			/error
nomemory 	<Unable allocate dynamic memory>		/error
filesize	<To many rows in conversion definition file>	/error
convdeftype 	<ConvDefType not defined>			/error
class		<ConvDef is not a class object>			/error
convdefclass	<Error creating conversion table from class, Reason: >	/error
convdeffile	<Error creating conversion table from file, Reason: >	/error
convdefobject	<Error creating conversion table from object, Reason: >	/error
