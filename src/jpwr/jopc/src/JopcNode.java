/* 
 * Proview   $Id: JopcNode.java,v 1.3 2005-09-01 14:57:51 claes Exp $
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

package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcNode extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_framethin	pwr_framethin3;
  pwr_valuelong	pwr_valuelong4;
  pwr_valuelong	pwr_valuelong6;
  pwr_valuelong	pwr_valuelong7;
  pwr_valuelong	pwr_valuelong9;
  pwr_valuelong	pwr_valuelong11;
  pwr_valuelong	pwr_valuelong13;
  pwr_valuelong	pwr_valuelong15;
  pwr_valuelong	pwr_valuelong17;
  pwr_valuelong	pwr_valuelong19;
  pwr_valuelong	pwr_valuelong21;
  pwr_framethin	pwr_framethin22;
  pwr_indsquare	pwr_indsquare23;
  pwr_valuelong	pwr_valuelong25;
  pwr_indsquare	pwr_indsquare26;
  pwr_valuelong	pwr_valuelong27;
  pwr_indsquare	pwr_indsquare28;
  pwr_valuelong	pwr_valuelong29;
  pwr_indsquare	pwr_indsquare30;
  pwr_valuelong	pwr_valuelong31;
  pwr_indsquare	pwr_indsquare32;
  pwr_valuelong	pwr_valuelong33;
  pwr_indsquare	pwr_indsquare34;
  pwr_valuelong	pwr_valuelong35;
  pwr_indsquare	pwr_indsquare36;
  pwr_valuelong	pwr_valuelong37;
  pwr_indsquare	pwr_indsquare38;
  pwr_valuelong	pwr_valuelong39;
  pwr_indsquare	pwr_indsquare40;
  pwr_valuelong	pwr_valuelong41;
  pwr_indsquare	pwr_indsquare42;
  pwr_valuelong	pwr_valuelong43;
  pwr_indsquare	pwr_indsquare44;
  pwr_valuelong	pwr_valuelong45;
  pwr_indsquare	pwr_indsquare46;
  pwr_valuelong	pwr_valuelong47;
  pwr_indsquare	pwr_indsquare48;
  pwr_valuelong	pwr_valuelong49;
  Grp110_	grp110_50;
  pwr_indsquare	pwr_indsquare51;
  pwr_valuelong	pwr_valuelong52;
  pwr_indsquare	pwr_indsquare53;
  pwr_valuelong	pwr_valuelong54;
  pwr_valuelong	pwr_valuelong55;
  pwr_indsquare	pwr_indsquare56;
  pwr_valuelong	pwr_valuelong57;
  Grp144_	grp144_58;
  Grp146_	grp146_59;
  Grp148_	grp148_60;
  Grp150_	grp150_61;
  Grp151_	grp151_62;
  Grp152_	grp152_63;
  Grp153_	grp153_64;
  Grp155_	grp155_65;
  pwr_indsquare	pwr_indsquare66;
  pwr_indsquare	pwr_indsquare68;
  pwr_valuelong	pwr_valuelong69;
  pwr_indsquare	pwr_indsquare70;
  pwr_valuelong	pwr_valuelong71;
  pwr_indsquare	pwr_indsquare72;
  pwr_valuelong	pwr_valuelong73;
  pwr_indsquare	pwr_indsquare74;
  pwr_valuelong	pwr_valuelong75;
  pwr_indsquare	pwr_indsquare76;
  pwr_valuelong	pwr_valuelong77;
  pwr_indsquare	pwr_indsquare78;
  pwr_valuelong	pwr_valuelong79;
  pwr_indsquare	pwr_indsquare80;
  pwr_valuelong	pwr_valuelong81;
  pwr_indsquare	pwr_indsquare82;
  pwr_valuelong	pwr_valuelong83;
  pwr_indsquare	pwr_indsquare84;
  pwr_valuelong	pwr_valuelong85;
  pwr_indsquare	pwr_indsquare86;
  pwr_valuelong	pwr_valuelong87;
  pwr_indsquare	pwr_indsquare88;
  pwr_valuelong	pwr_valuelong89;
  pwr_indsquare	pwr_indsquare90;
  pwr_valuelong	pwr_valuelong91;
  pwr_indsquare	pwr_indsquare92;
  pwr_valuelong	pwr_valuelong93;
  pwr_indsquare	pwr_indsquare94;
  pwr_valuelong	pwr_valuelong95;
  pwr_valuelong	pwr_valuelong96;
  pwr_indsquare	pwr_indsquare97;
  pwr_valuelong	pwr_valuelong98;
  pwr_indsquare	pwr_indsquare99;
  pwr_indsquare	pwr_indsquare108;
  pwr_valuelong	pwr_valuelong109;
  pwr_indsquare	pwr_indsquare110;
  pwr_valuelong	pwr_valuelong111;
  pwr_indsquare	pwr_indsquare112;
  pwr_valuelong	pwr_valuelong113;
  pwr_indsquare	pwr_indsquare114;
  pwr_valuelong	pwr_valuelong115;
  pwr_indsquare	pwr_indsquare116;
  pwr_valuelong	pwr_valuelong117;
  pwr_indsquare	pwr_indsquare118;
  pwr_valuelong	pwr_valuelong119;
  pwr_indsquare	pwr_indsquare120;
  pwr_valuelong	pwr_valuelong121;
  pwr_indsquare	pwr_indsquare122;
  pwr_valuelong	pwr_valuelong123;
  pwr_indsquare	pwr_indsquare124;
  pwr_valuelong	pwr_valuelong125;
  pwr_indsquare	pwr_indsquare126;
  pwr_valuelong	pwr_valuelong127;
  pwr_indsquare	pwr_indsquare128;
  pwr_valuelong	pwr_valuelong129;
  pwr_indsquare	pwr_indsquare130;
  pwr_valuelong	pwr_valuelong131;
  pwr_indsquare	pwr_indsquare132;
  pwr_valuelong	pwr_valuelong133;
  pwr_indsquare	pwr_indsquare134;
  pwr_valuelong	pwr_valuelong135;
  pwr_indsquare	pwr_indsquare136;
  pwr_valuelong	pwr_valuelong137;
  pwr_indsquare	pwr_indsquare138;
  pwr_valuelong	pwr_valuelong139;
  pwr_indsquare	pwr_indsquare140;
  pwr_valuelong	pwr_valuelong141;
  pwr_indsquare	pwr_indsquare142;
  pwr_valuelong	pwr_valuelong143;
  pwr_indsquare	pwr_indsquare144;
  pwr_valuelong	pwr_valuelong145;
  pwr_indsquare	pwr_indsquare146;
  pwr_valuelong	pwr_valuelong147;
  pwr_indsquare	pwr_indsquare148;
  pwr_valuelong	pwr_valuelong149;
  pwr_indsquare	pwr_indsquare150;
  pwr_valuelong	pwr_valuelong151;
  pwr_indsquare	pwr_indsquare152;
  pwr_valuelong	pwr_valuelong153;
  pwr_indsquare	pwr_indsquare154;
  pwr_valuelong	pwr_valuelong155;
  pwr_indsquare	pwr_indsquare156;
  pwr_valuelong	pwr_valuelong157;
  pwr_indsquare	pwr_indsquare158;
  pwr_valuelong	pwr_valuelong159;
  JopSmallbutton	jopSmallbutton160;
  JopSmallbutton	jopSmallbutton161;
  JopSmallbutton	jopSmallbutton162;
  JopSmallbutton	jopSmallbutton163;
  JopSmallbutton	jopSmallbutton164;
  JopSmallbutton	jopSmallbutton165;
  JopSmallbutton	jopSmallbutton166;
  JopSmallbutton	jopSmallbutton167;
  JopSmallbutton	jopSmallbutton168;
  JopSmallbutton	jopSmallbutton169;
  Grp310_	grp310_170;
  Grp311_	grp311_171;
  Grp312_	grp312_172;
  Grp319_	grp319_179;
  pwr_indsquare	pwr_indsquare180;
  pwr_valuelong	pwr_valuelong181;
  pwr_indsquare	pwr_indsquare182;
  pwr_valuelong	pwr_valuelong183;
  Grp330_	grp330_184;
  public JopcNode( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcNode( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "Œ-i@€l@œá@`ïÿ¿(ïÿ¿ß}");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 1012, 899);
    Dimension dsize = new Dimension(localPanel.original_width,localPanel.original_height);
    this.addComponentListener(new AspectRatioListener(this,size));
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    if ( scrollbar)
      contentPane.add( new JScrollPane(localPanel), BorderLayout.CENTER);
    else
      contentPane.add(localPanel, BorderLayout.CENTER);
    contentPane.setOpaque(true);
    localPanel.setLayout( new RatioLayout()); // scaletest
    localPanel.setOpaque(true);
    localPanel.setBackground(GeColor.getColor(51, GeColor.NO_COLOR));
    this.setSize(size);
    if ( engine.isInstance())
      setTitle( engine.getInstance());
    else
      this.setTitle("JopcNode");
    pwr_framethin3 = new pwr_framethin(session);
    pwr_framethin3.setBounds(new Rectangle(8,45,982,107));
    localPanel.add(pwr_framethin3, new Proportion(pwr_framethin3.getBounds(), dsize));
    pwr_valuelong4 = new pwr_valuelong(session);
    pwr_valuelong4.setBounds(new Rectangle(157,11,824,27));
    pwr_valuelong4.setFillColor(51);
    pwr_valuelong4.setBorderColor(56);
    localPanel.add(pwr_valuelong4, new Proportion(pwr_valuelong4.getBounds(), dsize));
    pwr_valuelong6 = new pwr_valuelong(session);
    pwr_valuelong6.setBounds(new Rectangle(141,56,842,21));
    pwr_valuelong6.setBorderColor(33);
    localPanel.add(pwr_valuelong6, new Proportion(pwr_valuelong6.getBounds(), dsize));
    pwr_valuelong7 = new pwr_valuelong(session);
    pwr_valuelong7.setBounds(new Rectangle(141,77,181,21));
    pwr_valuelong7.setBorderColor(33);
    localPanel.add(pwr_valuelong7, new Proportion(pwr_valuelong7.getBounds(), dsize));
    pwr_valuelong9 = new pwr_valuelong(session);
    pwr_valuelong9.setBounds(new Rectangle(141,98,181,21));
    pwr_valuelong9.setBorderColor(33);
    localPanel.add(pwr_valuelong9, new Proportion(pwr_valuelong9.getBounds(), dsize));
    pwr_valuelong11 = new pwr_valuelong(session);
    pwr_valuelong11.setBounds(new Rectangle(141,119,181,21));
    pwr_valuelong11.setBorderColor(33);
    localPanel.add(pwr_valuelong11, new Proportion(pwr_valuelong11.getBounds(), dsize));
    pwr_valuelong13 = new pwr_valuelong(session);
    pwr_valuelong13.setBounds(new Rectangle(500,77,181,21));
    pwr_valuelong13.setBorderColor(33);
    localPanel.add(pwr_valuelong13, new Proportion(pwr_valuelong13.getBounds(), dsize));
    pwr_valuelong15 = new pwr_valuelong(session);
    pwr_valuelong15.setBounds(new Rectangle(500,98,181,21));
    pwr_valuelong15.setBorderColor(33);
    localPanel.add(pwr_valuelong15, new Proportion(pwr_valuelong15.getBounds(), dsize));
    pwr_valuelong17 = new pwr_valuelong(session);
    pwr_valuelong17.setBounds(new Rectangle(500,119,181,21));
    pwr_valuelong17.setBorderColor(33);
    localPanel.add(pwr_valuelong17, new Proportion(pwr_valuelong17.getBounds(), dsize));
    pwr_valuelong19 = new pwr_valuelong(session);
    pwr_valuelong19.setBounds(new Rectangle(864,77,118,21));
    pwr_valuelong19.setBorderColor(33);
    localPanel.add(pwr_valuelong19, new Proportion(pwr_valuelong19.getBounds(), dsize));
    pwr_valuelong21 = new pwr_valuelong(session);
    pwr_valuelong21.setBounds(new Rectangle(864,98,118,21));
    pwr_valuelong21.setBorderColor(33);
    localPanel.add(pwr_valuelong21, new Proportion(pwr_valuelong21.getBounds(), dsize));
    pwr_framethin22 = new pwr_framethin(session);
    pwr_framethin22.setBounds(new Rectangle(7,160,983,687));
    localPanel.add(pwr_framethin22, new Proportion(pwr_framethin22.getBounds(), dsize));
    pwr_indsquare23 = new pwr_indsquare(session);
    pwr_indsquare23.setBounds(new Rectangle(156,167,18,19));
    localPanel.add(pwr_indsquare23, new Proportion(pwr_indsquare23.getBounds(), dsize));
    pwr_valuelong25 = new pwr_valuelong(session);
    pwr_valuelong25.setBounds(new Rectangle(185,167,795,21));
    pwr_valuelong25.setBorderColor(33);
    localPanel.add(pwr_valuelong25, new Proportion(pwr_valuelong25.getBounds(), dsize));
    pwr_indsquare26 = new pwr_indsquare(session);
    pwr_indsquare26.setBounds(new Rectangle(147,215,18,19));
    localPanel.add(pwr_indsquare26, new Proportion(pwr_indsquare26.getBounds(), dsize));
    pwr_valuelong27 = new pwr_valuelong(session);
    pwr_valuelong27.setBounds(new Rectangle(167,214,352,21));
    pwr_valuelong27.setBorderColor(33);
    localPanel.add(pwr_valuelong27, new Proportion(pwr_valuelong27.getBounds(), dsize));
    pwr_indsquare28 = new pwr_indsquare(session);
    pwr_indsquare28.setBounds(new Rectangle(147,234,18,19));
    localPanel.add(pwr_indsquare28, new Proportion(pwr_indsquare28.getBounds(), dsize));
    pwr_valuelong29 = new pwr_valuelong(session);
    pwr_valuelong29.setBounds(new Rectangle(167,233,352,21));
    pwr_valuelong29.setBorderColor(33);
    localPanel.add(pwr_valuelong29, new Proportion(pwr_valuelong29.getBounds(), dsize));
    pwr_indsquare30 = new pwr_indsquare(session);
    pwr_indsquare30.setBounds(new Rectangle(147,254,18,19));
    localPanel.add(pwr_indsquare30, new Proportion(pwr_indsquare30.getBounds(), dsize));
    pwr_valuelong31 = new pwr_valuelong(session);
    pwr_valuelong31.setBounds(new Rectangle(167,253,352,21));
    pwr_valuelong31.setBorderColor(33);
    localPanel.add(pwr_valuelong31, new Proportion(pwr_valuelong31.getBounds(), dsize));
    pwr_indsquare32 = new pwr_indsquare(session);
    pwr_indsquare32.setBounds(new Rectangle(147,274,18,19));
    localPanel.add(pwr_indsquare32, new Proportion(pwr_indsquare32.getBounds(), dsize));
    pwr_valuelong33 = new pwr_valuelong(session);
    pwr_valuelong33.setBounds(new Rectangle(167,273,352,21));
    pwr_valuelong33.setBorderColor(33);
    localPanel.add(pwr_valuelong33, new Proportion(pwr_valuelong33.getBounds(), dsize));
    pwr_indsquare34 = new pwr_indsquare(session);
    pwr_indsquare34.setBounds(new Rectangle(147,293,18,19));
    localPanel.add(pwr_indsquare34, new Proportion(pwr_indsquare34.getBounds(), dsize));
    pwr_valuelong35 = new pwr_valuelong(session);
    pwr_valuelong35.setBounds(new Rectangle(167,292,352,21));
    pwr_valuelong35.setBorderColor(33);
    localPanel.add(pwr_valuelong35, new Proportion(pwr_valuelong35.getBounds(), dsize));
    pwr_indsquare36 = new pwr_indsquare(session);
    pwr_indsquare36.setBounds(new Rectangle(147,313,18,19));
    localPanel.add(pwr_indsquare36, new Proportion(pwr_indsquare36.getBounds(), dsize));
    pwr_valuelong37 = new pwr_valuelong(session);
    pwr_valuelong37.setBounds(new Rectangle(167,312,352,21));
    pwr_valuelong37.setBorderColor(33);
    localPanel.add(pwr_valuelong37, new Proportion(pwr_valuelong37.getBounds(), dsize));
    pwr_indsquare38 = new pwr_indsquare(session);
    pwr_indsquare38.setBounds(new Rectangle(147,333,18,19));
    localPanel.add(pwr_indsquare38, new Proportion(pwr_indsquare38.getBounds(), dsize));
    pwr_valuelong39 = new pwr_valuelong(session);
    pwr_valuelong39.setBounds(new Rectangle(167,332,352,21));
    pwr_valuelong39.setBorderColor(33);
    localPanel.add(pwr_valuelong39, new Proportion(pwr_valuelong39.getBounds(), dsize));
    pwr_indsquare40 = new pwr_indsquare(session);
    pwr_indsquare40.setBounds(new Rectangle(147,353,18,19));
    localPanel.add(pwr_indsquare40, new Proportion(pwr_indsquare40.getBounds(), dsize));
    pwr_valuelong41 = new pwr_valuelong(session);
    pwr_valuelong41.setBounds(new Rectangle(167,351,352,21));
    pwr_valuelong41.setBorderColor(33);
    localPanel.add(pwr_valuelong41, new Proportion(pwr_valuelong41.getBounds(), dsize));
    pwr_indsquare42 = new pwr_indsquare(session);
    pwr_indsquare42.setBounds(new Rectangle(147,372,18,19));
    localPanel.add(pwr_indsquare42, new Proportion(pwr_indsquare42.getBounds(), dsize));
    pwr_valuelong43 = new pwr_valuelong(session);
    pwr_valuelong43.setBounds(new Rectangle(167,371,352,21));
    pwr_valuelong43.setBorderColor(33);
    localPanel.add(pwr_valuelong43, new Proportion(pwr_valuelong43.getBounds(), dsize));
    pwr_indsquare44 = new pwr_indsquare(session);
    pwr_indsquare44.setBounds(new Rectangle(147,392,18,19));
    localPanel.add(pwr_indsquare44, new Proportion(pwr_indsquare44.getBounds(), dsize));
    pwr_valuelong45 = new pwr_valuelong(session);
    pwr_valuelong45.setBounds(new Rectangle(167,391,352,21));
    pwr_valuelong45.setBorderColor(33);
    localPanel.add(pwr_valuelong45, new Proportion(pwr_valuelong45.getBounds(), dsize));
    pwr_indsquare46 = new pwr_indsquare(session);
    pwr_indsquare46.setBounds(new Rectangle(147,412,18,19));
    localPanel.add(pwr_indsquare46, new Proportion(pwr_indsquare46.getBounds(), dsize));
    pwr_valuelong47 = new pwr_valuelong(session);
    pwr_valuelong47.setBounds(new Rectangle(167,410,352,21));
    pwr_valuelong47.setBorderColor(33);
    localPanel.add(pwr_valuelong47, new Proportion(pwr_valuelong47.getBounds(), dsize));
    pwr_indsquare48 = new pwr_indsquare(session);
    pwr_indsquare48.setBounds(new Rectangle(147,431,18,19));
    localPanel.add(pwr_indsquare48, new Proportion(pwr_indsquare48.getBounds(), dsize));
    pwr_valuelong49 = new pwr_valuelong(session);
    pwr_valuelong49.setBounds(new Rectangle(167,430,352,21));
    pwr_valuelong49.setBorderColor(33);
    localPanel.add(pwr_valuelong49, new Proportion(pwr_valuelong49.getBounds(), dsize));
    grp110_50 = new Grp110_(session);
    grp110_50.setBounds(new Rectangle(14,430,130,22));
    localPanel.add(grp110_50, new Proportion(grp110_50.getBounds(), dsize));
    pwr_indsquare51 = new pwr_indsquare(session);
    pwr_indsquare51.setBounds(new Rectangle(147,451,18,19));
    localPanel.add(pwr_indsquare51, new Proportion(pwr_indsquare51.getBounds(), dsize));
    pwr_valuelong52 = new pwr_valuelong(session);
    pwr_valuelong52.setBounds(new Rectangle(167,450,352,21));
    pwr_valuelong52.setBorderColor(33);
    localPanel.add(pwr_valuelong52, new Proportion(pwr_valuelong52.getBounds(), dsize));
    pwr_indsquare53 = new pwr_indsquare(session);
    pwr_indsquare53.setBounds(new Rectangle(147,471,18,19));
    localPanel.add(pwr_indsquare53, new Proportion(pwr_indsquare53.getBounds(), dsize));
    pwr_valuelong54 = new pwr_valuelong(session);
    pwr_valuelong54.setBounds(new Rectangle(167,469,352,21));
    pwr_valuelong54.setBorderColor(33);
    localPanel.add(pwr_valuelong54, new Proportion(pwr_valuelong54.getBounds(), dsize));
    pwr_valuelong55 = new pwr_valuelong(session);
    pwr_valuelong55.setBounds(new Rectangle(167,489,352,21));
    pwr_valuelong55.setBorderColor(33);
    localPanel.add(pwr_valuelong55, new Proportion(pwr_valuelong55.getBounds(), dsize));
    pwr_indsquare56 = new pwr_indsquare(session);
    pwr_indsquare56.setBounds(new Rectangle(147,530,18,19));
    localPanel.add(pwr_indsquare56, new Proportion(pwr_indsquare56.getBounds(), dsize));
    pwr_valuelong57 = new pwr_valuelong(session);
    pwr_valuelong57.setBounds(new Rectangle(167,528,352,21));
    pwr_valuelong57.setBorderColor(33);
    localPanel.add(pwr_valuelong57, new Proportion(pwr_valuelong57.getBounds(), dsize));
    grp144_58 = new Grp144_(session);
    grp144_58.setBounds(new Rectangle(14,292,130,22));
    localPanel.add(grp144_58, new Proportion(grp144_58.getBounds(), dsize));
    grp146_59 = new Grp146_(session);
    grp146_59.setBounds(new Rectangle(14,332,130,22));
    localPanel.add(grp146_59, new Proportion(grp146_59.getBounds(), dsize));
    grp148_60 = new Grp148_(session);
    grp148_60.setBounds(new Rectangle(14,371,130,22));
    localPanel.add(grp148_60, new Proportion(grp148_60.getBounds(), dsize));
    grp150_61 = new Grp150_(session);
    grp150_61.setBounds(new Rectangle(14,391,130,22));
    localPanel.add(grp150_61, new Proportion(grp150_61.getBounds(), dsize));
    grp151_62 = new Grp151_(session);
    grp151_62.setBounds(new Rectangle(14,410,130,22));
    localPanel.add(grp151_62, new Proportion(grp151_62.getBounds(), dsize));
    grp152_63 = new Grp152_(session);
    grp152_63.setBounds(new Rectangle(14,450,130,22));
    localPanel.add(grp152_63, new Proportion(grp152_63.getBounds(), dsize));
    grp153_64 = new Grp153_(session);
    grp153_64.setBounds(new Rectangle(14,469,130,22));
    localPanel.add(grp153_64, new Proportion(grp153_64.getBounds(), dsize));
    grp155_65 = new Grp155_(session);
    grp155_65.setBounds(new Rectangle(14,528,130,22));
    localPanel.add(grp155_65, new Proportion(grp155_65.getBounds(), dsize));
    pwr_indsquare66 = new pwr_indsquare(session);
    pwr_indsquare66.setBounds(new Rectangle(147,490,18,19));
    localPanel.add(pwr_indsquare66, new Proportion(pwr_indsquare66.getBounds(), dsize));
    pwr_indsquare68 = new pwr_indsquare(session);
    pwr_indsquare68.setBounds(new Rectangle(533,215,18,19));
    localPanel.add(pwr_indsquare68, new Proportion(pwr_indsquare68.getBounds(), dsize));
    pwr_valuelong69 = new pwr_valuelong(session);
    pwr_valuelong69.setBounds(new Rectangle(552,214,428,21));
    pwr_valuelong69.setBorderColor(33);
    localPanel.add(pwr_valuelong69, new Proportion(pwr_valuelong69.getBounds(), dsize));
    pwr_indsquare70 = new pwr_indsquare(session);
    pwr_indsquare70.setBounds(new Rectangle(533,234,18,19));
    localPanel.add(pwr_indsquare70, new Proportion(pwr_indsquare70.getBounds(), dsize));
    pwr_valuelong71 = new pwr_valuelong(session);
    pwr_valuelong71.setBounds(new Rectangle(552,233,428,21));
    pwr_valuelong71.setBorderColor(33);
    localPanel.add(pwr_valuelong71, new Proportion(pwr_valuelong71.getBounds(), dsize));
    pwr_indsquare72 = new pwr_indsquare(session);
    pwr_indsquare72.setBounds(new Rectangle(533,254,18,19));
    localPanel.add(pwr_indsquare72, new Proportion(pwr_indsquare72.getBounds(), dsize));
    pwr_valuelong73 = new pwr_valuelong(session);
    pwr_valuelong73.setBounds(new Rectangle(552,253,428,21));
    pwr_valuelong73.setBorderColor(33);
    localPanel.add(pwr_valuelong73, new Proportion(pwr_valuelong73.getBounds(), dsize));
    pwr_indsquare74 = new pwr_indsquare(session);
    pwr_indsquare74.setBounds(new Rectangle(533,274,18,19));
    localPanel.add(pwr_indsquare74, new Proportion(pwr_indsquare74.getBounds(), dsize));
    pwr_valuelong75 = new pwr_valuelong(session);
    pwr_valuelong75.setBounds(new Rectangle(552,273,428,21));
    pwr_valuelong75.setBorderColor(33);
    localPanel.add(pwr_valuelong75, new Proportion(pwr_valuelong75.getBounds(), dsize));
    pwr_indsquare76 = new pwr_indsquare(session);
    pwr_indsquare76.setBounds(new Rectangle(533,293,18,19));
    localPanel.add(pwr_indsquare76, new Proportion(pwr_indsquare76.getBounds(), dsize));
    pwr_valuelong77 = new pwr_valuelong(session);
    pwr_valuelong77.setBounds(new Rectangle(552,292,428,21));
    pwr_valuelong77.setBorderColor(33);
    localPanel.add(pwr_valuelong77, new Proportion(pwr_valuelong77.getBounds(), dsize));
    pwr_indsquare78 = new pwr_indsquare(session);
    pwr_indsquare78.setBounds(new Rectangle(533,313,18,19));
    localPanel.add(pwr_indsquare78, new Proportion(pwr_indsquare78.getBounds(), dsize));
    pwr_valuelong79 = new pwr_valuelong(session);
    pwr_valuelong79.setBounds(new Rectangle(552,312,428,21));
    pwr_valuelong79.setBorderColor(33);
    localPanel.add(pwr_valuelong79, new Proportion(pwr_valuelong79.getBounds(), dsize));
    pwr_indsquare80 = new pwr_indsquare(session);
    pwr_indsquare80.setBounds(new Rectangle(533,333,18,19));
    localPanel.add(pwr_indsquare80, new Proportion(pwr_indsquare80.getBounds(), dsize));
    pwr_valuelong81 = new pwr_valuelong(session);
    pwr_valuelong81.setBounds(new Rectangle(552,332,428,21));
    pwr_valuelong81.setBorderColor(33);
    localPanel.add(pwr_valuelong81, new Proportion(pwr_valuelong81.getBounds(), dsize));
    pwr_indsquare82 = new pwr_indsquare(session);
    pwr_indsquare82.setBounds(new Rectangle(533,353,18,19));
    localPanel.add(pwr_indsquare82, new Proportion(pwr_indsquare82.getBounds(), dsize));
    pwr_valuelong83 = new pwr_valuelong(session);
    pwr_valuelong83.setBounds(new Rectangle(552,351,428,21));
    pwr_valuelong83.setBorderColor(33);
    localPanel.add(pwr_valuelong83, new Proportion(pwr_valuelong83.getBounds(), dsize));
    pwr_indsquare84 = new pwr_indsquare(session);
    pwr_indsquare84.setBounds(new Rectangle(533,372,18,19));
    localPanel.add(pwr_indsquare84, new Proportion(pwr_indsquare84.getBounds(), dsize));
    pwr_valuelong85 = new pwr_valuelong(session);
    pwr_valuelong85.setBounds(new Rectangle(552,371,428,21));
    pwr_valuelong85.setBorderColor(33);
    localPanel.add(pwr_valuelong85, new Proportion(pwr_valuelong85.getBounds(), dsize));
    pwr_indsquare86 = new pwr_indsquare(session);
    pwr_indsquare86.setBounds(new Rectangle(533,392,18,19));
    localPanel.add(pwr_indsquare86, new Proportion(pwr_indsquare86.getBounds(), dsize));
    pwr_valuelong87 = new pwr_valuelong(session);
    pwr_valuelong87.setBounds(new Rectangle(552,391,428,21));
    pwr_valuelong87.setBorderColor(33);
    localPanel.add(pwr_valuelong87, new Proportion(pwr_valuelong87.getBounds(), dsize));
    pwr_indsquare88 = new pwr_indsquare(session);
    pwr_indsquare88.setBounds(new Rectangle(533,412,18,19));
    localPanel.add(pwr_indsquare88, new Proportion(pwr_indsquare88.getBounds(), dsize));
    pwr_valuelong89 = new pwr_valuelong(session);
    pwr_valuelong89.setBounds(new Rectangle(552,410,428,21));
    pwr_valuelong89.setBorderColor(33);
    localPanel.add(pwr_valuelong89, new Proportion(pwr_valuelong89.getBounds(), dsize));
    pwr_indsquare90 = new pwr_indsquare(session);
    pwr_indsquare90.setBounds(new Rectangle(533,431,18,19));
    localPanel.add(pwr_indsquare90, new Proportion(pwr_indsquare90.getBounds(), dsize));
    pwr_valuelong91 = new pwr_valuelong(session);
    pwr_valuelong91.setBounds(new Rectangle(552,430,428,21));
    pwr_valuelong91.setBorderColor(33);
    localPanel.add(pwr_valuelong91, new Proportion(pwr_valuelong91.getBounds(), dsize));
    pwr_indsquare92 = new pwr_indsquare(session);
    pwr_indsquare92.setBounds(new Rectangle(533,451,18,19));
    localPanel.add(pwr_indsquare92, new Proportion(pwr_indsquare92.getBounds(), dsize));
    pwr_valuelong93 = new pwr_valuelong(session);
    pwr_valuelong93.setBounds(new Rectangle(552,450,428,21));
    pwr_valuelong93.setBorderColor(33);
    localPanel.add(pwr_valuelong93, new Proportion(pwr_valuelong93.getBounds(), dsize));
    pwr_indsquare94 = new pwr_indsquare(session);
    pwr_indsquare94.setBounds(new Rectangle(533,471,18,19));
    localPanel.add(pwr_indsquare94, new Proportion(pwr_indsquare94.getBounds(), dsize));
    pwr_valuelong95 = new pwr_valuelong(session);
    pwr_valuelong95.setBounds(new Rectangle(552,469,428,21));
    pwr_valuelong95.setBorderColor(33);
    localPanel.add(pwr_valuelong95, new Proportion(pwr_valuelong95.getBounds(), dsize));
    pwr_valuelong96 = new pwr_valuelong(session);
    pwr_valuelong96.setBounds(new Rectangle(552,489,428,21));
    pwr_valuelong96.setBorderColor(33);
    localPanel.add(pwr_valuelong96, new Proportion(pwr_valuelong96.getBounds(), dsize));
    pwr_indsquare97 = new pwr_indsquare(session);
    pwr_indsquare97.setBounds(new Rectangle(533,530,18,19));
    localPanel.add(pwr_indsquare97, new Proportion(pwr_indsquare97.getBounds(), dsize));
    pwr_valuelong98 = new pwr_valuelong(session);
    pwr_valuelong98.setBounds(new Rectangle(552,528,428,21));
    pwr_valuelong98.setBorderColor(33);
    localPanel.add(pwr_valuelong98, new Proportion(pwr_valuelong98.getBounds(), dsize));
    pwr_indsquare99 = new pwr_indsquare(session);
    pwr_indsquare99.setBounds(new Rectangle(533,490,18,19));
    localPanel.add(pwr_indsquare99, new Proportion(pwr_indsquare99.getBounds(), dsize));
    pwr_indsquare108 = new pwr_indsquare(session);
    pwr_indsquare108.setBounds(new Rectangle(147,549,18,19));
    localPanel.add(pwr_indsquare108, new Proportion(pwr_indsquare108.getBounds(), dsize));
    pwr_valuelong109 = new pwr_valuelong(session);
    pwr_valuelong109.setBounds(new Rectangle(167,548,352,21));
    pwr_valuelong109.setBorderColor(33);
    localPanel.add(pwr_valuelong109, new Proportion(pwr_valuelong109.getBounds(), dsize));
    pwr_indsquare110 = new pwr_indsquare(session);
    pwr_indsquare110.setBounds(new Rectangle(533,549,18,19));
    localPanel.add(pwr_indsquare110, new Proportion(pwr_indsquare110.getBounds(), dsize));
    pwr_valuelong111 = new pwr_valuelong(session);
    pwr_valuelong111.setBounds(new Rectangle(552,548,428,21));
    pwr_valuelong111.setBorderColor(33);
    localPanel.add(pwr_valuelong111, new Proportion(pwr_valuelong111.getBounds(), dsize));
    pwr_indsquare112 = new pwr_indsquare(session);
    pwr_indsquare112.setBounds(new Rectangle(147,569,18,19));
    localPanel.add(pwr_indsquare112, new Proportion(pwr_indsquare112.getBounds(), dsize));
    pwr_valuelong113 = new pwr_valuelong(session);
    pwr_valuelong113.setBounds(new Rectangle(167,568,352,21));
    pwr_valuelong113.setBorderColor(33);
    localPanel.add(pwr_valuelong113, new Proportion(pwr_valuelong113.getBounds(), dsize));
    pwr_indsquare114 = new pwr_indsquare(session);
    pwr_indsquare114.setBounds(new Rectangle(533,569,18,19));
    localPanel.add(pwr_indsquare114, new Proportion(pwr_indsquare114.getBounds(), dsize));
    pwr_valuelong115 = new pwr_valuelong(session);
    pwr_valuelong115.setBounds(new Rectangle(552,568,428,21));
    pwr_valuelong115.setBorderColor(33);
    localPanel.add(pwr_valuelong115, new Proportion(pwr_valuelong115.getBounds(), dsize));
    pwr_indsquare116 = new pwr_indsquare(session);
    pwr_indsquare116.setBounds(new Rectangle(147,589,18,19));
    localPanel.add(pwr_indsquare116, new Proportion(pwr_indsquare116.getBounds(), dsize));
    pwr_valuelong117 = new pwr_valuelong(session);
    pwr_valuelong117.setBounds(new Rectangle(167,587,352,21));
    pwr_valuelong117.setBorderColor(33);
    localPanel.add(pwr_valuelong117, new Proportion(pwr_valuelong117.getBounds(), dsize));
    pwr_indsquare118 = new pwr_indsquare(session);
    pwr_indsquare118.setBounds(new Rectangle(533,589,18,19));
    localPanel.add(pwr_indsquare118, new Proportion(pwr_indsquare118.getBounds(), dsize));
    pwr_valuelong119 = new pwr_valuelong(session);
    pwr_valuelong119.setBounds(new Rectangle(552,587,428,21));
    pwr_valuelong119.setBorderColor(33);
    localPanel.add(pwr_valuelong119, new Proportion(pwr_valuelong119.getBounds(), dsize));
    pwr_indsquare120 = new pwr_indsquare(session);
    pwr_indsquare120.setBounds(new Rectangle(147,639,18,19));
    localPanel.add(pwr_indsquare120, new Proportion(pwr_indsquare120.getBounds(), dsize));
    pwr_valuelong121 = new pwr_valuelong(session);
    pwr_valuelong121.setBounds(new Rectangle(167,638,352,21));
    pwr_valuelong121.setBorderColor(33);
    localPanel.add(pwr_valuelong121, new Proportion(pwr_valuelong121.getBounds(), dsize));
    pwr_indsquare122 = new pwr_indsquare(session);
    pwr_indsquare122.setBounds(new Rectangle(533,639,18,19));
    localPanel.add(pwr_indsquare122, new Proportion(pwr_indsquare122.getBounds(), dsize));
    pwr_valuelong123 = new pwr_valuelong(session);
    pwr_valuelong123.setBounds(new Rectangle(552,638,428,21));
    pwr_valuelong123.setBorderColor(33);
    localPanel.add(pwr_valuelong123, new Proportion(pwr_valuelong123.getBounds(), dsize));
    pwr_indsquare124 = new pwr_indsquare(session);
    pwr_indsquare124.setBounds(new Rectangle(147,659,18,19));
    localPanel.add(pwr_indsquare124, new Proportion(pwr_indsquare124.getBounds(), dsize));
    pwr_valuelong125 = new pwr_valuelong(session);
    pwr_valuelong125.setBounds(new Rectangle(167,658,352,21));
    pwr_valuelong125.setBorderColor(33);
    localPanel.add(pwr_valuelong125, new Proportion(pwr_valuelong125.getBounds(), dsize));
    pwr_indsquare126 = new pwr_indsquare(session);
    pwr_indsquare126.setBounds(new Rectangle(533,659,18,19));
    localPanel.add(pwr_indsquare126, new Proportion(pwr_indsquare126.getBounds(), dsize));
    pwr_valuelong127 = new pwr_valuelong(session);
    pwr_valuelong127.setBounds(new Rectangle(552,658,428,21));
    pwr_valuelong127.setBorderColor(33);
    localPanel.add(pwr_valuelong127, new Proportion(pwr_valuelong127.getBounds(), dsize));
    pwr_indsquare128 = new pwr_indsquare(session);
    pwr_indsquare128.setBounds(new Rectangle(147,678,18,19));
    localPanel.add(pwr_indsquare128, new Proportion(pwr_indsquare128.getBounds(), dsize));
    pwr_valuelong129 = new pwr_valuelong(session);
    pwr_valuelong129.setBounds(new Rectangle(167,677,352,21));
    pwr_valuelong129.setBorderColor(33);
    localPanel.add(pwr_valuelong129, new Proportion(pwr_valuelong129.getBounds(), dsize));
    pwr_indsquare130 = new pwr_indsquare(session);
    pwr_indsquare130.setBounds(new Rectangle(533,678,18,19));
    localPanel.add(pwr_indsquare130, new Proportion(pwr_indsquare130.getBounds(), dsize));
    pwr_valuelong131 = new pwr_valuelong(session);
    pwr_valuelong131.setBounds(new Rectangle(552,677,428,21));
    pwr_valuelong131.setBorderColor(33);
    localPanel.add(pwr_valuelong131, new Proportion(pwr_valuelong131.getBounds(), dsize));
    pwr_indsquare132 = new pwr_indsquare(session);
    pwr_indsquare132.setBounds(new Rectangle(147,698,18,19));
    localPanel.add(pwr_indsquare132, new Proportion(pwr_indsquare132.getBounds(), dsize));
    pwr_valuelong133 = new pwr_valuelong(session);
    pwr_valuelong133.setBounds(new Rectangle(167,697,352,21));
    pwr_valuelong133.setBorderColor(33);
    localPanel.add(pwr_valuelong133, new Proportion(pwr_valuelong133.getBounds(), dsize));
    pwr_indsquare134 = new pwr_indsquare(session);
    pwr_indsquare134.setBounds(new Rectangle(533,698,18,19));
    localPanel.add(pwr_indsquare134, new Proportion(pwr_indsquare134.getBounds(), dsize));
    pwr_valuelong135 = new pwr_valuelong(session);
    pwr_valuelong135.setBounds(new Rectangle(552,697,428,21));
    pwr_valuelong135.setBorderColor(33);
    localPanel.add(pwr_valuelong135, new Proportion(pwr_valuelong135.getBounds(), dsize));
    pwr_indsquare136 = new pwr_indsquare(session);
    pwr_indsquare136.setBounds(new Rectangle(147,718,18,19));
    localPanel.add(pwr_indsquare136, new Proportion(pwr_indsquare136.getBounds(), dsize));
    pwr_valuelong137 = new pwr_valuelong(session);
    pwr_valuelong137.setBounds(new Rectangle(167,717,352,21));
    pwr_valuelong137.setBorderColor(33);
    localPanel.add(pwr_valuelong137, new Proportion(pwr_valuelong137.getBounds(), dsize));
    pwr_indsquare138 = new pwr_indsquare(session);
    pwr_indsquare138.setBounds(new Rectangle(533,718,18,19));
    localPanel.add(pwr_indsquare138, new Proportion(pwr_indsquare138.getBounds(), dsize));
    pwr_valuelong139 = new pwr_valuelong(session);
    pwr_valuelong139.setBounds(new Rectangle(552,717,428,21));
    pwr_valuelong139.setBorderColor(33);
    localPanel.add(pwr_valuelong139, new Proportion(pwr_valuelong139.getBounds(), dsize));
    pwr_indsquare140 = new pwr_indsquare(session);
    pwr_indsquare140.setBounds(new Rectangle(147,738,18,19));
    localPanel.add(pwr_indsquare140, new Proportion(pwr_indsquare140.getBounds(), dsize));
    pwr_valuelong141 = new pwr_valuelong(session);
    pwr_valuelong141.setBounds(new Rectangle(167,736,352,21));
    pwr_valuelong141.setBorderColor(33);
    localPanel.add(pwr_valuelong141, new Proportion(pwr_valuelong141.getBounds(), dsize));
    pwr_indsquare142 = new pwr_indsquare(session);
    pwr_indsquare142.setBounds(new Rectangle(533,738,18,19));
    localPanel.add(pwr_indsquare142, new Proportion(pwr_indsquare142.getBounds(), dsize));
    pwr_valuelong143 = new pwr_valuelong(session);
    pwr_valuelong143.setBounds(new Rectangle(552,736,428,21));
    pwr_valuelong143.setBorderColor(33);
    localPanel.add(pwr_valuelong143, new Proportion(pwr_valuelong143.getBounds(), dsize));
    pwr_indsquare144 = new pwr_indsquare(session);
    pwr_indsquare144.setBounds(new Rectangle(147,757,18,19));
    localPanel.add(pwr_indsquare144, new Proportion(pwr_indsquare144.getBounds(), dsize));
    pwr_valuelong145 = new pwr_valuelong(session);
    pwr_valuelong145.setBounds(new Rectangle(167,756,352,21));
    pwr_valuelong145.setBorderColor(33);
    localPanel.add(pwr_valuelong145, new Proportion(pwr_valuelong145.getBounds(), dsize));
    pwr_indsquare146 = new pwr_indsquare(session);
    pwr_indsquare146.setBounds(new Rectangle(533,757,18,19));
    localPanel.add(pwr_indsquare146, new Proportion(pwr_indsquare146.getBounds(), dsize));
    pwr_valuelong147 = new pwr_valuelong(session);
    pwr_valuelong147.setBounds(new Rectangle(552,756,428,21));
    pwr_valuelong147.setBorderColor(33);
    localPanel.add(pwr_valuelong147, new Proportion(pwr_valuelong147.getBounds(), dsize));
    pwr_indsquare148 = new pwr_indsquare(session);
    pwr_indsquare148.setBounds(new Rectangle(147,777,18,19));
    localPanel.add(pwr_indsquare148, new Proportion(pwr_indsquare148.getBounds(), dsize));
    pwr_valuelong149 = new pwr_valuelong(session);
    pwr_valuelong149.setBounds(new Rectangle(167,776,352,21));
    pwr_valuelong149.setBorderColor(33);
    localPanel.add(pwr_valuelong149, new Proportion(pwr_valuelong149.getBounds(), dsize));
    pwr_indsquare150 = new pwr_indsquare(session);
    pwr_indsquare150.setBounds(new Rectangle(533,777,18,19));
    localPanel.add(pwr_indsquare150, new Proportion(pwr_indsquare150.getBounds(), dsize));
    pwr_valuelong151 = new pwr_valuelong(session);
    pwr_valuelong151.setBounds(new Rectangle(552,776,428,21));
    pwr_valuelong151.setBorderColor(33);
    localPanel.add(pwr_valuelong151, new Proportion(pwr_valuelong151.getBounds(), dsize));
    pwr_indsquare152 = new pwr_indsquare(session);
    pwr_indsquare152.setBounds(new Rectangle(147,797,18,19));
    localPanel.add(pwr_indsquare152, new Proportion(pwr_indsquare152.getBounds(), dsize));
    pwr_valuelong153 = new pwr_valuelong(session);
    pwr_valuelong153.setBounds(new Rectangle(167,795,352,21));
    pwr_valuelong153.setBorderColor(33);
    localPanel.add(pwr_valuelong153, new Proportion(pwr_valuelong153.getBounds(), dsize));
    pwr_indsquare154 = new pwr_indsquare(session);
    pwr_indsquare154.setBounds(new Rectangle(533,797,18,19));
    localPanel.add(pwr_indsquare154, new Proportion(pwr_indsquare154.getBounds(), dsize));
    pwr_valuelong155 = new pwr_valuelong(session);
    pwr_valuelong155.setBounds(new Rectangle(552,795,428,21));
    pwr_valuelong155.setBorderColor(33);
    localPanel.add(pwr_valuelong155, new Proportion(pwr_valuelong155.getBounds(), dsize));
    pwr_indsquare156 = new pwr_indsquare(session);
    pwr_indsquare156.setBounds(new Rectangle(147,816,18,19));
    localPanel.add(pwr_indsquare156, new Proportion(pwr_indsquare156.getBounds(), dsize));
    pwr_valuelong157 = new pwr_valuelong(session);
    pwr_valuelong157.setBounds(new Rectangle(167,815,352,21));
    pwr_valuelong157.setBorderColor(33);
    localPanel.add(pwr_valuelong157, new Proportion(pwr_valuelong157.getBounds(), dsize));
    pwr_indsquare158 = new pwr_indsquare(session);
    pwr_indsquare158.setBounds(new Rectangle(533,816,18,19));
    localPanel.add(pwr_indsquare158, new Proportion(pwr_indsquare158.getBounds(), dsize));
    pwr_valuelong159 = new pwr_valuelong(session);
    pwr_valuelong159.setBounds(new Rectangle(552,815,428,21));
    pwr_valuelong159.setBorderColor(33);
    localPanel.add(pwr_valuelong159, new Proportion(pwr_valuelong159.getBounds(), dsize));
    jopSmallbutton160 = new JopSmallbutton(session);
    jopSmallbutton160.setBounds(new Rectangle(14,639,129,23));
    jopSmallbutton160.setFillColor(31);
    localPanel.add(jopSmallbutton160, new Proportion(jopSmallbutton160.getBounds(), dsize));
    jopSmallbutton161 = new JopSmallbutton(session);
    jopSmallbutton161.setBounds(new Rectangle(14,659,129,23));
    jopSmallbutton161.setFillColor(31);
    localPanel.add(jopSmallbutton161, new Proportion(jopSmallbutton161.getBounds(), dsize));
    jopSmallbutton162 = new JopSmallbutton(session);
    jopSmallbutton162.setBounds(new Rectangle(14,678,129,23));
    jopSmallbutton162.setFillColor(31);
    localPanel.add(jopSmallbutton162, new Proportion(jopSmallbutton162.getBounds(), dsize));
    jopSmallbutton163 = new JopSmallbutton(session);
    jopSmallbutton163.setBounds(new Rectangle(14,697,129,23));
    jopSmallbutton163.setFillColor(31);
    localPanel.add(jopSmallbutton163, new Proportion(jopSmallbutton163.getBounds(), dsize));
    jopSmallbutton164 = new JopSmallbutton(session);
    jopSmallbutton164.setBounds(new Rectangle(14,717,129,23));
    jopSmallbutton164.setFillColor(31);
    localPanel.add(jopSmallbutton164, new Proportion(jopSmallbutton164.getBounds(), dsize));
    jopSmallbutton165 = new JopSmallbutton(session);
    jopSmallbutton165.setBounds(new Rectangle(14,736,129,23));
    jopSmallbutton165.setFillColor(31);
    localPanel.add(jopSmallbutton165, new Proportion(jopSmallbutton165.getBounds(), dsize));
    jopSmallbutton166 = new JopSmallbutton(session);
    jopSmallbutton166.setBounds(new Rectangle(14,756,129,23));
    jopSmallbutton166.setFillColor(31);
    localPanel.add(jopSmallbutton166, new Proportion(jopSmallbutton166.getBounds(), dsize));
    jopSmallbutton167 = new JopSmallbutton(session);
    jopSmallbutton167.setBounds(new Rectangle(14,775,129,23));
    jopSmallbutton167.setFillColor(31);
    localPanel.add(jopSmallbutton167, new Proportion(jopSmallbutton167.getBounds(), dsize));
    jopSmallbutton168 = new JopSmallbutton(session);
    jopSmallbutton168.setBounds(new Rectangle(14,794,129,23));
    jopSmallbutton168.setFillColor(31);
    localPanel.add(jopSmallbutton168, new Proportion(jopSmallbutton168.getBounds(), dsize));
    jopSmallbutton169 = new JopSmallbutton(session);
    jopSmallbutton169.setBounds(new Rectangle(14,814,129,23));
    jopSmallbutton169.setFillColor(31);
    localPanel.add(jopSmallbutton169, new Proportion(jopSmallbutton169.getBounds(), dsize));
    grp310_170 = new Grp310_(session);
    grp310_170.setBounds(new Rectangle(14,548,130,22));
    localPanel.add(grp310_170, new Proportion(grp310_170.getBounds(), dsize));
    grp311_171 = new Grp311_(session);
    grp311_171.setBounds(new Rectangle(14,568,130,22));
    localPanel.add(grp311_171, new Proportion(grp311_171.getBounds(), dsize));
    grp312_172 = new Grp312_(session);
    grp312_172.setBounds(new Rectangle(14,587,130,22));
    localPanel.add(grp312_172, new Proportion(grp312_172.getBounds(), dsize));
    grp319_179 = new Grp319_(session);
    grp319_179.setBounds(new Rectangle(14,489,130,22));
    localPanel.add(grp319_179, new Proportion(grp319_179.getBounds(), dsize));
    pwr_indsquare180 = new pwr_indsquare(session);
    pwr_indsquare180.setBounds(new Rectangle(147,510,18,19));
    localPanel.add(pwr_indsquare180, new Proportion(pwr_indsquare180.getBounds(), dsize));
    pwr_valuelong181 = new pwr_valuelong(session);
    pwr_valuelong181.setBounds(new Rectangle(167,509,352,21));
    pwr_valuelong181.setBorderColor(33);
    localPanel.add(pwr_valuelong181, new Proportion(pwr_valuelong181.getBounds(), dsize));
    pwr_indsquare182 = new pwr_indsquare(session);
    pwr_indsquare182.setBounds(new Rectangle(533,510,18,19));
    localPanel.add(pwr_indsquare182, new Proportion(pwr_indsquare182.getBounds(), dsize));
    pwr_valuelong183 = new pwr_valuelong(session);
    pwr_valuelong183.setBounds(new Rectangle(552,509,428,21));
    pwr_valuelong183.setBorderColor(33);
    localPanel.add(pwr_valuelong183, new Proportion(pwr_valuelong183.getBounds(), dsize));
    grp330_184 = new Grp330_(session);
    grp330_184.setBounds(new Rectangle(14,509,130,22));
    localPanel.add(grp330_184, new Proportion(grp330_184.getBounds(), dsize));
    pwr_framethin3.dd.setDynType(1);
    pwr_framethin3.dd.setActionType(0);
    pwr_valuelong4.dd.setDynType(1025);
    pwr_valuelong4.dd.setActionType(0);
    pwr_valuelong4.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong4.dd, "$object.Description##String80","%s")
      });
    pwr_valuelong6.dd.setDynType(1025);
    pwr_valuelong6.dd.setActionType(0);
    pwr_valuelong6.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong6.dd, "$object.ErrLogFile##String80","%s")
      });
    pwr_valuelong7.dd.setDynType(1025);
    pwr_valuelong7.dd.setActionType(0);
    pwr_valuelong7.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong7.dd, "$object.BootTime##Time","%t")
      });
    pwr_valuelong9.dd.setDynType(1025);
    pwr_valuelong9.dd.setActionType(0);
    pwr_valuelong9.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong9.dd, "$object.BootVersion##Time","%t")
      });
    pwr_valuelong11.dd.setDynType(1025);
    pwr_valuelong11.dd.setActionType(0);
    pwr_valuelong11.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong11.dd, "$object.BootPlcVersion##Int32","%d")
      });
    pwr_valuelong13.dd.setDynType(1025);
    pwr_valuelong13.dd.setActionType(0);
    pwr_valuelong13.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong13.dd, "$object.RestartTime##Time","%t")
      });
    pwr_valuelong15.dd.setDynType(1025);
    pwr_valuelong15.dd.setActionType(0);
    pwr_valuelong15.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong15.dd, "$object.CurrentVersion##Time","%t")
      });
    pwr_valuelong17.dd.setDynType(1025);
    pwr_valuelong17.dd.setActionType(0);
    pwr_valuelong17.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong17.dd, "$object.CurrentPlcVersion##Int32","%d")
      });
    pwr_valuelong19.dd.setDynType(1025);
    pwr_valuelong19.dd.setActionType(0);
    pwr_valuelong19.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong19.dd, "$object.Restarts##Int32","%d")
      });
    pwr_valuelong21.dd.setDynType(1025);
    pwr_valuelong21.dd.setActionType(0);
    pwr_valuelong21.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong21.dd, "$object.RestartStallTime##DeltaTime","%t")
      });
    pwr_framethin22.dd.setDynType(1);
    pwr_framethin22.dd.setActionType(0);
    pwr_indsquare23.dd.setDynType(33554432);
    pwr_indsquare23.dd.setActionType(0);
    pwr_indsquare23.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare23.dd, "$object.SystemStatus##NetStatus",28)
      });
    pwr_valuelong25.dd.setDynType(1025);
    pwr_valuelong25.dd.setActionType(0);
    pwr_valuelong25.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong25.dd, "$object.SystemStatus##NetStatus","%1m")
      });
    pwr_indsquare26.dd.setDynType(33554432);
    pwr_indsquare26.dd.setActionType(0);
    pwr_indsquare26.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare26.dd, "$object.ProcStatus[0]##status",28)
      });
    pwr_valuelong27.dd.setDynType(1025);
    pwr_valuelong27.dd.setActionType(0);
    pwr_valuelong27.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong27.dd, "$object.ProcStatus[0]##Status","%1m")
      });
    pwr_indsquare28.dd.setDynType(33554432);
    pwr_indsquare28.dd.setActionType(0);
    pwr_indsquare28.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare28.dd, "$object.ProcStatus[1]##status",38)
      });
    pwr_valuelong29.dd.setDynType(1025);
    pwr_valuelong29.dd.setActionType(0);
    pwr_valuelong29.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong29.dd, "$object.ProcStatus[1]##Status","%1m")
      });
    pwr_indsquare30.dd.setDynType(33554432);
    pwr_indsquare30.dd.setActionType(0);
    pwr_indsquare30.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare30.dd, "$object.ProcStatus[2]##status",38)
      });
    pwr_valuelong31.dd.setDynType(1025);
    pwr_valuelong31.dd.setActionType(0);
    pwr_valuelong31.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong31.dd, "$object.ProcStatus[2]##Status","%1m")
      });
    pwr_indsquare32.dd.setDynType(33554432);
    pwr_indsquare32.dd.setActionType(0);
    pwr_indsquare32.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare32.dd, "$object.ProcStatus[3]##status",38)
      });
    pwr_valuelong33.dd.setDynType(1025);
    pwr_valuelong33.dd.setActionType(0);
    pwr_valuelong33.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong33.dd, "$object.ProcStatus[3]##Status","%1m")
      });
    pwr_indsquare34.dd.setDynType(33554432);
    pwr_indsquare34.dd.setActionType(0);
    pwr_indsquare34.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare34.dd, "$object.ProcStatus[4]##status",38)
      });
    pwr_valuelong35.dd.setDynType(1025);
    pwr_valuelong35.dd.setActionType(0);
    pwr_valuelong35.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong35.dd, "$object.ProcStatus[4]##Status","%1m")
      });
    pwr_indsquare36.dd.setDynType(33554432);
    pwr_indsquare36.dd.setActionType(0);
    pwr_indsquare36.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare36.dd, "$object.ProcStatus[5]##status",38)
      });
    pwr_valuelong37.dd.setDynType(1025);
    pwr_valuelong37.dd.setActionType(0);
    pwr_valuelong37.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong37.dd, "$object.ProcStatus[5]##Status","%1m")
      });
    pwr_indsquare38.dd.setDynType(33554432);
    pwr_indsquare38.dd.setActionType(0);
    pwr_indsquare38.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare38.dd, "$object.ProcStatus[6]##status",38)
      });
    pwr_valuelong39.dd.setDynType(1025);
    pwr_valuelong39.dd.setActionType(0);
    pwr_valuelong39.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong39.dd, "$object.ProcStatus[6]##Status","%1m")
      });
    pwr_indsquare40.dd.setDynType(33554432);
    pwr_indsquare40.dd.setActionType(0);
    pwr_indsquare40.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare40.dd, "$object.ProcStatus[7]##status",38)
      });
    pwr_valuelong41.dd.setDynType(1025);
    pwr_valuelong41.dd.setActionType(0);
    pwr_valuelong41.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong41.dd, "$object.ProcStatus[7]##Status","%1m")
      });
    pwr_indsquare42.dd.setDynType(33554432);
    pwr_indsquare42.dd.setActionType(0);
    pwr_indsquare42.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare42.dd, "$object.ProcStatus[8]##status",38)
      });
    pwr_valuelong43.dd.setDynType(1025);
    pwr_valuelong43.dd.setActionType(0);
    pwr_valuelong43.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong43.dd, "$object.ProcStatus[8]##Status","%1m")
      });
    pwr_indsquare44.dd.setDynType(33554432);
    pwr_indsquare44.dd.setActionType(0);
    pwr_indsquare44.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare44.dd, "$object.ProcStatus[9]##status",38)
      });
    pwr_valuelong45.dd.setDynType(1025);
    pwr_valuelong45.dd.setActionType(0);
    pwr_valuelong45.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong45.dd, "$object.ProcStatus[9]##Status","%1m")
      });
    pwr_indsquare46.dd.setDynType(33554432);
    pwr_indsquare46.dd.setActionType(0);
    pwr_indsquare46.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare46.dd, "$object.ProcStatus[10]##status",38)
      });
    pwr_valuelong47.dd.setDynType(1025);
    pwr_valuelong47.dd.setActionType(0);
    pwr_valuelong47.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong47.dd, "$object.ProcStatus[10]##Status","%1m")
      });
    pwr_indsquare48.dd.setDynType(33554432);
    pwr_indsquare48.dd.setActionType(0);
    pwr_indsquare48.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare48.dd, "$object.ProcStatus[11]##status",38)
      });
    pwr_valuelong49.dd.setDynType(1025);
    pwr_valuelong49.dd.setActionType(0);
    pwr_valuelong49.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong49.dd, "$object.ProcStatus[11]##Status","%1m")
      });
    grp110_50.dd.setDynType(1);
    grp110_50.dd.setActionType(66);
    grp110_50.dd.setAccess(65535);
    grp110_50.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp110_50.dd, "&$object.ProcObject[11]")
      ,new GeDynCommand(grp110_50.dd, "open graph/classgraph/instance=&$object.ProcObject[11]")
      });
    pwr_indsquare51.dd.setDynType(33554432);
    pwr_indsquare51.dd.setActionType(0);
    pwr_indsquare51.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare51.dd, "$object.ProcStatus[12]##status",38)
      });
    pwr_valuelong52.dd.setDynType(1025);
    pwr_valuelong52.dd.setActionType(0);
    pwr_valuelong52.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong52.dd, "$object.ProcStatus[12]##Status","%1m")
      });
    pwr_indsquare53.dd.setDynType(33554432);
    pwr_indsquare53.dd.setActionType(0);
    pwr_indsquare53.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare53.dd, "$object.ProcStatus[13]##status",38)
      });
    pwr_valuelong54.dd.setDynType(1025);
    pwr_valuelong54.dd.setActionType(0);
    pwr_valuelong54.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong54.dd, "$object.ProcStatus[13]##Status","%1m")
      });
    pwr_valuelong55.dd.setDynType(1025);
    pwr_valuelong55.dd.setActionType(0);
    pwr_valuelong55.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong55.dd, "$object.ProcStatus[14]##Status","%1m")
      });
    pwr_indsquare56.dd.setDynType(33554432);
    pwr_indsquare56.dd.setActionType(0);
    pwr_indsquare56.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare56.dd, "$object.ProcStatus[16]##status",38)
      });
    pwr_valuelong57.dd.setDynType(1025);
    pwr_valuelong57.dd.setActionType(0);
    pwr_valuelong57.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong57.dd, "$object.ProcStatus[16]##status","%1m")
      });
    grp144_58.dd.setDynType(0);
    grp144_58.dd.setActionType(66);
    grp144_58.dd.setAccess(65535);
    grp144_58.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp144_58.dd, "&$object.ProcObject[4]")
      ,new GeDynCommand(grp144_58.dd, "open graph/classgraph/instance=&$object.ProcObject[4]")
      });
    grp146_59.dd.setDynType(0);
    grp146_59.dd.setActionType(66);
    grp146_59.dd.setAccess(65535);
    grp146_59.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp146_59.dd, "&$object.ProcObject[6]")
      ,new GeDynCommand(grp146_59.dd, "open graph/classgraph/instance=&$object.ProcObject[6]")
      });
    grp148_60.dd.setDynType(0);
    grp148_60.dd.setActionType(66);
    grp148_60.dd.setAccess(65535);
    grp148_60.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp148_60.dd, "&$object.ProcObject[8]")
      ,new GeDynCommand(grp148_60.dd, "open graph/classgraph/instance=&$object.ProcObject[8]")
      });
    grp150_61.dd.setDynType(0);
    grp150_61.dd.setActionType(66);
    grp150_61.dd.setAccess(65535);
    grp150_61.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp150_61.dd, "&$object.ProcObject[9]")
      ,new GeDynCommand(grp150_61.dd, "open graph/classgraph/instance=&$object.ProcObject[9]")
      });
    grp151_62.dd.setDynType(0);
    grp151_62.dd.setActionType(66);
    grp151_62.dd.setAccess(65535);
    grp151_62.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp151_62.dd, "&$object.ProcObject[10]")
      ,new GeDynCommand(grp151_62.dd, "open graph/classgraph/instance=&$object.ProcObject[10]")
      });
    grp152_63.dd.setDynType(0);
    grp152_63.dd.setActionType(66);
    grp152_63.dd.setAccess(65535);
    grp152_63.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp152_63.dd, "&$object.ProcObject[12]")
      ,new GeDynCommand(grp152_63.dd, "open graph/classgraph/instance=&$object.ProcObject[12]")
      });
    grp153_64.dd.setDynType(0);
    grp153_64.dd.setActionType(66);
    grp153_64.dd.setAccess(65535);
    grp153_64.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp153_64.dd, "&$object.ProcObject[13]")
      ,new GeDynCommand(grp153_64.dd, "open graph/classgraph/instance=&$object.ProcObject[13]")
      });
    grp155_65.dd.setDynType(0);
    grp155_65.dd.setActionType(66);
    grp155_65.dd.setAccess(65535);
    grp155_65.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp155_65.dd, "&$object.ProcObject[16]")
      ,new GeDynCommand(grp155_65.dd, "open graph/classgraph/instance=&$object.ProcObject[16]")
      });
    pwr_indsquare66.dd.setDynType(33554432);
    pwr_indsquare66.dd.setActionType(0);
    pwr_indsquare66.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare66.dd, "$object.ProcStatus[14]##status",28)
      });
    pwr_indsquare68.dd.setDynType(33554432);
    pwr_indsquare68.dd.setActionType(0);
    pwr_indsquare68.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare68.dd, "$object.ProcMsgSeverity[0]##status",28)
      });
    pwr_valuelong69.dd.setDynType(1025);
    pwr_valuelong69.dd.setActionType(0);
    pwr_valuelong69.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong69.dd, "$object.ProcMessage[0]##String80","%s")
      });
    pwr_indsquare70.dd.setDynType(33554432);
    pwr_indsquare70.dd.setActionType(0);
    pwr_indsquare70.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare70.dd, "$object.ProcMsgSeverity[1]##status",38)
      });
    pwr_valuelong71.dd.setDynType(1025);
    pwr_valuelong71.dd.setActionType(0);
    pwr_valuelong71.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong71.dd, "$object.ProcMessage[1]##String80","%s")
      });
    pwr_indsquare72.dd.setDynType(33554432);
    pwr_indsquare72.dd.setActionType(0);
    pwr_indsquare72.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare72.dd, "$object.ProcMsgSeverity[2]##status",38)
      });
    pwr_valuelong73.dd.setDynType(1025);
    pwr_valuelong73.dd.setActionType(0);
    pwr_valuelong73.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong73.dd, "$object.ProcMessage[2]##String80","%s")
      });
    pwr_indsquare74.dd.setDynType(33554432);
    pwr_indsquare74.dd.setActionType(0);
    pwr_indsquare74.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare74.dd, "$object.ProcMsgSeverity[3]##status",38)
      });
    pwr_valuelong75.dd.setDynType(1025);
    pwr_valuelong75.dd.setActionType(0);
    pwr_valuelong75.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong75.dd, "$object.ProcMessage[3]##String80","%s")
      });
    pwr_indsquare76.dd.setDynType(33554432);
    pwr_indsquare76.dd.setActionType(0);
    pwr_indsquare76.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare76.dd, "$object.ProcMsgSeverity[4]##status",38)
      });
    pwr_valuelong77.dd.setDynType(1025);
    pwr_valuelong77.dd.setActionType(0);
    pwr_valuelong77.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong77.dd, "$object.ProcMessage[4]##String80","%s")
      });
    pwr_indsquare78.dd.setDynType(33554432);
    pwr_indsquare78.dd.setActionType(0);
    pwr_indsquare78.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare78.dd, "$object.ProcMsgSeverity[5]##status",38)
      });
    pwr_valuelong79.dd.setDynType(1025);
    pwr_valuelong79.dd.setActionType(0);
    pwr_valuelong79.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong79.dd, "$object.ProcMessage[5]##String80","%s")
      });
    pwr_indsquare80.dd.setDynType(33554432);
    pwr_indsquare80.dd.setActionType(0);
    pwr_indsquare80.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare80.dd, "$object.ProcMsgSeverity[6]##status",38)
      });
    pwr_valuelong81.dd.setDynType(1025);
    pwr_valuelong81.dd.setActionType(0);
    pwr_valuelong81.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong81.dd, "$object.ProcMessage[6]##String80","%s")
      });
    pwr_indsquare82.dd.setDynType(33554432);
    pwr_indsquare82.dd.setActionType(0);
    pwr_indsquare82.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare82.dd, "$object.ProcMsgSeverity[7]##status",38)
      });
    pwr_valuelong83.dd.setDynType(1025);
    pwr_valuelong83.dd.setActionType(0);
    pwr_valuelong83.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong83.dd, "$object.ProcMessage[7]##String80","%s")
      });
    pwr_indsquare84.dd.setDynType(33554432);
    pwr_indsquare84.dd.setActionType(0);
    pwr_indsquare84.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare84.dd, "$object.ProcMsgSeverity[8]##status",38)
      });
    pwr_valuelong85.dd.setDynType(1025);
    pwr_valuelong85.dd.setActionType(0);
    pwr_valuelong85.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong85.dd, "$object.ProcMessage[8]##String80","%s")
      });
    pwr_indsquare86.dd.setDynType(33554432);
    pwr_indsquare86.dd.setActionType(0);
    pwr_indsquare86.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare86.dd, "$object.ProcMsgSeverity[9]##status",38)
      });
    pwr_valuelong87.dd.setDynType(1025);
    pwr_valuelong87.dd.setActionType(0);
    pwr_valuelong87.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong87.dd, "$object.ProcMessage[9]##String80","%s")
      });
    pwr_indsquare88.dd.setDynType(33554432);
    pwr_indsquare88.dd.setActionType(0);
    pwr_indsquare88.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare88.dd, "$object.ProcMsgSeverity[10]##status",38)
      });
    pwr_valuelong89.dd.setDynType(1025);
    pwr_valuelong89.dd.setActionType(0);
    pwr_valuelong89.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong89.dd, "$object.ProcMessage[10]##String80","%s")
      });
    pwr_indsquare90.dd.setDynType(33554432);
    pwr_indsquare90.dd.setActionType(0);
    pwr_indsquare90.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare90.dd, "$object.ProcMsgSeverity[11]##status",38)
      });
    pwr_valuelong91.dd.setDynType(1025);
    pwr_valuelong91.dd.setActionType(0);
    pwr_valuelong91.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong91.dd, "$object.ProcMessage[11]##String80","%s")
      });
    pwr_indsquare92.dd.setDynType(33554432);
    pwr_indsquare92.dd.setActionType(0);
    pwr_indsquare92.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare92.dd, "$object.ProcMsgSeverity[12]##status",38)
      });
    pwr_valuelong93.dd.setDynType(1025);
    pwr_valuelong93.dd.setActionType(0);
    pwr_valuelong93.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong93.dd, "$object.ProcMessage[12]##String80","%s")
      });
    pwr_indsquare94.dd.setDynType(33554432);
    pwr_indsquare94.dd.setActionType(0);
    pwr_indsquare94.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare94.dd, "$object.ProcMsgSeverity[13]##status",38)
      });
    pwr_valuelong95.dd.setDynType(1025);
    pwr_valuelong95.dd.setActionType(0);
    pwr_valuelong95.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong95.dd, "$object.ProcMessage[13]##String80","%s")
      });
    pwr_valuelong96.dd.setDynType(1025);
    pwr_valuelong96.dd.setActionType(0);
    pwr_valuelong96.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong96.dd, "$object.ProcMessage[14]##String80","%s")
      });
    pwr_indsquare97.dd.setDynType(33554432);
    pwr_indsquare97.dd.setActionType(0);
    pwr_indsquare97.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare97.dd, "$object.ProcMsgSeverity[16]##status",38)
      });
    pwr_valuelong98.dd.setDynType(1025);
    pwr_valuelong98.dd.setActionType(0);
    pwr_valuelong98.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong98.dd, "$object.ProcMessage[16]##string80","%s")
      });
    pwr_indsquare99.dd.setDynType(33554432);
    pwr_indsquare99.dd.setActionType(0);
    pwr_indsquare99.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare99.dd, "$object.ProcMsgSeverity[14]##status",28)
      });
    pwr_indsquare108.dd.setDynType(33554432);
    pwr_indsquare108.dd.setActionType(0);
    pwr_indsquare108.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare108.dd, "$object.ProcStatus[17]##status",38)
      });
    pwr_valuelong109.dd.setDynType(1025);
    pwr_valuelong109.dd.setActionType(0);
    pwr_valuelong109.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong109.dd, "$object.ProcStatus[17]##status","%1m")
      });
    pwr_indsquare110.dd.setDynType(33554432);
    pwr_indsquare110.dd.setActionType(0);
    pwr_indsquare110.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare110.dd, "$object.ProcMsgSeverity[17]##status",38)
      });
    pwr_valuelong111.dd.setDynType(1025);
    pwr_valuelong111.dd.setActionType(0);
    pwr_valuelong111.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong111.dd, "$object.ProcMessage[17]##string80","%s")
      });
    pwr_indsquare112.dd.setDynType(33554432);
    pwr_indsquare112.dd.setActionType(0);
    pwr_indsquare112.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare112.dd, "$object.ProcStatus[18]##status",38)
      });
    pwr_valuelong113.dd.setDynType(1025);
    pwr_valuelong113.dd.setActionType(0);
    pwr_valuelong113.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong113.dd, "$object.ProcStatus[18]##status","%1m")
      });
    pwr_indsquare114.dd.setDynType(33554432);
    pwr_indsquare114.dd.setActionType(0);
    pwr_indsquare114.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare114.dd, "$object.ProcMsgSeverity[18]##status",38)
      });
    pwr_valuelong115.dd.setDynType(1025);
    pwr_valuelong115.dd.setActionType(0);
    pwr_valuelong115.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong115.dd, "$object.ProcMessage[18]##string80","%s")
      });
    pwr_indsquare116.dd.setDynType(33554432);
    pwr_indsquare116.dd.setActionType(0);
    pwr_indsquare116.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare116.dd, "$object.ProcStatus[19]##status",38)
      });
    pwr_valuelong117.dd.setDynType(1025);
    pwr_valuelong117.dd.setActionType(0);
    pwr_valuelong117.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong117.dd, "$object.ProcStatus[19]##status","%1m")
      });
    pwr_indsquare118.dd.setDynType(33554432);
    pwr_indsquare118.dd.setActionType(0);
    pwr_indsquare118.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare118.dd, "$object.ProcMsgSeverity[19]##status",38)
      });
    pwr_valuelong119.dd.setDynType(1025);
    pwr_valuelong119.dd.setActionType(0);
    pwr_valuelong119.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong119.dd, "$object.ProcMessage[19]##string80","%s")
      });
    pwr_indsquare120.dd.setDynType(33554432);
    pwr_indsquare120.dd.setActionType(0);
    pwr_indsquare120.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare120.dd, "$object.ProcStatus[20]##status",38)
      });
    pwr_valuelong121.dd.setDynType(1025);
    pwr_valuelong121.dd.setActionType(0);
    pwr_valuelong121.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong121.dd, "$object.ProcStatus[20]##status","%1m")
      });
    pwr_indsquare122.dd.setDynType(33554432);
    pwr_indsquare122.dd.setActionType(0);
    pwr_indsquare122.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare122.dd, "$object.ProcMsgSeverity[20]##status",38)
      });
    pwr_valuelong123.dd.setDynType(1025);
    pwr_valuelong123.dd.setActionType(0);
    pwr_valuelong123.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong123.dd, "$object.ProcMessage[20]##string80","%s")
      });
    pwr_indsquare124.dd.setDynType(33554432);
    pwr_indsquare124.dd.setActionType(0);
    pwr_indsquare124.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare124.dd, "$object.ProcStatus[21]##status",38)
      });
    pwr_valuelong125.dd.setDynType(1025);
    pwr_valuelong125.dd.setActionType(0);
    pwr_valuelong125.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong125.dd, "$object.ProcStatus[21]##status","%1m")
      });
    pwr_indsquare126.dd.setDynType(33554432);
    pwr_indsquare126.dd.setActionType(0);
    pwr_indsquare126.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare126.dd, "$object.ProcMsgSeverity[21]##status",38)
      });
    pwr_valuelong127.dd.setDynType(1025);
    pwr_valuelong127.dd.setActionType(0);
    pwr_valuelong127.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong127.dd, "$object.ProcMessage[21]##string80","%s")
      });
    pwr_indsquare128.dd.setDynType(33554432);
    pwr_indsquare128.dd.setActionType(0);
    pwr_indsquare128.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare128.dd, "$object.ProcStatus[22]##status",38)
      });
    pwr_valuelong129.dd.setDynType(1025);
    pwr_valuelong129.dd.setActionType(0);
    pwr_valuelong129.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong129.dd, "$object.ProcStatus[22]##status","%1m")
      });
    pwr_indsquare130.dd.setDynType(33554432);
    pwr_indsquare130.dd.setActionType(0);
    pwr_indsquare130.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare130.dd, "$object.ProcMsgSeverity[22]##status",38)
      });
    pwr_valuelong131.dd.setDynType(1025);
    pwr_valuelong131.dd.setActionType(0);
    pwr_valuelong131.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong131.dd, "$object.ProcMessage[22]##string80","%s")
      });
    pwr_indsquare132.dd.setDynType(33554432);
    pwr_indsquare132.dd.setActionType(0);
    pwr_indsquare132.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare132.dd, "$object.ProcStatus[23]##status",38)
      });
    pwr_valuelong133.dd.setDynType(1025);
    pwr_valuelong133.dd.setActionType(0);
    pwr_valuelong133.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong133.dd, "$object.ProcStatus[23]##status","%1m")
      });
    pwr_indsquare134.dd.setDynType(33554432);
    pwr_indsquare134.dd.setActionType(0);
    pwr_indsquare134.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare134.dd, "$object.ProcMsgSeverity[23]##status",38)
      });
    pwr_valuelong135.dd.setDynType(1025);
    pwr_valuelong135.dd.setActionType(0);
    pwr_valuelong135.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong135.dd, "$object.ProcMessage[23]##string80","%s")
      });
    pwr_indsquare136.dd.setDynType(33554432);
    pwr_indsquare136.dd.setActionType(0);
    pwr_indsquare136.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare136.dd, "$object.ProcStatus[24]##status",38)
      });
    pwr_valuelong137.dd.setDynType(1025);
    pwr_valuelong137.dd.setActionType(0);
    pwr_valuelong137.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong137.dd, "$object.ProcStatus[24]##status","%1m")
      });
    pwr_indsquare138.dd.setDynType(33554432);
    pwr_indsquare138.dd.setActionType(0);
    pwr_indsquare138.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare138.dd, "$object.ProcMsgSeverity[24]##status",38)
      });
    pwr_valuelong139.dd.setDynType(1025);
    pwr_valuelong139.dd.setActionType(0);
    pwr_valuelong139.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong139.dd, "$object.ProcMessage[24]##string80","%s")
      });
    pwr_indsquare140.dd.setDynType(33554432);
    pwr_indsquare140.dd.setActionType(0);
    pwr_indsquare140.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare140.dd, "$object.ProcStatus[25]##status",38)
      });
    pwr_valuelong141.dd.setDynType(1025);
    pwr_valuelong141.dd.setActionType(0);
    pwr_valuelong141.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong141.dd, "$object.ProcStatus[25]##status","%1m")
      });
    pwr_indsquare142.dd.setDynType(33554432);
    pwr_indsquare142.dd.setActionType(0);
    pwr_indsquare142.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare142.dd, "$object.ProcMsgSeverity[25]##status",38)
      });
    pwr_valuelong143.dd.setDynType(1025);
    pwr_valuelong143.dd.setActionType(0);
    pwr_valuelong143.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong143.dd, "$object.ProcMessage[25]##string80","%s")
      });
    pwr_indsquare144.dd.setDynType(33554432);
    pwr_indsquare144.dd.setActionType(0);
    pwr_indsquare144.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare144.dd, "$object.ProcStatus[26]##status",38)
      });
    pwr_valuelong145.dd.setDynType(1025);
    pwr_valuelong145.dd.setActionType(0);
    pwr_valuelong145.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong145.dd, "$object.ProcStatus[26]##status","%1m")
      });
    pwr_indsquare146.dd.setDynType(33554432);
    pwr_indsquare146.dd.setActionType(0);
    pwr_indsquare146.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare146.dd, "$object.ProcMsgSeverity[26]##status",38)
      });
    pwr_valuelong147.dd.setDynType(1025);
    pwr_valuelong147.dd.setActionType(0);
    pwr_valuelong147.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong147.dd, "$object.ProcMessage[26]##string80","%s")
      });
    pwr_indsquare148.dd.setDynType(33554432);
    pwr_indsquare148.dd.setActionType(0);
    pwr_indsquare148.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare148.dd, "$object.ProcStatus[27]##status",38)
      });
    pwr_valuelong149.dd.setDynType(1025);
    pwr_valuelong149.dd.setActionType(0);
    pwr_valuelong149.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong149.dd, "$object.ProcStatus[27]##status","%1m")
      });
    pwr_indsquare150.dd.setDynType(33554432);
    pwr_indsquare150.dd.setActionType(0);
    pwr_indsquare150.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare150.dd, "$object.ProcMsgSeverity[27]##status",38)
      });
    pwr_valuelong151.dd.setDynType(1025);
    pwr_valuelong151.dd.setActionType(0);
    pwr_valuelong151.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong151.dd, "$object.ProcMessage[27]##string80","%s")
      });
    pwr_indsquare152.dd.setDynType(33554432);
    pwr_indsquare152.dd.setActionType(0);
    pwr_indsquare152.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare152.dd, "$object.ProcStatus[28]##status",38)
      });
    pwr_valuelong153.dd.setDynType(1025);
    pwr_valuelong153.dd.setActionType(0);
    pwr_valuelong153.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong153.dd, "$object.ProcStatus[28]##status","%1m")
      });
    pwr_indsquare154.dd.setDynType(33554432);
    pwr_indsquare154.dd.setActionType(0);
    pwr_indsquare154.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare154.dd, "$object.ProcMsgSeverity[28]##status",38)
      });
    pwr_valuelong155.dd.setDynType(1025);
    pwr_valuelong155.dd.setActionType(0);
    pwr_valuelong155.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong155.dd, "$object.ProcMessage[28]##string80","%s")
      });
    pwr_indsquare156.dd.setDynType(33554432);
    pwr_indsquare156.dd.setActionType(0);
    pwr_indsquare156.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare156.dd, "$object.ProcStatus[29]##status",38)
      });
    pwr_valuelong157.dd.setDynType(1025);
    pwr_valuelong157.dd.setActionType(0);
    pwr_valuelong157.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong157.dd, "$object.ProcStatus[29]##status","%1m")
      });
    pwr_indsquare158.dd.setDynType(33554432);
    pwr_indsquare158.dd.setActionType(0);
    pwr_indsquare158.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare158.dd, "$object.ProcMsgSeverity[29]##status",38)
      });
    pwr_valuelong159.dd.setDynType(1025);
    pwr_valuelong159.dd.setActionType(0);
    pwr_valuelong159.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong159.dd, "$object.ProcMessage[29]##string80","%s")
      });
    jopSmallbutton160.dd.setDynType(1024);
    jopSmallbutton160.dd.setActionType(64);
    jopSmallbutton160.dd.setAccess(65535);
    jopSmallbutton160.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton160.dd, "$object.ProcObject[20]##objid","%o")
      ,new GeDynCommand(jopSmallbutton160.dd, "open graph/classgraph/instance=&$object.ProcObject[20]")
      });
    jopSmallbutton161.dd.setDynType(1024);
    jopSmallbutton161.dd.setActionType(64);
    jopSmallbutton161.dd.setAccess(65535);
    jopSmallbutton161.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton161.dd, "$object.ProcObject[21]##objid","%o")
      ,new GeDynCommand(jopSmallbutton161.dd, "open graph/classgraph/instance=&$object.ProcObject[21]")
      });
    jopSmallbutton162.dd.setDynType(1024);
    jopSmallbutton162.dd.setActionType(64);
    jopSmallbutton162.dd.setAccess(65535);
    jopSmallbutton162.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton162.dd, "$object.ProcObject[22]##objid","%o")
      ,new GeDynCommand(jopSmallbutton162.dd, "open graph/classgraph/instance=&$object.ProcObject[22]")
      });
    jopSmallbutton163.dd.setDynType(1024);
    jopSmallbutton163.dd.setActionType(64);
    jopSmallbutton163.dd.setAccess(65535);
    jopSmallbutton163.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton163.dd, "$object.ProcObject[23]##objid","%o")
      ,new GeDynCommand(jopSmallbutton163.dd, "open graph/classgraph/instance=&$object.ProcObject[23]")
      });
    jopSmallbutton164.dd.setDynType(1024);
    jopSmallbutton164.dd.setActionType(64);
    jopSmallbutton164.dd.setAccess(65535);
    jopSmallbutton164.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton164.dd, "$object.ProcObject[24]##objid","%o")
      ,new GeDynCommand(jopSmallbutton164.dd, "open graph/classgraph/instance=&$object.ProcObject[24]")
      });
    jopSmallbutton165.dd.setDynType(1024);
    jopSmallbutton165.dd.setActionType(64);
    jopSmallbutton165.dd.setAccess(65535);
    jopSmallbutton165.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton165.dd, "$object.ProcObject[25]##objid","%o")
      ,new GeDynCommand(jopSmallbutton165.dd, "open graph/classgraph/instance=&$object.ProcObject[25]")
      });
    jopSmallbutton166.dd.setDynType(1024);
    jopSmallbutton166.dd.setActionType(64);
    jopSmallbutton166.dd.setAccess(65535);
    jopSmallbutton166.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton166.dd, "$object.ProcObject[26]##objid","%o")
      ,new GeDynCommand(jopSmallbutton166.dd, "open graph/classgraph/instance=&$object.ProcObject[26]")
      });
    jopSmallbutton167.dd.setDynType(1024);
    jopSmallbutton167.dd.setActionType(64);
    jopSmallbutton167.dd.setAccess(65535);
    jopSmallbutton167.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton167.dd, "$object.ProcObject[27]##objid","%o")
      ,new GeDynCommand(jopSmallbutton167.dd, "open graph/classgraph/instance=&$object.ProcObject[27]")
      });
    jopSmallbutton168.dd.setDynType(1024);
    jopSmallbutton168.dd.setActionType(64);
    jopSmallbutton168.dd.setAccess(65535);
    jopSmallbutton168.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton168.dd, "$object.ProcObject[28]##objid","%o")
      ,new GeDynCommand(jopSmallbutton168.dd, "open graph/classgraph/instance=&$object.ProcObject[28]")
      });
    jopSmallbutton169.dd.setDynType(1024);
    jopSmallbutton169.dd.setActionType(64);
    jopSmallbutton169.dd.setAccess(65535);
    jopSmallbutton169.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(jopSmallbutton169.dd, "$object.ProcObject[29]##objid","%o")
      ,new GeDynCommand(jopSmallbutton169.dd, "open graph/classgraph/instance=&$object.ProcObject[29]")
      });
    grp310_170.dd.setDynType(0);
    grp310_170.dd.setActionType(66);
    grp310_170.dd.setAccess(65535);
    grp310_170.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp310_170.dd, "&$object.ProcObject[17]")
      ,new GeDynCommand(grp310_170.dd, "open graph/classgraph/instance=&$object.ProcObject[17]")
      });
    grp311_171.dd.setDynType(0);
    grp311_171.dd.setActionType(66);
    grp311_171.dd.setAccess(65535);
    grp311_171.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp311_171.dd, "&$object.ProcObject[18]")
      ,new GeDynCommand(grp311_171.dd, "open graph/classgraph/instance=&$object.ProcObject[18]")
      });
    grp312_172.dd.setDynType(0);
    grp312_172.dd.setActionType(66);
    grp312_172.dd.setAccess(65535);
    grp312_172.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp312_172.dd, "&$object.ProcObject[19]")
      ,new GeDynCommand(grp312_172.dd, "open graph/classgraph/instance=&$object.ProcObject[19]")
      });
    grp319_179.dd.setDynType(0);
    grp319_179.dd.setActionType(66);
    grp319_179.dd.setAccess(65535);
    grp319_179.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp319_179.dd, "&$object.ProcObject[14]")
      ,new GeDynCommand(grp319_179.dd, "open graph/classgraph/instance=&$object.ProcObject[14]")
      });
    pwr_indsquare180.dd.setDynType(33554432);
    pwr_indsquare180.dd.setActionType(0);
    pwr_indsquare180.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare180.dd, "$object.ProcStatus[15]##status",38)
      });
    pwr_valuelong181.dd.setDynType(1025);
    pwr_valuelong181.dd.setActionType(0);
    pwr_valuelong181.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong181.dd, "$object.ProcStatus[15]##status","%1m")
      });
    pwr_indsquare182.dd.setDynType(33554432);
    pwr_indsquare182.dd.setActionType(0);
    pwr_indsquare182.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare182.dd, "$object.ProcMsgSeverity[15]##status",38)
      });
    pwr_valuelong183.dd.setDynType(1025);
    pwr_valuelong183.dd.setActionType(0);
    pwr_valuelong183.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong183.dd, "$object.ProcMessage[15]##string80","%s")
      });
    grp330_184.dd.setDynType(0);
    grp330_184.dd.setActionType(66);
    grp330_184.dd.setAccess(65535);
    grp330_184.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(grp330_184.dd, "&$object.ProcObject[15]")
      ,new GeDynCommand(grp330_184.dd, "open graph/classgraph/instance=&$object.ProcObject[15]")
      });
    engine.setFrameReady();
  }

class LocalPanel extends JPanel {
  public LocalPanel() {}
  int fillColor = 9999;
  int originalFillColor = 9999;
  int textColor = 9999;
  int originalTextColor = 9999;
  int borderColor = 9999;
  int colorTone = 0;
  int originalColorTone = 0;
  int colorShift = 0;
  int originalColorShift = 0;
  int colorBrightness = 0;
  int originalColorBrightness = 0;
  int colorIntensity = 0;
  int originalColorIntensity = 0;
  int colorInverse = 0;
  int originalColorInverse = 0;
  int shadow = 0;
  boolean dimmed = false;
  public void setColorTone( int colorTone) {
    this.colorTone = colorTone;
    originalColorTone = colorTone;
  }
  public int getColorTone() {
    return colorTone;
  }
  public void setColorShift( int colorShift) {
    this.colorShift = colorShift;
    originalColorShift = colorShift;
  }
  public int getColorShift() {
    return colorShift;
  }
  public void setColorBrightness( int colorBrightness) {
    this.colorBrightness = colorBrightness;
    originalColorBrightness = colorBrightness;
  }
  public int getColorBrightness() {
    return colorBrightness;
  }
  public void setColorIntensity( int colorIntensity) {
    this.colorIntensity = colorIntensity;
    originalColorIntensity = colorIntensity;
  }
  public int getColorIntensity() {
    return colorIntensity;
  }
  public void setFillColor( int fillColor) {
    this.fillColor = fillColor;
    this.originalFillColor = fillColor;
  }
  public void resetFillColor() {
    fillColor = originalFillColor;
  }
  public int getFillColor() {
    return fillColor;
  }
  public void setBorderColor( int borderColor) {
    this.borderColor = borderColor;
  }
  public int getBorderColor() {
    return borderColor;
  }
  public int original_width = 1004;
  public int original_height = 855;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Line2D.Float( 988.47F, 211.102F, 10.6101F, 211.102F),
    new Line2D.Float( 988.47F, 636.686F, 10.6101F, 636.686F),
    new Line2D.Float( 988.47F, 615.776F, 10.6101F, 615.776F),
    new Line2D.Float( 988.47F, 191.422F, 10.6101F, 191.422F),
  };
  public void paint(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    Component c;
    Point p;
    paintComponent(g);
    for ( int i = 0; i < getComponentCount(); i++) {
      AffineTransform save = g.getTransform();
      c = getComponent(i);
      p = c.getLocation();
      g.translate((int)p.getX(), (int)p.getY());
      c.paint(g);
      g.setTransform(save);
    }
  }
  public void paintComponent(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    double scaleWidth =  (1.0*width/original_width);
    double scaleHeight = (1.0*height/original_height);
    AffineTransform save = g.getTransform();
    g.setColor(getBackground());
    g.fill(new Rectangle(0,0,getWidth(),getHeight()));
    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight)); // scaletest
    AffineTransform save_tmp;
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 18));
    g.drawString( "Description",20, 31);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "ErrLogFile",20, 74);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "BootTime",20, 94);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "BootVersion",20, 115);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "BootPlcVersion",20, 136);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "RestartTime",357, 94);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "CurrentVersion",357, 115);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "CurrentPlcVersion",357, 136);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Restarts",721, 94);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "RestartStallTime",721, 115);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "SystemStatus",20, 182);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Process",20, 207);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Status",149, 207);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Log message",535, 207);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_ini",28, 230);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_qmon",28, 250);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_neth_acp",28, 289);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_tmon",28, 329);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_alim",28, 368);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_neth",28, 270);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Application",20, 632);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[1]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[2]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Status",149, 632);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Log message",535, 631);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_framethin extends GeFrameThin {
  public pwr_framethin( JopSession session)
  {
     super(session);
  }
}
protected class pwr_indsquare extends GeComponent {
  Dimension size;
  public pwr_indsquare( JopSession session)
  {
    super( session);
    size = new Dimension( 18, 19);
  }
  public int original_width = 18;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 14.7601F, 15.9902F),
    new Polygon( new int[] { 2, 17, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 17, 17, 15, 15, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(293, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuelong extends GeComponent {
  Dimension size;
  public pwr_valuelong( JopSession session)
  {
    super( session);
    size = new Dimension( 658, 21);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 12);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 658;
  public int original_height = 21;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 654.367F, 17.2202F),
    new Polygon( new int[] { 2, 656, 656, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 656, 656, 656, 656, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(41, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 9 * original_height / height * width / original_width, 16F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class JopSmallbutton extends GeComponent {
  Dimension size;
  public JopSmallbutton( JopSession session)
  {
    super( session);
    size = new Dimension( 58, 24);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 58;
  public int original_height = 24;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 54.1205F, 20.9102F),
    new Polygon( new int[] { 2, 56, 53, 5, 5, 2}, new int[] { 2, 2, 5, 5, 20, 23}, 6), 
    new Polygon( new int[] { 56, 56, 53, 53, 5, 2}, new int[] { 23, 2, 5, 20, 20, 23}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(102, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 8 * original_height / height * width / original_width, 17F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp110_ extends GeComponent {
  Dimension size;
  public Grp110_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_fast",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp144_ extends GeComponent {
  Dimension size;
  public Grp144_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_io",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp146_ extends GeComponent {
  Dimension size;
  public Grp146_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_emon",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp148_ extends GeComponent {
  Dimension size;
  public Grp148_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_bck",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp150_ extends GeComponent {
  Dimension size;
  public Grp150_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_linksup",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp151_ extends GeComponent {
  Dimension size;
  public Grp151_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_trend",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp152_ extends GeComponent {
  Dimension size;
  public Grp152_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_elog",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp153_ extends GeComponent {
  Dimension size;
  public Grp153_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_webmon",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp155_ extends GeComponent {
  Dimension size;
  public Grp155_( JopSession session)
  {
    super( session);
    size = new Dimension( 91, 22);
  }
  public int original_width = 91;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 87.3309F, 18.4502F),
    new Polygon( new int[] { 2, 89, 86, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 89, 89, 86, 86, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "plc",9, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp310_ extends GeComponent {
  Dimension size;
  public Grp310_( JopSession session)
  {
    super( session);
    size = new Dimension( 130, 22);
  }
  public int original_width = 130;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 126.034F, 18.4502F),
    new Polygon( new int[] { 2, 128, 125, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 128, 128, 125, 125, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rs_remote",13, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp311_ extends GeComponent {
  Dimension size;
  public Grp311_( JopSession session)
  {
    super( session);
    size = new Dimension( 130, 22);
  }
  public int original_width = 130;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 126.034F, 18.4502F),
    new Polygon( new int[] { 2, 128, 125, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 128, 128, 125, 125, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rs_nmps_bck",13, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp312_ extends GeComponent {
  Dimension size;
  public Grp312_( JopSession session)
  {
    super( session);
    size = new Dimension( 130, 22);
  }
  public int original_width = 130;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 126.034F, 18.4502F),
    new Polygon( new int[] { 2, 128, 125, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 128, 128, 125, 125, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rs_nmps_trans",13, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp319_ extends GeComponent {
  Dimension size;
  public Grp319_( JopSession session)
  {
    super( session);
    size = new Dimension( 130, 22);
  }
  public int original_width = 130;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 126.034F, 18.4502F),
    new Polygon( new int[] { 2, 128, 125, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 128, 128, 125, 125, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_webmonmh",13, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp330_ extends GeComponent {
  Dimension size;
  public Grp330_( JopSession session)
  {
    super( session);
    size = new Dimension( 130, 22);
  }
  public int original_width = 130;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 126.034F, 18.4502F),
    new Polygon( new int[] { 2, 128, 125, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 20}, 6), 
    new Polygon( new int[] { 128, 128, 125, 125, 5, 2}, new int[] { 20, 2, 5, 17, 17, 20}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "rt_sysmon",13, 17);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
  public int getUtilityType() {
    return JopUtility.GRAPH;
  }
  public PwrtObjid getUtilityObjid() {
    return utilityObjid;
  }
  public String getUtilityName() {
    return this.getClass().getName();
  }
}
