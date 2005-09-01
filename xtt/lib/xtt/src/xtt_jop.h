/* 
 * Proview   $Id: xtt_jop.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#ifndef xtt_jop_h
#define xtt_jop_h

/* xtt_jop.h -- Handling of java processe in xtt. */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

class Jop {
  public:
    Jop(
	void *op_parent_ctx);
    ~Jop();

    void 		*parent_ctx;
    qcom_sQid		qcom_qid;
    qcom_sQid		jop_qcom_qid;
    void 		(*command_cb)( void *, char *);

    void	scan();
    int		create_que();
    void 	que_free( char *data);
    int 	que_get( char **data);
    int 	que_put( char *data);
    void	set_jop_qid( int qix);
    int 	command( char *command);
    void	close();
    void	create_process();
};

#if defined __cplusplus
}
#endif
#endif

