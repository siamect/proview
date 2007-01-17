/** 
 * Proview   $Id: co_wow.h,v 1.13 2007-01-17 10:27:06 claes Exp $
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

#ifndef co_wow_h
#define co_wow_h

/* co_wow -- useful windows */

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
  wow_eModalDialogReturn_Deleted
} wow_eMotalDialogReturn;

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
 public:
  CoWow() {}
  virtual ~CoWow() {}
  virtual void DisplayQuestion( void *ctx, char	*title, char *text,
				void (* questionbox_ok) ( void *, void *),
				void (* questionbox_cancel) ( void *, void *),
				void *data) {}
  virtual void DisplayError( char *title, char *text) {}
  virtual void *CreateList( char *title, char *texts,
			    void (action_cb)( void *, char *),
			    void *ctx) { return NULL;}
  virtual void CreateFileSelDia( char *title, void *parent_ctx,
				 void (*file_selected_cb)(void *, char *, wow_eFileSelType),
				 wow_eFileSelType file_type) {}
  virtual int CreateModalDialog( char *title, char *text, char *button1, char *button2, char *button3,
				 char *image) { return wow_eModalDialogReturn_NYI;}
  static int HideWarranty();
  virtual int DisplayWarranty() { return 0;}
  virtual void DisplayLicense() {} 
  virtual CoWowTimer *timer_new() { return 0;}
};

#endif

