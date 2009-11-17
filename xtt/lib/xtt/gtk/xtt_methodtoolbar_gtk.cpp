/* 
 * Proview   $Id: xtt_methodtoolbar_gtk.cpp,v 1.4 2008-12-02 09:31:51 claes Exp $
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

/* xtt_methodtoolbar_gtk.cpp -- Display method toolbar */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "co_dcli.h"
#include "cow_wow_gtk.h"
#include "xtt_xnav.h"
#include "xtt_methodtoolbar_gtk.h"

XttMethodToolbarGtk::XttMethodToolbarGtk( XNav *xnav) :
  XttMethodToolbar(xnav), m_timerid(0)
{
  for ( int i = 0; i < m_size; i++) {
    m_cb[i].mt = this;
    m_cb[i].idx = i;
  }
}

XttMethodToolbarGtk::~XttMethodToolbarGtk()
{
  if ( m_timerid)
    g_source_remove( m_timerid);
}

void XttMethodToolbarGtk::activate_button( GtkWidget *w, gpointer data)
{
  XttMethodToolbarGtk *mt = ((xtt_sMethodButtonCb *)data)->mt;
  int idx = ((xtt_sMethodButtonCb *)data)->idx;

  int		sts;
  int		is_attr;
  pwr_sAttrRef	aref;
  xmenu_eItemType menu_type;

  sts = mt->m_xnav->get_select( &aref, &is_attr);

  if ( aref.Flags.b.Object)
    menu_type = xmenu_eItemType_Object;
  else if ( aref.Flags.b.ObjectAttr)
    menu_type = xmenu_eItemType_AttrObject;
  else
    menu_type = xmenu_eItemType_Attribute;

  if ( ODD(sts))
    mt->m_xnav->call_method( mt->m_data[idx].method, mt->m_data[idx].filter, aref,
			     menu_type,
			     xmenu_mUtility_XNav,
			     mt->m_xnav->priv, 0);
  
}

GtkWidget *XttMethodToolbarGtk::build()
{
  pwr_tFileName fname;

  // Toolbar
  m_toolbar_w = (GtkWidget *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  for ( int i = 0; i < m_size; i++) {
    m_button_w[i] = gtk_button_new();
    dcli_translate_filename( fname, m_data[i].image);
    gtk_container_add( GTK_CONTAINER(m_button_w[i]), 
		       gtk_image_new_from_file( fname));
    g_signal_connect( m_button_w[i], "clicked", G_CALLBACK(activate_button), &m_cb[i]);
    g_object_set( m_button_w[i], "can-focus", FALSE, NULL);
    gtk_toolbar_append_widget( GTK_TOOLBAR(m_toolbar_w), m_button_w[i], CoWowGtk::translate_utf8( m_data[i].tooltip), "");
  }

  return m_toolbar_w;
}

void XttMethodToolbarGtk::set_sensitive()
{
  if ( m_timerid)
    g_source_remove( m_timerid);

  m_timerid = g_timeout_add( 400, set_sensitive_cb, this);
}

gboolean XttMethodToolbarGtk::set_sensitive_cb( void *data)
{
  XttMethodToolbarGtk *toolbar = (XttMethodToolbarGtk *)data;

  toolbar->m_timerid = 0;
  toolbar->set_current_sensitive();
  return FALSE;
} 

void XttMethodToolbarGtk::set_current_sensitive()
{
  int		is_attr;
  pwr_sAttrRef	aref;
  xmenu_eItemType menu_type;
  pwr_tStatus 	sts;
  
  sts = m_xnav->get_select( &aref, &is_attr);
  if ( EVEN(sts)) {
    // Nothing selected
    for ( int i = 0; i < m_size; i++)
      gtk_widget_set_sensitive( m_button_w[i], FALSE);      
  }
  else {
    gdh_sVolumeInfo info;
    
    // Skip extern volumes
    sts = gdh_GetVolumeInfo( aref.Objid.vid, &info);
    if ( EVEN(sts)) return;

    if ( info.cid == pwr_eClass_ExternVolume) {
      for ( int i = 0; i < m_size; i++)
	gtk_widget_set_sensitive( m_button_w[i], FALSE);      
      return;
    }

    if ( aref.Flags.b.Object)
      menu_type = xmenu_eItemType_Object;
    else if ( aref.Flags.b.ObjectAttr)
      menu_type = xmenu_eItemType_AttrObject;
    else
      menu_type = xmenu_eItemType_Attribute;
    
    for ( int i = 0; i < m_size; i++) {
      sts = m_xnav->check_object_methodfilter( aref, menu_type, xmenu_mUtility_XNav,
					       m_xnav->priv, m_data[i].name);
      if ( ODD(sts)) 
	gtk_widget_set_sensitive( m_button_w[i], TRUE);
      else
	gtk_widget_set_sensitive( m_button_w[i], FALSE);
    }
  }
}
