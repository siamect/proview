/* 
 * Proview   $Id$
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

package jpwr.jop;

import jpwr.rt.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopLanguageFrame extends JFrame implements ActionListener {
  String[] languages = { "English",
			 "Swedish",
			 "German",
			 "French",
			 "Chinese"};

  JPanel contentPane;
  BorderLayout borderLayout = new BorderLayout();
  BorderLayout borderLayoutButton = new BorderLayout();
  JPanel langPanel = new JPanel();
  JComboBox cbLang = new JComboBox(languages);
  Dimension size;
  JLabel labelLang = new JLabel(JopLang.transl("Select Language"));
  JPanel buttonPanel = new JPanel();
  JLabel labelMsg = new JLabel();
  JopSession session;
  int lang;

  public JopLanguageFrame( JopSession session) {
    this.session = session;
    size = new Dimension( 300, 170);
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout);
    contentPane.add(labelLang, BorderLayout.NORTH);
    contentPane.add(cbLang, BorderLayout.CENTER);
    contentPane.add(labelMsg, BorderLayout.SOUTH);
    contentPane.setOpaque(true);
    
    contentPane.setBackground(GeColor.getColor(40, GeColor.NO_COLOR));
    this.setSize(size);

    this.setTitle(JopLang.transl("Language"));

    cbLang.addActionListener( this);

    int idx;
    switch ( JopLang.getLanguage()) {
    case JopLang.LANGUAGE_sv_SE: idx = 1; break;
    case JopLang.LANGUAGE_de_DE: idx = 2; break;
    case JopLang.LANGUAGE_fr_FR: idx = 3; break;
    case JopLang.LANGUAGE_zh_CN: idx = 4; break;
    default: 
	idx = 0;
    }
    cbLang.setSelectedIndex( idx);
	
    Dimension d = labelLang.getSize();
    d.height = 50;
    labelLang.setPreferredSize(d);

    d = labelMsg.getSize();
    d.height = 70;
    labelMsg.setPreferredSize(d);

  }
  public void actionPerformed( ActionEvent e) {

        switch(cbLang.getSelectedIndex()){
        case 0:
	    lang = JopLang.LANGUAGE_en_US;
	    break;
        case 1:
	    lang = JopLang.LANGUAGE_sv_SE;
	    break;
        case 2:
	    lang = JopLang.LANGUAGE_de_DE;
	    break;
        case 3:
	    lang = JopLang.LANGUAGE_fr_FR;
	    break;
        case 4:
	    lang = JopLang.LANGUAGE_zh_CN;
	    break;
	}
	JopLang.switchLanguage(lang);
	session.setOpWindowLanguage( lang);
  } 
}
