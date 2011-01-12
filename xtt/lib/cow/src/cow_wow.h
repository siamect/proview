/** 
 * Proview   $Id: co_wow.h,v 1.16 2008-12-01 16:42:35 claes Exp $
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

#ifndef cow_wow_h
#define cow_wow_h

/* cow_wow -- useful windows */

#include <string.h>

typedef enum {
  wow_eFileSelType_,
  wow_eFileSelType_All,
  wow_eFileSelType_Dbs,
  wow_eFileSelType_Wbl,
  wow_eFileSelType_WblClass,
  wow_eFileSelType_Graph,
  wow_eFileSelType_Image,
  wow_eFileSelType__
} wow_eFileSelType;

typedef enum {
  wow_eModalDialogReturn_NYI,
  wow_eModalDialogReturn_Button1,
  wow_eModalDialogReturn_Button2,
  wow_eModalDialogReturn_Button3,
  wow_eModalDialogReturn_Deleted,
  wow_eModalDialogReturn_ReturnPressed
} wow_eMotalDialogReturn;

typedef enum {
  wow_ePixmap_No,
  wow_ePixmap_Graph,
  wow_ePixmap__
} wow_ePixmap;

typedef struct {
  char input_str[200];
  int status;
} wow_sModalInputDialog;

class CoWowWidget {
};

class CoWowTimer {
 public:
  void (* m_callback)( void *);
  void *m_data;

  CoWowTimer() {}
  virtual ~CoWowTimer() {}
  virtual void add( int time, void (* callback)(void *data), void *data) {}
  virtual void remove() {}
};

class CoWowRecall {
 public:
  static const int m_recall_size = 30;
  static const int m_line_size = 160;
  char		m_recall[m_recall_size][m_line_size];
  int		m_current_recall_line;

  CoWowRecall() : m_current_recall_line(0) {
    memset( m_recall, 0, sizeof(m_recall));
  }
};

class CoWow {
 protected:
  static bool m_autoremove;

 public:
  CoWow() {}
  virtual ~CoWow() {}
  virtual void DisplayQuestion( void *ctx, const char *title, const char *text,
				void (* questionbox_ok) ( void *, void *),
				void (* questionbox_cancel) ( void *, void *),
				void *data) {}
  virtual void DisplayError( const char *title, const char *text) {}
  virtual void DisplayText( const char *title, const char *text) {}
  virtual void *CreateList( const char *title, const char *texts, int textsize,
			    void (action_cb)( void *, char *),
			    void (cancel_cb)( void *),
			    void *ctx, 
			    int show_apply_button = 0) { return NULL;}
  virtual void CreateFileSelDia( const char *title, void *parent_ctx,
				 void (*file_selected_cb)(void *, char *, wow_eFileSelType),
				 wow_eFileSelType file_type) {}
  virtual int CreateModalDialog( const char *title, const char *text, const char *button1, 
				 const char *button2, const char *button3,
				 const char *image) { return wow_eModalDialogReturn_NYI;}
  virtual wow_sModalInputDialog *CreateModalInputDialog( const char *title, const char *text, 
							 const char *button1, const char *button2, 
							 const char *button3, const char *image, 
							 int input_length) { return 0;}
  virtual void Wait( float time) {}

  static int HideWarranty();
  virtual int DisplayWarranty() { return 0;}
  virtual void DisplayLicense() {} 
  virtual CoWowTimer *timer_new() { return 0;}
  virtual pwr_tStatus CreateMenuItem( const char *name, void *menu, int pixmap, int append, void *w) { return 0;}
  virtual pwr_tStatus DeleteMenuItem( const char *name, void *menu) { return 0;}
  static void SetAutoRemove( bool on) { m_autoremove = on;}
};

#endif

