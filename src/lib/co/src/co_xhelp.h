/** 
 * Proview   $Id: co_xhelp.h,v 1.9 2008-10-31 12:51:30 claes Exp $
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

#ifndef co_xhelp_h
#define co_xhelp_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_xhelpnav_h
# include "co_xhelpnav.h"
#endif

class CoXHelp {
  public:
    CoXHelp( 
	void 		*xa_parent_ctx,
	xhelp_eUtility  utility,
	int             *xa_sts);
    void 	*parent_ctx;
    char 	name[80];
    CoXHelpNav	*xhelpnav;
    void	*client_data;
    void	(*close_cb) ( void *, void *);
    void 	(*open_URL_cb)( void *, char *);
    int		set_focus_disabled;
    int         displayed;
    void	(*india_ok_cb)( CoXHelp *, char *);

    static CoXHelp *default_xhelp;

    virtual void set_dimension( int width, int height) {}
    virtual void pop();

    void map();
    void clear();
    int help( const char *key, const char *help_bookmark, navh_eHelpFile file_type,
	      const char *file_name, bool strict);
    int	help_index( navh_eHelpFile file_type, const char *file_name);
    int home();
    int back();
    int next_topic();
    int previous_topic();
    static void set_default( CoXHelp *xhelp) { default_xhelp = xhelp;}
    static int dhelp( const char *key, const char *help_bookmark, navh_eHelpFile file_type,
		      const char *file_name, bool strict);
    static int	dhelp_index( navh_eHelpFile file_type, const char *file_name);

    static void open_URL( void *ctx, char *url);
    static void open_input_dialog( CoXHelp *xhelp, const char *text, const char *title,
				   char *init_text,
				   void (*ok_cb)( CoXHelp *, char *));
    static void find_ok( CoXHelp *xhelp, char *search_str);
    
    virtual ~CoXHelp();
};


#endif







