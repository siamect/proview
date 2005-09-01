/** 
 * Proview   $Id: co_msg.h,v 1.3 2005-09-01 14:57:52 claes Exp $
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

#ifndef co_msg_h
#define co_msg_h

#ifndef pwr_h
# include "pwr.h"
#endif

#if defined __cplusplus
extern "C" {
#endif

char *
msg_GetMsg (
  const int	sts,
  char		*buf,
  int		bufSize
);

char *
msg_GetText (
  const int sts,
  char *buf,
  int bufSize
);

char *
msg_GetMessage (
  const pwr_tStatus	sts,
  unsigned int		flags,
  char			*buf,
  int			bufSize
);

#if defined __cplusplus
}
#endif
#endif








