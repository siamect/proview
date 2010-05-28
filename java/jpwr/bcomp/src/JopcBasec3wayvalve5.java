package jpwr.bcomp;
import jpwr.rt.*;
import jpwr.jop.*;
import jpwr.jopc.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcBasec3wayvalve5 extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  public LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_valuelong	pwr_valuelong5;
  pwr_menubar2	pwr_menubar26;
  pwr_pulldownmenu2	pwr_pulldownmenu27;
  pwr_pulldownmenu2	pwr_pulldownmenu28;
  pwr_mbopenobject	pwr_mbopenobject9;
  pwr_mbtrend	pwr_mbtrend10;
  pwr_mbfast	pwr_mbfast11;
  pwr_mbphoto	pwr_mbphoto12;
  pwr_mbdatasheet	pwr_mbdatasheet13;
  pwr_mbopenplc	pwr_mbopenplc14;
  pwr_mbcircuitdiagram	pwr_mbcircuitdiagram15;
  pwr_mbrtnavigator	pwr_mbrtnavigator16;
  pwr_mbhelpclass	pwr_mbhelpclass17;
  pwr_mbblockevents	pwr_mbblockevents18;
  pwr_mbhistevent	pwr_mbhistevent19;
  pwr_mbnote	pwr_mbnote20;
  pwr_mbhelp	pwr_mbhelp21;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter22;
  pwr_valuelong	pwr_valuelong23;
  pwr_valuelong	pwr_valuelong24;
  pwr_valuesmall	pwr_valuesmall26;
  pwr_sliderbackground3	pwr_sliderbackground327;
  pwr_valueinputreliefup	pwr_valueinputreliefup28;
  pwr_valueinputreliefup	pwr_valueinputreliefup29;
  pwr_pulldownmenu2	pwr_pulldownmenu230;
  pwr_pulldownmenu2	pwr_pulldownmenu231;
  pwr_indsquare	pwr_indsquare34;
  JopBar	jopBar40;
  JopBar	jopBar41;
  JopAxis	jopAxis42;
  pwr_valuesmall	pwr_valuesmall43;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter44;
  pwr_indround	pwr_indround45;
  pwr_indround	pwr_indround46;
  pwr_sliderbackground1_2	pwr_sliderbackground1_248;
  pwr_slider1	pwr_slider149;
  pwr_valueinputsmallrd	pwr_valueinputsmallrd50;
  pwr_indsquare	pwr_indsquare53;
  pwr_indsquare	pwr_indsquare54;
  Grp167_	grp167_55;
  pwr_mbsimulate	pwr_mbsimulate56;
  pwr_actuator	pwr_actuator57;
  pwr_pulldownmenu2	pwr_pulldownmenu260;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter61;
  pwr_indround	pwr_indround62;
  Grp175_	grp175_63;
  pwr_indsquare	pwr_indsquare64;
  pwr_valve3waycontrol2	pwr_valve3waycontrol267;
  pwr_base3wayvalve	pwr_base3wayvalve69;
  pwr_base3wayvalve2	pwr_base3wayvalve270;
  pwr_mbup	pwr_mbup71;
  pwr_indsquare	pwr_indsquare73;
  public JopcBasec3wayvalve5( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcBasec3wayvalve5( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "ä×˜·Dað·Dað·p—uÄo˜·Kž· Cž·xŠµ¿ç¹•·Dað·Dað·x—u Cž·");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 538, 733);
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
    localPanel.setBackground(GeColor.getColor(31, GeColor.NO_COLOR));
    this.setSize(size);
    if ( engine.isInstance())
      setTitle( engine.getInstance());
    else
      this.setTitle("JopcBasec3wayvalve5");
    pwr_valuelong5 = new pwr_valuelong(session);
    pwr_valuelong5.setBounds(new Rectangle(16,46,385,22));
    pwr_valuelong5.setFillColor(31);
    pwr_valuelong5.setBorderColor(32);
    localPanel.add(pwr_valuelong5, new Proportion(pwr_valuelong5.getBounds(), dsize));
    pwr_menubar26 = new pwr_menubar2(session);
    pwr_menubar26.setBounds(new Rectangle(0,0,529,24));
    pwr_menubar26.setShadow(1);
    localPanel.add(pwr_menubar26, new Proportion(pwr_menubar26.getBounds(), dsize));
    pwr_pulldownmenu27 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu27.setBounds(new Rectangle(11,0,69,24));
    pwr_pulldownmenu27.setShadow(1);
    localPanel.add(pwr_pulldownmenu27, new Proportion(pwr_pulldownmenu27.getBounds(), dsize));
    pwr_pulldownmenu28 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu28.setBounds(new Rectangle(446,0,69,24));
    pwr_pulldownmenu28.setShadow(1);
    localPanel.add(pwr_pulldownmenu28, new Proportion(pwr_pulldownmenu28.getBounds(), dsize));
    pwr_mbopenobject9 = new pwr_mbopenobject(session);
    pwr_mbopenobject9.setBounds(new Rectangle(195,24,20,20));
    pwr_mbopenobject9.setShadow(1);
    localPanel.add(pwr_mbopenobject9, new Proportion(pwr_mbopenobject9.getBounds(), dsize));
    pwr_mbtrend10 = new pwr_mbtrend(session);
    pwr_mbtrend10.setBounds(new Rectangle(56,24,20,20));
    pwr_mbtrend10.setShadow(1);
    localPanel.add(pwr_mbtrend10, new Proportion(pwr_mbtrend10.getBounds(), dsize));
    pwr_mbfast11 = new pwr_mbfast(session);
    pwr_mbfast11.setBounds(new Rectangle(76,24,20,20));
    pwr_mbfast11.setShadow(1);
    localPanel.add(pwr_mbfast11, new Proportion(pwr_mbfast11.getBounds(), dsize));
    pwr_mbphoto12 = new pwr_mbphoto(session);
    pwr_mbphoto12.setBounds(new Rectangle(95,24,20,20));
    pwr_mbphoto12.setShadow(1);
    localPanel.add(pwr_mbphoto12, new Proportion(pwr_mbphoto12.getBounds(), dsize));
    pwr_mbdatasheet13 = new pwr_mbdatasheet(session);
    pwr_mbdatasheet13.setBounds(new Rectangle(115,24,20,20));
    pwr_mbdatasheet13.setShadow(1);
    localPanel.add(pwr_mbdatasheet13, new Proportion(pwr_mbdatasheet13.getBounds(), dsize));
    pwr_mbopenplc14 = new pwr_mbopenplc(session);
    pwr_mbopenplc14.setBounds(new Rectangle(214,24,20,20));
    pwr_mbopenplc14.setShadow(1);
    localPanel.add(pwr_mbopenplc14, new Proportion(pwr_mbopenplc14.getBounds(), dsize));
    pwr_mbcircuitdiagram15 = new pwr_mbcircuitdiagram(session);
    pwr_mbcircuitdiagram15.setBounds(new Rectangle(234,24,20,20));
    pwr_mbcircuitdiagram15.setShadow(1);
    localPanel.add(pwr_mbcircuitdiagram15, new Proportion(pwr_mbcircuitdiagram15.getBounds(), dsize));
    pwr_mbrtnavigator16 = new pwr_mbrtnavigator(session);
    pwr_mbrtnavigator16.setBounds(new Rectangle(175,24,20,20));
    pwr_mbrtnavigator16.setShadow(1);
    localPanel.add(pwr_mbrtnavigator16, new Proportion(pwr_mbrtnavigator16.getBounds(), dsize));
    pwr_mbhelpclass17 = new pwr_mbhelpclass(session);
    pwr_mbhelpclass17.setBounds(new Rectangle(254,24,20,20));
    pwr_mbhelpclass17.setShadow(1);
    localPanel.add(pwr_mbhelpclass17, new Proportion(pwr_mbhelpclass17.getBounds(), dsize));
    pwr_mbblockevents18 = new pwr_mbblockevents(session);
    pwr_mbblockevents18.setBounds(new Rectangle(155,24,20,20));
    pwr_mbblockevents18.setShadow(1);
    localPanel.add(pwr_mbblockevents18, new Proportion(pwr_mbblockevents18.getBounds(), dsize));
    pwr_mbhistevent19 = new pwr_mbhistevent(session);
    pwr_mbhistevent19.setBounds(new Rectangle(135,24,20,20));
    localPanel.add(pwr_mbhistevent19, new Proportion(pwr_mbhistevent19.getBounds(), dsize));
    pwr_mbnote20 = new pwr_mbnote(session);
    pwr_mbnote20.setBounds(new Rectangle(36,24,19,20));
    localPanel.add(pwr_mbnote20, new Proportion(pwr_mbnote20.getBounds(), dsize));
    pwr_mbhelp21 = new pwr_mbhelp(session);
    pwr_mbhelp21.setBounds(new Rectangle(16,24,20,20));
    localPanel.add(pwr_mbhelp21, new Proportion(pwr_mbhelp21.getBounds(), dsize));
    pwr_smallbuttoncenter22 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter22.setBounds(new Rectangle(17,667,47,21));
    pwr_smallbuttoncenter22.setFillColor(102);
    pwr_smallbuttoncenter22.setBorderColor(37);
    localPanel.add(pwr_smallbuttoncenter22, new Proportion(pwr_smallbuttoncenter22.getBounds(), dsize));
    pwr_valuelong23 = new pwr_valuelong(session);
    pwr_valuelong23.setBounds(new Rectangle(16,70,385,22));
    pwr_valuelong23.setFillColor(31);
    pwr_valuelong23.setBorderColor(32);
    localPanel.add(pwr_valuelong23, new Proportion(pwr_valuelong23.getBounds(), dsize));
    pwr_valuelong24 = new pwr_valuelong(session);
    pwr_valuelong24.setBounds(new Rectangle(74,666,336,23));
    pwr_valuelong24.setFillColor(31);
    pwr_valuelong24.setBorderColor(92);
    localPanel.add(pwr_valuelong24, new Proportion(pwr_valuelong24.getBounds(), dsize));
    pwr_valuesmall26 = new pwr_valuesmall(session);
    pwr_valuesmall26.setBounds(new Rectangle(340,391,50,17));
    localPanel.add(pwr_valuesmall26, new Proportion(pwr_valuesmall26.getBounds(), dsize));
    pwr_sliderbackground327 = new pwr_sliderbackground3(session);
    pwr_sliderbackground327.setBounds(new Rectangle(337,132,120,243));
    pwr_sliderbackground327.setFillColor(35);
    localPanel.add(pwr_sliderbackground327, new Proportion(pwr_sliderbackground327.getBounds(), dsize));
    pwr_valueinputreliefup28 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup28.setBounds(new Rectangle(274,131,51,18));
    localPanel.add(pwr_valueinputreliefup28, new Proportion(pwr_valueinputreliefup28.getBounds(), dsize));
    pwr_valueinputreliefup29 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup29.setBounds(new Rectangle(274,358,51,18));
    localPanel.add(pwr_valueinputreliefup29, new Proportion(pwr_valueinputreliefup29.getBounds(), dsize));
    pwr_pulldownmenu230 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu230.setBounds(new Rectangle(268,0,69,24));
    pwr_pulldownmenu230.setShadow(1);
    localPanel.add(pwr_pulldownmenu230, new Proportion(pwr_pulldownmenu230.getBounds(), dsize));
    pwr_pulldownmenu231 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu231.setBounds(new Rectangle(70,0,69,24));
    pwr_pulldownmenu231.setShadow(1);
    localPanel.add(pwr_pulldownmenu231, new Proportion(pwr_pulldownmenu231.getBounds(), dsize));
    pwr_indsquare34 = new pwr_indsquare(session);
    pwr_indsquare34.setBounds(new Rectangle(166,185,15,17));
    pwr_indsquare34.setFillColor(294);
    pwr_indsquare34.setShadow(1);
    localPanel.add(pwr_indsquare34, new Proportion(pwr_indsquare34.getBounds(), dsize));
    jopBar40 = new JopBar(session);
    jopBar40.setBounds(new Rectangle(356,132,24,242));
    jopBar40.setFillColor(39);
    jopBar40.setBorderColor(0);
    jopBar40.setFillColorBar(167);
    jopBar40.setBorderColorBar(161);
    jopBar40.setDrawFill(1);
    jopBar40.setDrawBorder(1);
    jopBar40.setBarBorderWidth(1);
    jopBar40.setLineWidth(1);
    jopBar40.setMinValue(0F);
    jopBar40.setMaxValue(100F);
    jopBar40.setRotate(0);
    localPanel.add(jopBar40, new Proportion(jopBar40.getBounds(), dsize));
    jopBar41 = new JopBar(session);
    jopBar41.setBounds(new Rectangle(410,132,24,242));
    jopBar41.setFillColor(58);
    jopBar41.setBorderColor(0);
    jopBar41.setFillColorBar(105);
    jopBar41.setBorderColorBar(101);
    jopBar41.setDrawFill(1);
    jopBar41.setDrawBorder(1);
    jopBar41.setBarBorderWidth(1);
    jopBar41.setLineWidth(1);
    jopBar41.setMinValue(0F);
    jopBar41.setMaxValue(100F);
    jopBar41.setRotate(0);
    localPanel.add(jopBar41, new Proportion(jopBar41.getBounds(), dsize));
    jopAxis42 = new JopAxis();
    jopAxis42.setBounds(new Rectangle(327,134,11,238));
    jopAxis42.setBorderColor(0);
    jopAxis42.setTextColor(0);
    jopAxis42.setLineWidth(1);
    jopAxis42.setMinValue(0F);
    jopAxis42.setMaxValue(0F);
    jopAxis42.setLines(51);
    jopAxis42.setLongQuotient(5);
    jopAxis42.setValueQuotient(5);
    jopAxis42.setLineLength(11);
    jopAxis42.setLineWidth(1);
    jopAxis42.setRotate(0);
    jopAxis42.setFont(new Font("Helvetica", Font.PLAIN, 10));
    jopAxis42.setFormat("%3.0f");
    localPanel.add(jopAxis42, new Proportion(jopAxis42.getBounds(), dsize));
    pwr_valuesmall43 = new pwr_valuesmall(session);
    pwr_valuesmall43.setBounds(new Rectangle(397,391,50,17));
    localPanel.add(pwr_valuesmall43, new Proportion(pwr_valuesmall43.getBounds(), dsize));
    pwr_smallbuttoncenter44 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter44.setBounds(new Rectangle(36,365,48,21));
    pwr_smallbuttoncenter44.setFillColor(32);
    localPanel.add(pwr_smallbuttoncenter44, new Proportion(pwr_smallbuttoncenter44.getBounds(), dsize));
    pwr_indround45 = new pwr_indround(session);
    pwr_indround45.setBounds(new Rectangle(96,344,18,18));
    pwr_indround45.setShadow(1);
    localPanel.add(pwr_indround45, new Proportion(pwr_indround45.getBounds(), dsize));
    pwr_indround46 = new pwr_indround(session);
    pwr_indround46.setBounds(new Rectangle(96,366,18,18));
    pwr_indround46.setShadow(1);
    localPanel.add(pwr_indround46, new Proportion(pwr_indround46.getBounds(), dsize));
    pwr_sliderbackground1_248 = new pwr_sliderbackground1_2(session);
    pwr_sliderbackground1_248.setBounds(new Rectangle(474,119,34,269));
    pwr_sliderbackground1_248.setShadow(1);
    localPanel.add(pwr_sliderbackground1_248, new Proportion(pwr_sliderbackground1_248.getBounds(), dsize));
    pwr_slider149 = new pwr_slider1(session);
    pwr_slider149.setBounds(new Rectangle(478,301,34,32));
    localPanel.add(pwr_slider149, new Proportion(pwr_slider149.getBounds(), dsize));
    pwr_valueinputsmallrd50 = new pwr_valueinputsmallrd(session);
    pwr_valueinputsmallrd50.setBounds(new Rectangle(464,388,56,24));
    pwr_valueinputsmallrd50.setShadow(1);
    localPanel.add(pwr_valueinputsmallrd50, new Proportion(pwr_valueinputsmallrd50.getBounds(), dsize));
    pwr_indsquare53 = new pwr_indsquare(session);
    pwr_indsquare53.setBounds(new Rectangle(166,224,15,17));
    pwr_indsquare53.setFillColor(176);
    pwr_indsquare53.setShadow(1);
    localPanel.add(pwr_indsquare53, new Proportion(pwr_indsquare53.getBounds(), dsize));
    pwr_indsquare54 = new pwr_indsquare(session);
    pwr_indsquare54.setBounds(new Rectangle(166,243,16,17));
    pwr_indsquare54.setFillColor(176);
    pwr_indsquare54.setShadow(1);
    localPanel.add(pwr_indsquare54, new Proportion(pwr_indsquare54.getBounds(), dsize));
    grp167_55 = new Grp167_(session);
    grp167_55.setBounds(new Rectangle(472,106,44,18));
    localPanel.add(grp167_55, new Proportion(grp167_55.getBounds(), dsize));
    pwr_mbsimulate56 = new pwr_mbsimulate(session);
    pwr_mbsimulate56.setBounds(new Rectangle(508,24,20,20));
    localPanel.add(pwr_mbsimulate56, new Proportion(pwr_mbsimulate56.getBounds(), dsize));
    pwr_actuator57 = new pwr_actuator(session);
    pwr_actuator57.setBounds(new Rectangle(260,453,132,84));
    pwr_actuator57.setShadow(1);
    localPanel.add(pwr_actuator57, new Proportion(pwr_actuator57.getBounds(), dsize));
    pwr_pulldownmenu260 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu260.setBounds(new Rectangle(161,0,86,24));
    pwr_pulldownmenu260.setShadow(1);
    localPanel.add(pwr_pulldownmenu260, new Proportion(pwr_pulldownmenu260.getBounds(), dsize));
    pwr_smallbuttoncenter61 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter61.setBounds(new Rectangle(36,342,48,21));
    pwr_smallbuttoncenter61.setFillColor(32);
    localPanel.add(pwr_smallbuttoncenter61, new Proportion(pwr_smallbuttoncenter61.getBounds(), dsize));
    pwr_indround62 = new pwr_indround(session);
    pwr_indround62.setBounds(new Rectangle(96,397,18,18));
    pwr_indround62.setFillColor(114);
    pwr_indround62.setShadow(1);
    localPanel.add(pwr_indround62, new Proportion(pwr_indround62.getBounds(), dsize));
    grp175_63 = new Grp175_(session);
    grp175_63.setBounds(new Rectangle(39,396,37,17));
    localPanel.add(grp175_63, new Proportion(grp175_63.getBounds(), dsize));
    pwr_indsquare64 = new pwr_indsquare(session);
    pwr_indsquare64.setBounds(new Rectangle(183,487,16,16));
    pwr_indsquare64.setFillColor(114);
    pwr_indsquare64.setShadow(1);
    localPanel.add(pwr_indsquare64, new Proportion(pwr_indsquare64.getBounds(), dsize));
    pwr_valve3waycontrol267 = new pwr_valve3waycontrol2(session);
    pwr_valve3waycontrol267.setBounds(new Rectangle(111,99,66,53));
    pwr_valve3waycontrol267.setShadow(1);
    localPanel.add(pwr_valve3waycontrol267, new Proportion(pwr_valve3waycontrol267.getBounds(), dsize));
    pwr_base3wayvalve69 = new pwr_base3wayvalve(session);
    pwr_base3wayvalve69.setBounds(new Rectangle(276,532,100,132));
    localPanel.add(pwr_base3wayvalve69, new Proportion(pwr_base3wayvalve69.getBounds(), dsize));
    pwr_base3wayvalve270 = new pwr_base3wayvalve2(session);
    pwr_base3wayvalve270.setBounds(new Rectangle(429,532,99,131));
    localPanel.add(pwr_base3wayvalve270, new Proportion(pwr_base3wayvalve270.getBounds(), dsize));
    pwr_mbup71 = new pwr_mbup(session);
    pwr_mbup71.setBounds(new Rectangle(488,24,20,20));
    localPanel.add(pwr_mbup71, new Proportion(pwr_mbup71.getBounds(), dsize));
    pwr_indsquare73 = new pwr_indsquare(session);
    pwr_indsquare73.setBounds(new Rectangle(166,270,16,17));
    pwr_indsquare73.setFillColor(295);
    pwr_indsquare73.setShadow(1);
    localPanel.add(pwr_indsquare73, new Proportion(pwr_indsquare73.getBounds(), dsize));
    pwr_valuelong5.dd.setDynType(1025);
    pwr_valuelong5.dd.setActionType(0);
    pwr_valuelong5.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong5.dd, "$object.Description##String80","%s")
      });
    pwr_menubar26.dd.setDynType(1);
    pwr_menubar26.dd.setActionType(0);
    pwr_pulldownmenu27.dd.setDynType(0);
    pwr_pulldownmenu27.dd.setActionType(524288);
    pwr_pulldownmenu27.dd.setAccess(65535);
    pwr_pulldownmenu27.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu27.dd, new String[] {
        "Print","Close",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu27,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu27.dd, "print graph/class/inst=$object")
      }),
    new GeDyn(pwr_pulldownmenu27,1,262145,65535,new GeDynElemIfc[] {
      new GeDynCloseGraph(pwr_pulldownmenu27.dd)
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu27.setAnnot1("File");
    pwr_pulldownmenu28.dd.setDynType(1);
    pwr_pulldownmenu28.dd.setActionType(524288);
    pwr_pulldownmenu28.dd.setAccess(65532);
    pwr_pulldownmenu28.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu28.dd, new String[] {
        "Help","Help Class",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu28,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu28.dd, "call method/method=\"Help\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu28,1,65,65532,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu28.dd, "call method/method=\"Help Class\"/object=$object")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu28.setAnnot1("Help");
    pwr_mbopenobject9.dd.setDynType(129);
    pwr_mbopenobject9.dd.setActionType(8256);
    pwr_mbopenobject9.dd.setAccess(65532);
    pwr_mbopenobject9.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbopenobject9.dd, "$cmd(check method/method=\"Open Object\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbopenobject9.dd, "call method/method=\"Open Object\"/object=$object")
      ,new GeDynTipText(pwr_mbopenobject9.dd, "Open Object")
      });
    pwr_mbtrend10.dd.setDynType(129);
    pwr_mbtrend10.dd.setActionType(8256);
    pwr_mbtrend10.dd.setAccess(65532);
    pwr_mbtrend10.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbtrend10.dd, "$cmd(check method/method=\"Trend\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbtrend10.dd, "call method/method=\"Trend\"/object=$object")
      ,new GeDynTipText(pwr_mbtrend10.dd, "Trend")
      });
    pwr_mbfast11.dd.setDynType(129);
    pwr_mbfast11.dd.setActionType(8256);
    pwr_mbfast11.dd.setAccess(65532);
    pwr_mbfast11.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbfast11.dd, "$cmd(check method/method=\"Fast\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbfast11.dd, "call method/method=\"Fast\"/object=$object")
      ,new GeDynTipText(pwr_mbfast11.dd, "Fast")
      });
    pwr_mbphoto12.dd.setDynType(129);
    pwr_mbphoto12.dd.setActionType(8256);
    pwr_mbphoto12.dd.setAccess(65532);
    pwr_mbphoto12.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbphoto12.dd, "$cmd(check method/method=\"Photo\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbphoto12.dd, "call method/method=\"Photo\"/object=$object")
      ,new GeDynTipText(pwr_mbphoto12.dd, "Photo")
      });
    pwr_mbdatasheet13.dd.setDynType(129);
    pwr_mbdatasheet13.dd.setActionType(8256);
    pwr_mbdatasheet13.dd.setAccess(65532);
    pwr_mbdatasheet13.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbdatasheet13.dd, "$cmd(check method/method=\"DataSheet\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbdatasheet13.dd, "call method/method=\"DataSheet\"/object=$object")
      ,new GeDynTipText(pwr_mbdatasheet13.dd, "DataSheet")
      });
    pwr_mbopenplc14.dd.setDynType(129);
    pwr_mbopenplc14.dd.setActionType(8256);
    pwr_mbopenplc14.dd.setAccess(65532);
    pwr_mbopenplc14.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbopenplc14.dd, "$cmd(check method/method=\"Open Plc\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbopenplc14.dd, "call method/method=\"Open Plc\"/object=$object")
      ,new GeDynTipText(pwr_mbopenplc14.dd, "Open Plc")
      });
    pwr_mbcircuitdiagram15.dd.setDynType(129);
    pwr_mbcircuitdiagram15.dd.setActionType(8256);
    pwr_mbcircuitdiagram15.dd.setAccess(65535);
    pwr_mbcircuitdiagram15.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbcircuitdiagram15.dd, "$cmd(check method/method=\"CircuitDiagram\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbcircuitdiagram15.dd, "call method/method=\"CircuitDiagram\"/object=$object")
      ,new GeDynTipText(pwr_mbcircuitdiagram15.dd, "CircuitDiagram")
      });
    pwr_mbrtnavigator16.dd.setDynType(129);
    pwr_mbrtnavigator16.dd.setActionType(8256);
    pwr_mbrtnavigator16.dd.setAccess(65532);
    pwr_mbrtnavigator16.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbrtnavigator16.dd, "$cmd(check method/method=\"RtNavigator\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbrtnavigator16.dd, "call method/method=\"RtNavigator\"/object=$object")
      ,new GeDynTipText(pwr_mbrtnavigator16.dd, "RtNavigator")
      });
    pwr_mbhelpclass17.dd.setDynType(129);
    pwr_mbhelpclass17.dd.setActionType(8256);
    pwr_mbhelpclass17.dd.setAccess(65532);
    pwr_mbhelpclass17.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbhelpclass17.dd, "$cmd(check method/method=\"Help Class\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbhelpclass17.dd, "call method/method=\"Help Class\"/object=$object")
      ,new GeDynTipText(pwr_mbhelpclass17.dd, "Help Class")
      });
    pwr_mbblockevents18.dd.setDynType(129);
    pwr_mbblockevents18.dd.setActionType(8256);
    pwr_mbblockevents18.dd.setAccess(65532);
    pwr_mbblockevents18.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbblockevents18.dd, "$cmd(check method/method=\"Block Events...\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbblockevents18.dd, "call method/method=\"Block Events...\"/object=$object")
      ,new GeDynTipText(pwr_mbblockevents18.dd, "Block Events")
      });
    pwr_mbhistevent19.dd.setDynType(129);
    pwr_mbhistevent19.dd.setActionType(8256);
    pwr_mbhistevent19.dd.setAccess(65532);
    pwr_mbhistevent19.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbhistevent19.dd, "$cmd(check method/method=\"Hist Event...\"/object=$object)",0)
      ,new GeDynCommand(pwr_mbhistevent19.dd, "call method/method=\"Hist Event...\"/object=$object")
      ,new GeDynTipText(pwr_mbhistevent19.dd, "Hist Event")
      });
    pwr_mbnote20.dd.setDynType(129);
    pwr_mbnote20.dd.setActionType(8256);
    pwr_mbnote20.dd.setAccess(65532);
    pwr_mbnote20.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbnote20.dd, "$cmd(check method/method=\"Note\"/object=$object)",0)
      ,new GeDynCommand(pwr_mbnote20.dd, "call method/method=\"Note\"/object=$object")
      ,new GeDynTipText(pwr_mbnote20.dd, "Note")
      });
    pwr_mbhelp21.dd.setDynType(128);
    pwr_mbhelp21.dd.setActionType(8256);
    pwr_mbhelp21.dd.setAccess(65532);
    pwr_mbhelp21.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbhelp21.dd, "$cmd(check method/method=\"Help\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbhelp21.dd, "call method/method=\"Help\"/object=$object")
      ,new GeDynTipText(pwr_mbhelp21.dd, "Help")
      });
    pwr_smallbuttoncenter22.dd.setDynType(128);
    pwr_smallbuttoncenter22.dd.setActionType(64);
    pwr_smallbuttoncenter22.dd.setAccess(65532);
    pwr_smallbuttoncenter22.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_smallbuttoncenter22.dd, "$object.Note##String80",0)
      ,new GeDynCommand(pwr_smallbuttoncenter22.dd, "call method/method=\"Note\"/object=$object")
      });
    pwr_smallbuttoncenter22.setAnnot1("Note !");
    pwr_valuelong23.dd.setDynType(1025);
    pwr_valuelong23.dd.setActionType(0);
    pwr_valuelong23.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong23.dd, "$object.Specification##String40","%s")
      });
    pwr_valuelong24.dd.setDynType(1025);
    pwr_valuelong24.dd.setActionType(0);
    pwr_valuelong24.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong24.dd, "$object.Note##String80","%s")
      });
    pwr_valuesmall26.dd.setDynType(1025);
    pwr_valuesmall26.dd.setActionType(2);
    pwr_valuesmall26.dd.setAccess(65532);
    pwr_valuesmall26.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall26.dd, "$object.Actuator.Position.ActualValue##Float32","%5.1f")
      ,new GeDynPopupMenu(pwr_valuesmall26.dd, "$object.Actuator.Position")
      });
    pwr_sliderbackground327.dd.setDynType(524289);
    pwr_sliderbackground327.dd.setActionType(0);
    pwr_valueinputreliefup28.dd.setDynType(1025);
    pwr_valueinputreliefup28.dd.setActionType(4096);
    pwr_valueinputreliefup28.dd.setAccess(65532);
    pwr_valueinputreliefup28.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup28.dd, "$object.Actuator.Order.PresMaxLimit##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup28.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup29.dd.setDynType(1025);
    pwr_valueinputreliefup29.dd.setActionType(4096);
    pwr_valueinputreliefup29.dd.setAccess(65532);
    pwr_valueinputreliefup29.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup29.dd, "$object.Actuator.Order.PresMinLimit##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup29.dd, 0,0,null,null)
      });
    pwr_pulldownmenu230.dd.setDynType(1);
    pwr_pulldownmenu230.dd.setActionType(524288);
    pwr_pulldownmenu230.dd.setAccess(65535);
    pwr_pulldownmenu230.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu230.dd, new String[] {
        "Position Ai","Order Ao","SwitchOpen Di",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu230,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu230.dd, "open graph /class /inst=$object.Actuator.Position")
      }),
    new GeDyn(pwr_pulldownmenu230,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu230.dd, "open graph /class /inst=$object.Actuator.Order")
      }),
    new GeDyn(pwr_pulldownmenu230,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu230.dd, "open graph /class /inst=$object.Actuator.SwitchOpen")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu230.setAnnot1("Signals");
    pwr_pulldownmenu231.dd.setDynType(1);
    pwr_pulldownmenu231.dd.setActionType(524288);
    pwr_pulldownmenu231.dd.setAccess(65535);
    pwr_pulldownmenu231.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu231.dd, new String[] {
        "Help","Note","Trend","Fast","Photo","DataSheet","Hist Event...","Block Events...","RtNavigator","Open  Object","Open Plc","CircuitDiagram","Help Class",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu231,129,65,65535,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Help\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Help\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Note\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Note\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Trend\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Trend\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Fast\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Fast\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Photo\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Photo\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"DataSheet\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"DataSheet\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Hist Event...\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Hist Event...\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Block Events...\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Block Events...\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"RtNavigator\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"RtNavigator\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Open Object\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Open Object\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Open Plc\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Open Plc\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"CircuitDiagram\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"CircuitDiagram\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu231,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu231.dd, "$cmd(check method/method=\"Help Class\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu231.dd, "call method/method=\"Help Class\"/object=$object")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu231.setAnnot1("Methods");
    pwr_indsquare34.dd.setDynType(5);
    pwr_indsquare34.dd.setActionType(2);
    pwr_indsquare34.dd.setAccess(65532);
    pwr_indsquare34.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare34.dd, "$object.Actuator.SwitchOpen.ActualValue##Boolean",29)
      ,new GeDynPopupMenu(pwr_indsquare34.dd, "$object.Actuator.SwitchOpen")
      });
    jopBar40.setPwrAttribute("$object.Actuator.Position.ActualValue##Float32");
    jopBar40.setMinValueAttr("$object.Actuator.Order.PresMinLimit##Float32");
    jopBar40.setMaxValueAttr("$object.Actuator.Order.PresMaxLimit##Float32");
    jopBar41.setPwrAttribute("$object.Actuator.Order.ActualValue##Float32");
    jopBar41.setMinValueAttr("$object.Actuator.Order.PresMinLimit##Float32");
    jopBar41.setMaxValueAttr("$object.Actuator.Order.PresMaxLimit##Float32");
    pwr_valuesmall43.dd.setDynType(1025);
    pwr_valuesmall43.dd.setActionType(2);
    pwr_valuesmall43.dd.setAccess(65532);
    pwr_valuesmall43.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall43.dd, "$object.Actuator.Order.ActualValue##Float32","%5.1f")
      ,new GeDynPopupMenu(pwr_valuesmall43.dd, "$object.Actuator.Order")
      });
    pwr_smallbuttoncenter44.dd.setDynType(129);
    pwr_smallbuttoncenter44.dd.setActionType(4);
    pwr_smallbuttoncenter44.dd.setAccess(65532);
    pwr_smallbuttoncenter44.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_smallbuttoncenter44.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      ,new GeDynSetDig(pwr_smallbuttoncenter44.dd, "$object.Actuator.Mode.OpAuto##Boolean")
      });
    pwr_smallbuttoncenter44.setAnnot1("Auto");
    pwr_indround45.dd.setDynType(133);
    pwr_indround45.dd.setActionType(0);
    pwr_indround45.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_indround45.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      ,new GeDynDigLowColor(pwr_indround45.dd, "$object.Actuator.Mode.ManMode##Boolean",29)
      });
    pwr_indround46.dd.setDynType(133);
    pwr_indround46.dd.setActionType(0);
    pwr_indround46.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_indround46.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      ,new GeDynDigLowColor(pwr_indround46.dd, "!$object.Actuator.Mode.ManMode##Boolean",29)
      });
    pwr_sliderbackground1_248.dd.setDynType(524417);
    pwr_sliderbackground1_248.dd.setActionType(0);
    pwr_sliderbackground1_248.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_sliderbackground1_248.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      });
    pwr_slider149.dd.setDynType(129);
    pwr_slider149.dd.setActionType(2048);
    pwr_slider149.dd.setAccess(65532);
    pwr_slider149.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_slider149.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      ,new GeDynSlider(pwr_slider149.dd, "$object.Actuator.Mode.OpRefMan##Float32",0,100,4,122.911,361.449,"$object.Actuator.Order.PresMinLimit##Float32","$object.Actuator.Order.PresMaxLimit##Float32",
null)
      });
    pwr_valueinputsmallrd50.dd.setDynType(1153);
    pwr_valueinputsmallrd50.dd.setActionType(4096);
    pwr_valueinputsmallrd50.dd.setAccess(65532);
    pwr_valueinputsmallrd50.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_valueinputsmallrd50.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      ,new GeDynValue(pwr_valueinputsmallrd50.dd, "$object.Actuator.Mode.OpRefMan##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputsmallrd50.dd, 0,0,"$object.Actuator.Mode.OpMinRefMan##Float32","$object.Actuator.Mode.OpMaxRefMan##Float32")
      });
    pwr_indsquare53.dd.setDynType(5);
    pwr_indsquare53.dd.setActionType(0);
    pwr_indsquare53.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare53.dd, "$object.Actuator.DiffPosError##Boolean",29)
      });
    pwr_indsquare54.dd.setDynType(5);
    pwr_indsquare54.dd.setActionType(0);
    pwr_indsquare54.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare54.dd, "$object.Actuator.SwitchError##Boolean",29)
      });
    grp167_55.dd.setDynType(129);
    grp167_55.dd.setActionType(0);
    grp167_55.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(grp167_55.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      });
    pwr_mbsimulate56.dd.setDynType(129);
    pwr_mbsimulate56.dd.setActionType(8256);
    pwr_mbsimulate56.dd.setAccess(65532);
    pwr_mbsimulate56.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbsimulate56.dd, "$cmd(check method/method=\"Simulate\"/object=$object)",0)
      ,new GeDynCommand(pwr_mbsimulate56.dd, "call method/method=\"Simulate\"/object=$object")
      ,new GeDynTipText(pwr_mbsimulate56.dd, "Open simulate graph")
      });
    pwr_actuator57.dd.setDynType(1);
    pwr_actuator57.dd.setActionType(66);
    pwr_actuator57.dd.setAccess(65532);
    pwr_actuator57.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(pwr_actuator57.dd, "$object.Actuator")
      ,new GeDynCommand(pwr_actuator57.dd, "open graph/class/inst=$object.Actuator")
      });
    pwr_pulldownmenu260.dd.setDynType(1);
    pwr_pulldownmenu260.dd.setActionType(524288);
    pwr_pulldownmenu260.dd.setAccess(65535);
    pwr_pulldownmenu260.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu260.dd, new String[] {
        "Actuator","Valve",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu260,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu260.dd, "open graph /class /inst=$object.Actuator")
      }),
    new GeDyn(pwr_pulldownmenu260,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu260.dd, "open graph /class /inst=$object.Valve")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu260.setAnnot1("Components");
    pwr_smallbuttoncenter61.dd.setDynType(128);
    pwr_smallbuttoncenter61.dd.setActionType(4);
    pwr_smallbuttoncenter61.dd.setAccess(65532);
    pwr_smallbuttoncenter61.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_smallbuttoncenter61.dd, "$object.Actuator.Mode.OpAutoOnly##Boolean",0)
      ,new GeDynSetDig(pwr_smallbuttoncenter61.dd, "$object.Actuator.Mode.OpMan##Boolean")
      });
    pwr_smallbuttoncenter61.setAnnot1("Man");
    pwr_indround62.dd.setDynType(133);
    pwr_indround62.dd.setActionType(0);
    pwr_indround62.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_indround62.dd, "$object.Actuator.Mode.OpNoLocal##Boolean",0)
      ,new GeDynDigLowColor(pwr_indround62.dd, "$object.Actuator.Mode.LocalMode##Boolean",29)
      });
    grp175_63.dd.setDynType(129);
    grp175_63.dd.setActionType(0);
    grp175_63.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(grp175_63.dd, "$object.Actuator.Mode.OpNoLocal##Boolean",0)
      });
    pwr_indsquare64.dd.setDynType(13);
    pwr_indsquare64.dd.setActionType(0);
    pwr_indsquare64.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigColor(pwr_indsquare64.dd, "$object.Actuator.IndError##Boolean",176)
      ,new GeDynDigLowColor(pwr_indsquare64.dd, "$object.Actuator.IndWarning##Boolean",29)
      });
    pwr_valve3waycontrol267.dd.setDynType(17456);
    pwr_valve3waycontrol267.dd.setActionType(0);
    pwr_valve3waycontrol267.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigError(pwr_valve3waycontrol267.dd, "$object.Actuator.IndError##Boolean")
      ,new GeDynDigWarning(pwr_valve3waycontrol267.dd, "$object.Actuator.IndWarning##Boolean")
      ,new GeDynValue(pwr_valve3waycontrol267.dd, "$object.Actuator.Mode.IndMode##String8","%s")
      ,new GeDynAnalogShift(pwr_valve3waycontrol267.dd, "$object.Actuator.PosEnum##Int32")
      });
    pwr_base3wayvalve69.dd.setDynType(0);
    pwr_base3wayvalve69.dd.setActionType(66);
    pwr_base3wayvalve69.dd.setAccess(65532);
    pwr_base3wayvalve69.dd.setElements(new GeDynElemIfc[] {
      new GeDynPopupMenu(pwr_base3wayvalve69.dd, "$object.Valve")
      ,new GeDynCommand(pwr_base3wayvalve69.dd, "open graph/class/inst=$object.Valve")
      });
    pwr_base3wayvalve270.dd.setDynType(16384);
    pwr_base3wayvalve270.dd.setActionType(66);
    pwr_base3wayvalve270.dd.setAccess(65532);
    pwr_base3wayvalve270.dd.setElements(new GeDynElemIfc[] {
      new GeDynAnalogShift(pwr_base3wayvalve270.dd, "$object.Actuator.PosEnum##Int32")
      ,new GeDynPopupMenu(pwr_base3wayvalve270.dd, "$object.Valve")
      ,new GeDynCommand(pwr_base3wayvalve270.dd, "open graph/class/inst=$object.Valve")
      });
    pwr_mbup71.dd.setDynType(129);
    pwr_mbup71.dd.setActionType(8256);
    pwr_mbup71.dd.setAccess(33619967);
    pwr_mbup71.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbup71.dd, "$cmd(check isattribute/object=$object)",0)
      ,new GeDynCommand(pwr_mbup71.dd, "open graph/class/parent/instance=$object")
      ,new GeDynTipText(pwr_mbup71.dd, "Open parent object graph")
      });
    pwr_indsquare73.dd.setDynType(5);
    pwr_indsquare73.dd.setActionType(0);
    pwr_indsquare73.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare73.dd, "$object.Actuator.SupDisabled##Boolean",29)
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
  public int original_width = 530;
  public int original_height = 689;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 23.1001F, 526.654F, 22.4305F),
    new Line2D.Float( 252.569F, 329.042F, 18.0339F, 329.042F),
    new Rectangle2D.Float(17.625F, 304.139F, 237.944F, 121.555F),
    new Polygon( new int[] { 18, 256, 255, 18, 18, 18}, new int[] { 304, 304, 305, 305, 425, 426}, 6), 
    new Polygon( new int[] { 256, 256, 255, 255, 18, 18}, new int[] { 426, 304, 305, 425, 425, 426}, 6), 
    new Rectangle2D.Float(259.625F, 98.9998F, 266.625F, 327.444F),
    new Polygon( new int[] { 260, 526, 525, 261, 261, 260}, new int[] { 99, 99, 100, 100, 425, 426}, 6), 
    new Polygon( new int[] { 526, 526, 525, 525, 261, 260}, new int[] { 426, 99, 100, 425, 425, 426}, 6), 
    new Rectangle2D.Float(17.625F, 98.9982F, 237.944F, 197.211F),
    new Polygon( new int[] { 18, 256, 255, 19, 19, 18}, new int[] { 99, 99, 100, 100, 295, 296}, 6), 
    new Polygon( new int[] { 256, 256, 255, 255, 19, 18}, new int[] { 296, 99, 100, 295, 295, 296}, 6), 
    new Line2D.Float( 454.656F, 181F, 338.496F, 181F),
    new Line2D.Float( 454.656F, 229F, 338.496F, 229F),
    new Line2D.Float( 454.656F, 276F, 338.496F, 276F),
    new Line2D.Float( 454.656F, 325F, 338.496F, 325F),
    new Rectangle2D.Float(66.0694F, 128.194F, 48.611F, 5.5556F),
    new Polygon( new int[] { 66, 115, 113, 68, 68, 66}, new int[] { 128, 128, 130, 130, 132, 134}, 6), 
    new Polygon( new int[] { 115, 115, 113, 113, 68, 66}, new int[] { 134, 128, 130, 132, 132, 134}, 6), 
    new Rectangle2D.Float(152.181F, 128.194F, 48.611F, 5.5556F),
    new Polygon( new int[] { 152, 201, 199, 154, 154, 152}, new int[] { 128, 128, 130, 130, 132, 134}, 6), 
    new Polygon( new int[] { 201, 201, 199, 199, 154, 152}, new int[] { 134, 128, 130, 132, 132, 134}, 6), 
    new Rectangle2D.Float(190.625F, 169F, 10F, 7F),
    new Line2D.Float( 190.625F, 174F, 199.625F, 174F),
    new Line2D.Float( 199.625F, 170F, 199.625F, 174F),
    new Line2D.Float( 190.625F, 170F, 199.625F, 170F),
    new Line2D.Float( 190.625F, 169F, 200.625F, 169F),
    new Line2D.Float( 190.625F, 175F, 200.625F, 175F),
    new Line2D.Float( 200.625F, 169F, 200.625F, 175F),
    new Line2D.Float( 133.625F, 152F, 133.625F, 162F),
    new Line2D.Float( 143.625F, 172F, 190.625F, 172F),
    new Arc2D.Float(133.625F, 152F, 20F, 20F, 180F, 90F, Arc2D.OPEN),
    new Line2D.Float( 131.625F, 152F, 131.625F, 162F),
    new Line2D.Float( 135.625F, 152F, 135.625F, 162F),
    new Line2D.Float( 130.625F, 152F, 130.625F, 162F),
    new Line2D.Float( 136.625F, 152F, 136.625F, 162F),
    new Line2D.Float( 143.625F, 170F, 190.625F, 170F),
    new Line2D.Float( 143.625F, 174F, 190.625F, 174F),
    new Line2D.Float( 143.625F, 169F, 190.625F, 169F),
    new Line2D.Float( 143.625F, 175F, 190.625F, 175F),
    new Arc2D.Float(131.625F, 150F, 24F, 24F, 225F, 45F, Arc2D.OPEN),
    new Arc2D.Float(131.625F, 150F, 24F, 24F, 180F, 45F, Arc2D.OPEN),
    new Arc2D.Float(135.625F, 154F, 16F, 16F, 225F, 45F, Arc2D.OPEN),
    new Arc2D.Float(135.625F, 154F, 16F, 16F, 180F, 45F, Arc2D.OPEN),
    new Arc2D.Float(130.625F, 149F, 26F, 26F, 180F, 90F, Arc2D.OPEN),
    new Arc2D.Float(136.625F, 155F, 14F, 14F, 180F, 90F, Arc2D.OPEN),
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
    {
      GradientPaint gp = new GradientPaint( 2F,45.5306F,        GeColor.getColor(31,colorTone,colorShift,colorIntensity,0, colorInverse, fillColor, dimmed),
        2F,23.1001F,GeColor.getColor(31,colorTone,colorShift,colorIntensity,-1, colorInverse, fillColor, dimmed),true);
      g.setPaint(gp);
    }
    g.fill( shapes[0]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[1]);
    {
    int fcolor = GeColor.getDrawtype(31, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[3]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    }
    {
    int fcolor = GeColor.getDrawtype(31, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[7]);
    }
    {
    int fcolor = GeColor.getDrawtype(31, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[9]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[10]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( "Position",345, 119);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( "Order",406, 119);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( "SwitchOpen",38, 199);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( "%",300, 165);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[11]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[12]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[13]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[14]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( "Mode",38, 324);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( "Position error",38, 238);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( "Switch error",38, 258);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Actuator",98, 500);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Valve",98, 601);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[16]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[17]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[15]);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[18]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[19]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[20]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[18]);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[21]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(76, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[22]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(76, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[23]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(72, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[24]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[25]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[26]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[27]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 10));
    g.drawString( "Supervision disabled",37, 282);
    g.setStroke( new BasicStroke(7F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[28]);
    g.setStroke( new BasicStroke(7F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[29]);
    g.setStroke( new BasicStroke(7F));
    g.setColor(GeColor.getColor(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[30]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(72, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[31]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(76, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[32]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[33]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[34]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(72, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[35]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(76, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[36]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[37]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[38]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(76, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[39]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(72, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[40]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(72, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[41]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(76, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[42]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[43]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[44]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_smallbuttoncenter extends GeComponent {
  // Dimension size;
  public pwr_smallbuttoncenter( JopSession session)
  {
    super( session);
    size = new Dimension( 47, 21);
    annot1Font = new Font("Helvetica", Font.BOLD, 12);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 47;
  public int original_height = 21;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 44F, 17F),
    new Polygon( new int[] { 2, 46, 43, 5, 5, 2}, new int[] { 2, 2, 5, 5, 16, 19}, 6), 
    new Polygon( new int[] { 46, 46, 43, 43, 5, 2}, new int[] { 19, 2, 5, 16, 16, 19}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,1,-1,2F,2F,44F,17F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    FontRenderContext frc = g.getFontRenderContext();
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 24 * original_height / height * width / original_width- (float)g.getFont().getStringBounds(annot1, frc).getWidth()/2, 15F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_indsquare extends GeComponent {
  // Dimension size;
  public pwr_indsquare( JopSession session)
  {
    super( session);
    size = new Dimension( 16, 17);
  }
  public int original_width = 16;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 12F, 13F),
    new Polygon( new int[] { 2, 14, 12, 4, 4, 2}, new int[] { 2, 2, 4, 4, 13, 15}, 6), 
    new Polygon( new int[] { 14, 14, 12, 12, 4, 2}, new int[] { 15, 2, 4, 13, 13, 15}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,12F,13F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
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
protected class pwr_base3wayvalve extends GeComponent {
  // Dimension size;
  public pwr_base3wayvalve( JopSession session)
  {
    super( session);
    size = new Dimension( 64, 84);
  }
  public int original_width = 64;
  public int original_height = 84;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(26F, 6F, 12F, 14F),
    new Polygon( new int[] { 26, 30, 30, 26, 26}, new int[] {6, 6, 20, 20, 6}, 5),
    new Polygon( new int[] { 34, 38, 38, 34, 34}, new int[] {6, 6, 20, 20, 6}, 5),
    new Rectangle2D.Float(26F, 6F, 12F, 14F),
    new Rectangle2D.Float(34F, 2F, 8F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 12F, 4.44445F),
    new Rectangle2D.Float(24F, 2F, 4F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 20F, 4.44445F),
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 27}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 27}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 27, 29, 29, 58}, new int[] { 18, 23, 23,-2147483648}, 4),
    new Polygon( new int[] { 6, 16, 16, 6, 6}, new int[] {48, 54, 60, 54, 48}, 5),
    new Polygon( new int[] { 6, 9, 17, 16}, new int[] { 48, 50, 55,54}, 4),
    new Polygon( new int[] { 16, 17, 17, 16}, new int[] { 54, 55, 58,60}, 4),
    new Polygon( new int[] { 16, 17, 9, 6}, new int[] { 60, 58, 53,54}, 4),
    new Polygon( new int[] { 6, 9, 9, 6}, new int[] { 54, 53, 50,48}, 4),
    new Polygon( new int[] { 6, 9, -1220087285, 58}, new int[] { 48, 50, -1220087316,27}, 4),
    new Polygon( new int[] { 6, 14, 16, 16, 14, 6, 6}, new int[] {34, 38, 36, 42, 42, 38, 34}, 7),
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {34, 30, 28, 34, 34, 38, 34}, 7),
    new Polygon( new int[] { 48, 42, 42, 40, 38, 38, 42, 42, 46, 48, 48}, new int[] {22, 22, 66, 66, 68, 78, 78, 68, 68, 66, 22}, 11),
    new Polygon( new int[] { 48, 22, 22, 16, 16, 18, 22, 26, 38, 42, 46, 48}, new int[] {22, 22, 66, 66, 22, 20, 20, 18, 18, 20, 20, 22}, 12),
    new Rectangle2D.Float(22F, 68F, 4F, 10F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 68, 68, 68, 68, 78, 78}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 78, 68, 68, 78, 78, 78}, 6), 
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {54, 50, 52, 44, 44, 48, 54}, 7),
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 26}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 26}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 26, 29, -1220087349, 58}, new int[] { 18, 23, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(2F, 50F, 4F, 8F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(2F, 32F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(2F, 46F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 50F, 4F, 8F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 32F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(58F, 46F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(22F, 78F, 4F, 4F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(40F, 78F, 6F, 4F),
    new Polygon( new int[] { 40, 46, 46, 40, 40, 40}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 40, 40}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(36F, 78F, 4F, 4F),
    new Polygon( new int[] { 36, 40, 40, 36, 36, 36}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 40, 40, 40, 40, 36, 36}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(32, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(32, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[4]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[5]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[6]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[7]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[8]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[8]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[9]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[10]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,18F,78F,28F,4F, false,2, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[13]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[14]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,17.9369F,52F,60.0631F, false,34, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[14]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[16]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[17]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[18]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[19]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[21]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[22]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[23]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[24]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[25]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[26]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[27]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[28]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[29]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[30]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[31]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[32]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[33]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[34]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[35]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[36]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[37]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[38]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[39]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[40]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[41]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[42]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[44]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,48F,10F,12F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[44]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[45]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[46]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[47]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[48]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[50]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[51]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[52]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[53]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[54]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,68F,4F,10F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[54]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[55]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[56]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[57]);
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[59]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[60]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[61]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[62]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[63]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[64]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[65]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[66]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[67]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[68]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[69]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[70]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[71]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[72]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[73]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[74]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[75]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[76]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[77]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[78]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[79]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[80]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[81]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[82]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[83]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[84]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[85]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[86]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[58]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[88]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[88]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[89]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[90]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[91]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[91]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[92]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[93]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[94]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[94]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[95]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[96]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[98]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[99]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[97]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[100]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[100]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[101]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[102]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[103]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[103]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[104]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[105]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[106]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[106]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[107]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[108]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[109]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[109]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[110]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[111]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[113]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[114]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[112]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[115]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,78F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[115]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[116]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[117]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[118]);
    } else {
    GeGradient.paint( g, gradient,2,-2,40F,78F,6F,4F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[118]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[119]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[120]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[121]);
    } else {
    GeGradient.paint( g, gradient,2,-2,36F,78F,4F,4F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[121]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[122]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[123]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[125]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[126]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[124]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_base3wayvalve2 extends GeComponent {
  // Dimension size;
  public pwr_base3wayvalve2( JopSession session)
  {
    super( session);
    size = new Dimension( 64, 84);
    pages = 3;
  }
  public int original_width = 64;
  public int original_height = 84;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(26F, 6F, 12F, 14F),
    new Polygon( new int[] { 26, 30, 30, 26, 26}, new int[] {6, 6, 20, 20, 6}, 5),
    new Polygon( new int[] { 34, 38, 38, 34, 34}, new int[] {6, 6, 20, 20, 6}, 5),
    new Rectangle2D.Float(26F, 6F, 12F, 14F),
    new Rectangle2D.Float(34F, 2F, 8F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 12F, 4.44445F),
    new Rectangle2D.Float(24F, 2F, 4F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 20F, 4.44445F),
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 27}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 27}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 27, 29, 29, 58}, new int[] { 18, 23, 23,-2147483648}, 4),
    new Polygon( new int[] { 6, 16, 16, 6, 6}, new int[] {48, 54, 60, 54, 48}, 5),
    new Polygon( new int[] { 6, 9, 17, 16}, new int[] { 48, 50, 55,54}, 4),
    new Polygon( new int[] { 16, 17, 17, 16}, new int[] { 54, 55, 58,60}, 4),
    new Polygon( new int[] { 16, 17, 9, 6}, new int[] { 60, 58, 53,54}, 4),
    new Polygon( new int[] { 6, 9, 9, 6}, new int[] { 54, 53, 50,48}, 4),
    new Polygon( new int[] { 6, 9, 58, 58}, new int[] { 48, 50, -1070333924,27}, 4),
    new Polygon( new int[] { 6, 14, 16, 16, 14, 6, 6}, new int[] {34, 38, 36, 42, 42, 38, 34}, 7),
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {34, 30, 28, 34, 34, 38, 34}, 7),
    new Polygon( new int[] { 48, 42, 42, 40, 38, 38, 42, 42, 46, 48, 48}, new int[] {22, 22, 66, 66, 68, 78, 78, 68, 68, 66, 22}, 11),
    new Polygon( new int[] { 48, 22, 22, 16, 16, 18, 22, 26, 38, 42, 46, 48}, new int[] {22, 22, 66, 66, 22, 20, 20, 18, 18, 20, 20, 22}, 12),
    new Rectangle2D.Float(22F, 68F, 4F, 10F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 68, 68, 68, 68, 78, 78}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 78, 68, 68, 78, 78, 78}, 6), 
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {54, 50, 52, 44, 44, 48, 54}, 7),
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 26}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 26}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 26, 29, -1220087349, 58}, new int[] { 18, 23, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(2F, 50F, 4F, 8F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(2F, 32F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(2F, 46F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 50F, 4F, 8F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 32F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(58F, 46F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(22F, 78F, 4F, 4F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(40F, 78F, 6F, 4F),
    new Polygon( new int[] { 40, 46, 46, 40, 40, 40}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 40, 40}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(36F, 78F, 4F, 4F),
    new Polygon( new int[] { 36, 40, 40, 36, 36, 36}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 40, 40, 40, 40, 36, 36}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Polygon( new int[] { 6, 24, 24, 40, 40, 58, 58, 40, 24, 24, 6, 6}, new int[] {52, 62, 78, 78, 43, 52, 36, 26, 26, 45, 36, 52}, 12),
    new Polygon( new int[] { 31, 31, 26, 26, 31, 33, 38, 38, 33, 33, 31}, new int[] {26, 32, 35, 37, 34, 34, 37, 35, 32, 26, 26}, 11),
    new Rectangle2D.Float(24F, 35F, 2F, 18F),
    new Rectangle2D.Float(38F, 35F, 2F, 18F),
  };
  int original_width_p2 = 64;
  int original_height_p2 = 84;
  Shape[] shapes_p2 = new Shape[] { 
    new Rectangle2D.Float(26F, 6F, 12F, 14F),
    new Polygon( new int[] { 26, 30, 30, 26, 26}, new int[] {6, 6, 20, 20, 6}, 5),
    new Polygon( new int[] { 34, 38, 38, 34, 34}, new int[] {6, 6, 20, 20, 6}, 5),
    new Rectangle2D.Float(26F, 4F, 12F, 14F),
    new Rectangle2D.Float(34F, 2F, 8F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 12F, 4.44445F),
    new Rectangle2D.Float(24F, 2F, 4F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 20F, 4.44445F),
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 27}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 27}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 27, 29, 29, 58}, new int[] { 18, 23, 23,-2147483648}, 4),
    new Polygon( new int[] { 6, 16, 16, 6, 6}, new int[] {48, 54, 60, 54, 48}, 5),
    new Polygon( new int[] { 6, 9, 17, 16}, new int[] { 48, 50, 55,54}, 4),
    new Polygon( new int[] { 16, 17, 17, 16}, new int[] { 54, 55, 58,60}, 4),
    new Polygon( new int[] { 16, 17, 9, 6}, new int[] { 60, 58, 53,54}, 4),
    new Polygon( new int[] { 6, 9, 9, 6}, new int[] { 54, 53, 50,48}, 4),
    new Polygon( new int[] { 6, 9, 58, 58}, new int[] { 48, 50, -1070333924,27}, 4),
    new Polygon( new int[] { 6, 14, 16, 16, 14, 6, 6}, new int[] {34, 38, 36, 42, 42, 38, 34}, 7),
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {34, 30, 28, 34, 34, 38, 34}, 7),
    new Polygon( new int[] { 48, 42, 42, 40, 38, 38, 42, 42, 46, 48, 48}, new int[] {22, 22, 66, 66, 68, 78, 78, 68, 68, 66, 22}, 11),
    new Polygon( new int[] { 48, 49, 45, 42}, new int[] { 22, 25, 25,22}, 4),
    new Polygon( new int[] { 42, 45, 45, 42}, new int[] { 22, 25, 67,66}, 4),
    new Polygon( new int[] { 42, 45, 42, 40}, new int[] { 66, 67, 67,66}, 4),
    new Polygon( new int[] { 40, 42, 41, 38}, new int[] { 66, 67, 68,68}, 4),
    new Polygon( new int[] { 38, 41, 41, 38}, new int[] { 68, 68, 77,78}, 4),
    new Polygon( new int[] { 38, 41, 43, 42}, new int[] { 78, 77, 77,78}, 4),
    new Polygon( new int[] { 42, 43, 43, 42}, new int[] { 78, 77, 67,68}, 4),
    new Polygon( new int[] { 42, 43, 48, 46}, new int[] { 68, 67, 67,68}, 4),
    new Polygon( new int[] { 46, 48, 49, 48}, new int[] { 68, 67, 66,66}, 4),
    new Polygon( new int[] { 48, 49, 49, 48}, new int[] { 66, 66, 25,22}, 4),
    new Polygon( new int[] { 48, 49, -1220087349, 58}, new int[] { 22, 25, -1220087380,-2147483648}, 4),
    new Polygon( new int[] { 48, 22, 22, 16, 16, 18, 22, 26, 38, 42, 46, 48}, new int[] {22, 22, 66, 66, 22, 20, 20, 18, 18, 20, 20, 22}, 12),
    new Polygon( new int[] { 48, 45, 22, 22}, new int[] { 22, 22, 22,22}, 4),
    new Polygon( new int[] { 22, 22, 22, 22}, new int[] { 22, 22, 64,66}, 4),
    new Polygon( new int[] { 22, 22, 20, 16}, new int[] { 66, 64, 64,66}, 4),
    new Polygon( new int[] { 16, 20, 20, 16}, new int[] { 66, 64, 25,22}, 4),
    new Polygon( new int[] { 16, 20, 21, 18}, new int[] { 22, 25, 24,20}, 4),
    new Polygon( new int[] { 18, 21, 24, 22}, new int[] { 20, 24, 24,20}, 4),
    new Polygon( new int[] { 22, 24, 28, 26}, new int[] { 20, 24, 22,18}, 4),
    new Polygon( new int[] { 26, 28, 40, 38}, new int[] { 18, 22, 22,18}, 4),
    new Polygon( new int[] { 38, 40, 44, 42}, new int[] { 18, 22, 24,20}, 4),
    new Polygon( new int[] { 42, 44, 47, 46}, new int[] { 20, 24, 24,20}, 4),
    new Polygon( new int[] { 46, 47, 45, 48}, new int[] { 20, 24, 22,22}, 4),
    new Polygon( new int[] { 48, 45, 1923, 58}, new int[] { 22, 22, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(22F, 68F, 4F, 10F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 68, 68, 68, 68, 78, 78}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 78, 68, 68, 78, 78, 78}, 6), 
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {54, 50, 52, 44, 44, 48, 54}, 7),
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 26}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 26}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 26, 29, -1220087349, 58}, new int[] { 18, 23, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(2F, 50F, 4F, 8F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(2F, 32F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(2F, 46F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 50F, 4F, 8F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 32F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(58F, 46F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(22F, 78F, 4F, 4F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(40F, 78F, 6F, 4F),
    new Polygon( new int[] { 40, 46, 46, 40, 40, 40}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 40, 40}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(36F, 78F, 4F, 4F),
    new Polygon( new int[] { 36, 40, 40, 36, 36, 36}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 40, 40, 40, 40, 36, 36}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Polygon( new int[] { 6, 24, 24, 40, 40, 58, 58, 40, 24, 24, 6, 6}, new int[] {52, 62, 78, 78, 43, 52, 36, 26, 26, 45, 36, 52}, 12),
    new Polygon( new int[] { 6, 11, 29, 24}, new int[] { 52, 50, 60,62}, 4),
    new Polygon( new int[] { 24, 29, 29, 24}, new int[] { 62, 60, 75,78}, 4),
    new Polygon( new int[] { 24, 29, 39, 40}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 40, 39, 39, 40}, new int[] { 78, 75, 38,43}, 4),
    new Polygon( new int[] { 40, 39, 56, 58}, new int[] { 43, 38, 47,52}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 52, 47, 38,36}, 4),
    new Polygon( new int[] { 58, 56, 41, 40}, new int[] { 36, 38, 30,26}, 4),
    new Polygon( new int[] { 40, 41, 29, 24}, new int[] { 26, 30, 30,26}, 4),
    new Polygon( new int[] { 24, 29, 29, 24}, new int[] { 26, 30, 50,45}, 4),
    new Polygon( new int[] { 24, 29, 11, 6}, new int[] { 45, 50, 41,36}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 36, 41, 50,52}, 4),
    new Polygon( new int[] { 6, 11, 1923, 58}, new int[] { 52, 50, -1220087380,-2147483648}, 4),
    new Polygon( new int[] { 31, 31, 26, 26, 31, 33, 38, 38, 33, 33, 31}, new int[] {26, 41, 44, 46, 43, 43, 46, 44, 41, 26, 26}, 11),
    new Polygon( new int[] { 31, 34, 34, 31}, new int[] { 26, 28, 42,41}, 4),
    new Polygon( new int[] { 31, 34, 29, 26}, new int[] { 41, 42, 45,44}, 4),
    new Polygon( new int[] { 26, 29, 29, 26}, new int[] { 44, 45, 44,46}, 4),
    new Polygon( new int[] { 26, 29, 33, 31}, new int[] { 46, 44, 42,43}, 4),
    new Polygon( new int[] { 31, 33, 35, 33}, new int[] { 43, 42, 42,43}, 4),
    new Polygon( new int[] { 33, 35, 39, 38}, new int[] { 43, 42, 44,46}, 4),
    new Polygon( new int[] { 38, 39, 39, 38}, new int[] { 46, 44, 45,44}, 4),
    new Polygon( new int[] { 38, 39, 34, 33}, new int[] { 44, 45, 42,41}, 4),
    new Polygon( new int[] { 33, 34, 34, 33}, new int[] { 41, 42, 28,26}, 4),
    new Polygon( new int[] { 33, 34, 34, 31}, new int[] { 26, 28, 28,26}, 4),
    new Polygon( new int[] { 31, 34, -1220087349, 58}, new int[] { 26, 28, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(24F, 44F, 2F, 18F),
    new Polygon( new int[] { 24, 26, 26, 24, 24, 24}, new int[] { 44, 44, 44, 44, 62, 62}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 24, 24}, new int[] { 62, 44, 44, 62, 62, 62}, 6), 
    new Rectangle2D.Float(38F, 44F, 2F, 18F),
    new Polygon( new int[] { 38, 40, 40, 38, 38, 38}, new int[] { 44, 44, 44, 44, 62, 62}, 6), 
    new Polygon( new int[] { 40, 40, 40, 40, 38, 38}, new int[] { 62, 44, 44, 62, 62, 62}, 6), 
  };
  int original_width_p3 = 64;
  int original_height_p3 = 84;
  Shape[] shapes_p3 = new Shape[] { 
    new Rectangle2D.Float(26F, 6F, 12F, 14F),
    new Polygon( new int[] { 26, 30, 30, 26, 26}, new int[] {6, 6, 20, 20, 6}, 5),
    new Polygon( new int[] { 34, 38, 38, 34, 34}, new int[] {6, 6, 20, 20, 6}, 5),
    new Rectangle2D.Float(26F, 4F, 12F, 14F),
    new Rectangle2D.Float(34F, 2F, 8F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 12F, 4.44445F),
    new Rectangle2D.Float(24F, 2F, 4F, 4.44445F),
    new Rectangle2D.Float(22F, 2F, 20F, 4.44445F),
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 27}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 27}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 27, 29, 29, 58}, new int[] { 18, 23, 23,-2147483648}, 4),
    new Polygon( new int[] { 6, 16, 16, 6, 6}, new int[] {48, 54, 60, 54, 48}, 5),
    new Polygon( new int[] { 6, 9, 17, 16}, new int[] { 48, 50, 55,54}, 4),
    new Polygon( new int[] { 16, 17, 17, 16}, new int[] { 54, 55, 58,60}, 4),
    new Polygon( new int[] { 16, 17, 9, 6}, new int[] { 60, 58, 53,54}, 4),
    new Polygon( new int[] { 6, 9, 9, 6}, new int[] { 54, 53, 50,48}, 4),
    new Polygon( new int[] { 6, 9, 58, 58}, new int[] { 48, 50, -1070333924,27}, 4),
    new Polygon( new int[] { 6, 14, 16, 16, 14, 6, 6}, new int[] {34, 38, 36, 42, 42, 38, 34}, 7),
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {34, 30, 28, 34, 34, 38, 34}, 7),
    new Polygon( new int[] { 48, 42, 42, 40, 38, 38, 42, 42, 46, 48, 48}, new int[] {22, 22, 66, 66, 68, 78, 78, 68, 68, 66, 22}, 11),
    new Polygon( new int[] { 48, 49, 45, 42}, new int[] { 22, 25, 25,22}, 4),
    new Polygon( new int[] { 42, 45, 45, 42}, new int[] { 22, 25, 67,66}, 4),
    new Polygon( new int[] { 42, 45, 42, 40}, new int[] { 66, 67, 67,66}, 4),
    new Polygon( new int[] { 40, 42, 41, 38}, new int[] { 66, 67, 68,68}, 4),
    new Polygon( new int[] { 38, 41, 41, 38}, new int[] { 68, 68, 77,78}, 4),
    new Polygon( new int[] { 38, 41, 43, 42}, new int[] { 78, 77, 77,78}, 4),
    new Polygon( new int[] { 42, 43, 43, 42}, new int[] { 78, 77, 67,68}, 4),
    new Polygon( new int[] { 42, 43, 48, 46}, new int[] { 68, 67, 67,68}, 4),
    new Polygon( new int[] { 46, 48, 49, 48}, new int[] { 68, 67, 66,66}, 4),
    new Polygon( new int[] { 48, 49, 49, 48}, new int[] { 66, 66, 25,22}, 4),
    new Polygon( new int[] { 48, 49, -1220087349, 58}, new int[] { 22, 25, -1220087380,-2147483648}, 4),
    new Polygon( new int[] { 48, 22, 22, 16, 16, 18, 22, 26, 38, 42, 46, 48}, new int[] {22, 22, 66, 66, 22, 20, 20, 18, 18, 20, 20, 22}, 12),
    new Polygon( new int[] { 48, 45, 22, 22}, new int[] { 22, 22, 22,22}, 4),
    new Polygon( new int[] { 22, 22, 22, 22}, new int[] { 22, 22, 64,66}, 4),
    new Polygon( new int[] { 22, 22, 20, 16}, new int[] { 66, 64, 64,66}, 4),
    new Polygon( new int[] { 16, 20, 20, 16}, new int[] { 66, 64, 25,22}, 4),
    new Polygon( new int[] { 16, 20, 21, 18}, new int[] { 22, 25, 24,20}, 4),
    new Polygon( new int[] { 18, 21, 24, 22}, new int[] { 20, 24, 24,20}, 4),
    new Polygon( new int[] { 22, 24, 28, 26}, new int[] { 20, 24, 22,18}, 4),
    new Polygon( new int[] { 26, 28, 40, 38}, new int[] { 18, 22, 22,18}, 4),
    new Polygon( new int[] { 38, 40, 44, 42}, new int[] { 18, 22, 24,20}, 4),
    new Polygon( new int[] { 42, 44, 47, 46}, new int[] { 20, 24, 24,20}, 4),
    new Polygon( new int[] { 46, 47, 45, 48}, new int[] { 20, 24, 22,22}, 4),
    new Polygon( new int[] { 48, 45, 1923, 58}, new int[] { 22, 22, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(22F, 68F, 4F, 10F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 68, 68, 68, 68, 78, 78}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 78, 68, 68, 78, 78, 78}, 6), 
    new Polygon( new int[] { 58, 50, 48, 48, 50, 58, 58}, new int[] {54, 50, 52, 44, 44, 48, 54}, 7),
    new Polygon( new int[] { 26, 22, 18, 16, 16, 14, 6, 6, 14, 16, 16, 18, 22, 22, 42, 42, 46, 48, 48, 50, 58, 58, 50, 48, 48, 46, 42, 38, 26}, new int[] {18, 20, 20, 22, 36, 38, 34, 54, 58, 60, 66, 68, 68, 78, 78, 68, 68, 66, 52, 50, 54, 34, 30, 28, 22, 20, 20, 18, 18}, 29),
    new Polygon( new int[] { 26, 29, 25, 22}, new int[] { 18, 23, 25,20}, 4),
    new Polygon( new int[] { 22, 25, 21, 18}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 18, 21, 21, 16}, new int[] { 20, 25, 25,22}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 22, 25, 37,36}, 4),
    new Polygon( new int[] { 16, 21, 17, 14}, new int[] { 36, 37, 42,38}, 4),
    new Polygon( new int[] { 14, 17, 11, 6}, new int[] { 38, 42, 39,34}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 34, 39, 52,54}, 4),
    new Polygon( new int[] { 6, 11, 18, 14}, new int[] { 54, 52, 56,58}, 4),
    new Polygon( new int[] { 14, 18, 21, 16}, new int[] { 58, 56, 59,60}, 4),
    new Polygon( new int[] { 16, 21, 21, 16}, new int[] { 60, 59, 65,66}, 4),
    new Polygon( new int[] { 16, 21, 21, 18}, new int[] { 66, 65, 65,68}, 4),
    new Polygon( new int[] { 18, 21, 27, 22}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 22, 27, 27, 22}, new int[] { 68, 65, 75,78}, 4),
    new Polygon( new int[] { 22, 27, 41, 42}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 78, 75, 65,68}, 4),
    new Polygon( new int[] { 42, 41, 47, 46}, new int[] { 68, 65, 65,68}, 4),
    new Polygon( new int[] { 46, 47, 47, 48}, new int[] { 68, 65, 65,66}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 66, 65, 51,52}, 4),
    new Polygon( new int[] { 48, 47, 51, 50}, new int[] { 52, 51, 46,50}, 4),
    new Polygon( new int[] { 50, 51, 56, 58}, new int[] { 50, 46, 49,54}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 54, 49, 36,34}, 4),
    new Polygon( new int[] { 58, 56, 50, 50}, new int[] { 34, 36, 32,30}, 4),
    new Polygon( new int[] { 50, 50, 47, 48}, new int[] { 30, 32, 29,28}, 4),
    new Polygon( new int[] { 48, 47, 47, 48}, new int[] { 28, 29, 25,22}, 4),
    new Polygon( new int[] { 48, 47, 47, 46}, new int[] { 22, 25, 25,20}, 4),
    new Polygon( new int[] { 46, 47, 43, 42}, new int[] { 20, 25, 25,20}, 4),
    new Polygon( new int[] { 42, 43, 39, 38}, new int[] { 20, 25, 23,18}, 4),
    new Polygon( new int[] { 38, 39, 29, 26}, new int[] { 18, 23, 23,18}, 4),
    new Polygon( new int[] { 26, 29, -1220087349, 58}, new int[] { 18, 23, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(2F, 50F, 4F, 8F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(2F, 32F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(2F, 46F, 4F, 4F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(2F, 30F, 4F, 28F),
    new Polygon( new int[] { 2, 6, 6, 2, 2, 2}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 6, 6, 6, 6, 2, 2}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 50F, 4F, 8F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 50, 50, 50, 50, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 50, 50, 58, 58, 58}, 6), 
    new Rectangle2D.Float(58F, 32F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 32, 32, 32, 32, 36, 36}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 36, 32, 32, 36, 36, 36}, 6), 
    new Rectangle2D.Float(58F, 46F, 4F, 4F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 46, 46, 46, 46, 50, 50}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 50, 46, 46, 50, 50, 50}, 6), 
    new Rectangle2D.Float(58F, 30F, 4F, 28F),
    new Polygon( new int[] { 58, 62, 62, 58, 58, 58}, new int[] { 30, 30, 30, 30, 58, 58}, 6), 
    new Polygon( new int[] { 62, 62, 62, 62, 58, 58}, new int[] { 58, 30, 30, 58, 58, 58}, 6), 
    new Rectangle2D.Float(22F, 78F, 4F, 4F),
    new Polygon( new int[] { 22, 26, 26, 22, 22, 22}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 22, 22}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(40F, 78F, 6F, 4F),
    new Polygon( new int[] { 40, 46, 46, 40, 40, 40}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 40, 40}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(36F, 78F, 4F, 4F),
    new Polygon( new int[] { 36, 40, 40, 36, 36, 36}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 40, 40, 40, 40, 36, 36}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Rectangle2D.Float(18F, 78F, 28F, 4F),
    new Polygon( new int[] { 18, 46, 46, 18, 18, 18}, new int[] { 78, 78, 78, 78, 82, 82}, 6), 
    new Polygon( new int[] { 46, 46, 46, 46, 18, 18}, new int[] { 82, 78, 78, 82, 82, 82}, 6), 
    new Polygon( new int[] { 6, 24, 24, 40, 40, 58, 58, 40, 24, 24, 6, 6}, new int[] {52, 62, 78, 78, 43, 52, 36, 26, 26, 45, 36, 52}, 12),
    new Polygon( new int[] { 6, 11, 29, 24}, new int[] { 52, 50, 60,62}, 4),
    new Polygon( new int[] { 24, 29, 29, 24}, new int[] { 62, 60, 75,78}, 4),
    new Polygon( new int[] { 24, 29, 39, 40}, new int[] { 78, 75, 75,78}, 4),
    new Polygon( new int[] { 40, 39, 39, 40}, new int[] { 78, 75, 38,43}, 4),
    new Polygon( new int[] { 40, 39, 56, 58}, new int[] { 43, 38, 47,52}, 4),
    new Polygon( new int[] { 58, 56, 56, 58}, new int[] { 52, 47, 38,36}, 4),
    new Polygon( new int[] { 58, 56, 41, 40}, new int[] { 36, 38, 30,26}, 4),
    new Polygon( new int[] { 40, 41, 29, 24}, new int[] { 26, 30, 30,26}, 4),
    new Polygon( new int[] { 24, 29, 29, 24}, new int[] { 26, 30, 50,45}, 4),
    new Polygon( new int[] { 24, 29, 11, 6}, new int[] { 45, 50, 41,36}, 4),
    new Polygon( new int[] { 6, 11, 11, 6}, new int[] { 36, 41, 50,52}, 4),
    new Polygon( new int[] { 6, 11, 1923, 58}, new int[] { 52, 50, -1220087380,-2147483648}, 4),
    new Rectangle2D.Float(24F, 26F, 2F, 18F),
    new Polygon( new int[] { 24, 26, 26, 24, 24, 24}, new int[] { 26, 26, 26, 26, 44, 44}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 24, 24}, new int[] { 44, 26, 26, 44, 44, 44}, 6), 
    new Rectangle2D.Float(38F, 26F, 2F, 18F),
    new Polygon( new int[] { 38, 40, 40, 38, 38, 38}, new int[] { 26, 26, 26, 26, 44, 44}, 6), 
    new Polygon( new int[] { 40, 40, 40, 40, 38, 38}, new int[] { 44, 26, 26, 44, 44, 44}, 6), 
    new Polygon( new int[] { 26, 28, 26, 26}, new int[] {26, 26, 28, 26}, 4),
    new Polygon( new int[] { 38, 36, 38, 38}, new int[] {28, 26, 26, 28}, 4),
  };
  public void paintComponent(Graphics g1) {
    switch ( page) {
      case 2 :
        paintComponent_p2(g1);
        return;
      case 3 :
        paintComponent_p3(g1);
        return;
      default:
        ;
    }
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(32, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(32, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[4]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[5]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[6]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[7]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[8]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[8]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[9]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[10]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,18F,78F,28F,4F, false,2, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[13]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[14]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,17.9369F,52F,60.0631F, false,34, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[14]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[16]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[17]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[18]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[19]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[21]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[22]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[23]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[24]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[25]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[26]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[27]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[28]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[29]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[30]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[31]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[32]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[33]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[34]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[35]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[36]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[37]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[38]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[39]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[40]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[41]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[42]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[44]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,48F,10F,12F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[44]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[45]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[46]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[47]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[48]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[50]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[51]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[52]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[53]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[54]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,68F,4F,10F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[54]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[55]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[56]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[57]);
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[59]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[60]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[61]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[62]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[63]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[64]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[65]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[66]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[67]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[68]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[69]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[70]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[71]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[72]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[73]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[74]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[75]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[76]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[77]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[78]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[79]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[80]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[81]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[82]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[83]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[84]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[85]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[86]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[58]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[88]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[88]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[89]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[90]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[91]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[91]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[92]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[93]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[94]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[94]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[95]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[96]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[98]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[99]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[97]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[100]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[100]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[101]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[102]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[103]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[103]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[104]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[105]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[106]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[106]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[107]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[108]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[109]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[109]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[110]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[111]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[113]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[114]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[112]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[115]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,78F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[115]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[116]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[117]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[118]);
    } else {
    GeGradient.paint( g, gradient,2,-2,40F,78F,6F,4F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[118]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[119]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[120]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[121]);
    } else {
    GeGradient.paint( g, gradient,2,-2,36F,78F,4F,4F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[121]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[122]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[123]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[125]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[126]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[124]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[127]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[127]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[128]);
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[129]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[129]);
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[130]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[130]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public void paintComponent_p2(Graphics g1) {
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[0]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(32, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(32, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[1]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[2]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[4]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[5]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[6]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[7]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[8]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[8]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[9]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[10]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,18F,78F,28F,4F, false,2, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[11]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[12]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[13]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[14]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,17.9369F,52F,60.0631F, false,34, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[14]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[15]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[16]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[17]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[18]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[19]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[21]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[22]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[23]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[24]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[25]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[26]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[27]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[28]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[29]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[30]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[31]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[32]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[33]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[34]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[35]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[36]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[37]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[38]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[39]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[40]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[41]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[42]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[44]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,48F,10F,12F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[44]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[45]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[46]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[47]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[48]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[50]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[51]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[52]);
    } else {
    GeGradient.paint( g, gradient,2,-2,38F,22F,10F,56F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[52]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[53]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[54]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[55]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[56]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[57]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[58]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[59]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[60]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[61]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[62]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[64]);
    } else {
    GeGradient.paint( g, gradient,2,-2,16F,18F,32F,48F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[64]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[65]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[66]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[67]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[68]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[69]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[70]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[71]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[72]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[73]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[74]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[75]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[77]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,68F,4F,10F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[77]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[78]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[79]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[80]);
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[82]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[83]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[84]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[85]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[86]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[87]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[88]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[89]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[90]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[91]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[92]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[93]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[94]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[95]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[96]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[97]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[98]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[99]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[100]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[101]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[102]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[103]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[104]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[105]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[106]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[107]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[108]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[109]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[81]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[111]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[111]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[112]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[113]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[114]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[114]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[115]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[116]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[117]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[117]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[118]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[119]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[121]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[122]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[120]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[123]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[123]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[124]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[125]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[126]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[126]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[127]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[128]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[129]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[129]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[130]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[131]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[132]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[132]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[133]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[134]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[136]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[137]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[135]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[138]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,78F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[138]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[139]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[140]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[141]);
    } else {
    GeGradient.paint( g, gradient,2,-2,40F,78F,6F,4F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[141]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[142]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[143]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[144]);
    } else {
    GeGradient.paint( g, gradient,2,-2,36F,78F,4F,4F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[144]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[145]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[146]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[148]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[149]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[147]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[150]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,26F,52F,52F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[150]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[151]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[152]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[153]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[154]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[155]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[156]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[157]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[158]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[159]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[160]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[161]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[150]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[163]);
    } else {
    GeGradient.paint( g, gradient,2,-2,26F,26F,12F,20F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[163]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[164]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[165]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[166]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[167]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[168]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[169]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[170]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[171]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[172]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[173]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[175]);
    } else {
    GeGradient.paint( g, gradient,2,-2,24F,44F,2F,18F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[175]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[176]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[177]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[175]);
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[178]);
    } else {
    GeGradient.paint( g, gradient,2,-2,38F,44F,2F,18F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[178]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[179]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[180]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[178]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public void paintComponent_p3(Graphics g1) {
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[0]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(32, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(32, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[1]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[2]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[4]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[5]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[6]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[7]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[8]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[8]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[9]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[10]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,18F,78F,28F,4F, false,2, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[11]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[12]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[13]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[14]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,17.9369F,52F,60.0631F, false,34, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[14]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[15]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[16]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[17]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[18]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[19]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[21]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[22]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[23]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[24]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[25]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[26]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[27]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[28]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[29]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[30]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[31]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[32]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[33]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[34]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[35]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[36]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[37]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[38]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[39]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[40]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[41]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[42]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[44]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,48F,10F,12F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[44]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[45]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[46]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[47]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[48]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[50]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[51]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[52]);
    } else {
    GeGradient.paint( g, gradient,2,-2,38F,22F,10F,56F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[52]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[53]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[54]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[55]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[56]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[57]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[58]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[59]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[60]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[61]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[62]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[64]);
    } else {
    GeGradient.paint( g, gradient,2,-2,16F,18F,32F,48F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[64]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[65]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[66]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[67]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[68]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[69]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[70]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[71]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[72]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[73]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[74]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[75]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[77]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,68F,4F,10F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[77]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[78]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[79]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[80]);
    }
    {
    int fcolor = GeColor.getDrawtype(34, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[82]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[83]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[84]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[85]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[86]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[87]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[88]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[89]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[90]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[91]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[92]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[93]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[94]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[95]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[96]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[97]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[98]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[99]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[100]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[101]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[102]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[103]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[104]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[105]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[106]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[107]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[108]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[109]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[81]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[111]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[111]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[112]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[113]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[114]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[114]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[115]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[116]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[117]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[117]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[118]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[119]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[121]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[122]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[120]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[123]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,30F,4F,28F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[123]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[124]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[125]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[126]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,50F,4F,8F, false,37, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[126]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[127]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[128]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[129]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,32F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[129]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[130]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[131]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[132]);
    } else {
    GeGradient.paint( g, gradient,2,-2,58F,46F,4F,4F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[132]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[133]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[134]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[136]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[137]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[135]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[138]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,78F,4F,4F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[138]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[139]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[140]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[141]);
    } else {
    GeGradient.paint( g, gradient,2,-2,40F,78F,6F,4F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[141]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[142]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[143]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[144]);
    } else {
    GeGradient.paint( g, gradient,2,-2,36F,78F,4F,4F, false,36, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[144]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[145]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[146]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[148]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[149]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[147]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[150]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,26F,52F,52F, false,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[150]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[151]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[152]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[153]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[154]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[155]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[156]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[157]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[158]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[159]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[160]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[161]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[150]);
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[163]);
    } else {
    GeGradient.paint( g, gradient,2,-2,24F,26F,2F,18F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[163]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[164]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[165]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[163]);
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[166]);
    } else {
    GeGradient.paint( g, gradient,2,-2,38F,26F,2F,18F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[166]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[167]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[168]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[166]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[169]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[170]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valve3waycontrol2 extends GeComponent {
  // Dimension size;
  public pwr_valve3waycontrol2( JopSession session)
  {
    super( session);
    size = new Dimension( 66, 53);
    annot1Font = new Font("Helvetica", Font.BOLD, 14);
    pages = 3;
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 66;
  public int original_height = 53;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 2, 2, 22, 2}, new int[] {22, 42, 32, 22}, 4),
    new Polygon( new int[] { 2, 6, 6, 2}, new int[] { 22, 27, 41,42}, 4),
    new Polygon( new int[] { 2, 6, 20, 22}, new int[] { 42, 41, 34,32}, 4),
    new Polygon( new int[] { 22, 20, 6, 2}, new int[] { 32, 34, 27,22}, 4),
    new Polygon( new int[] { 2, 6, 201, 48}, new int[] { 22, 27, 140891201,57}, 4),
    new Polygon( new int[] { 42, 42, 22, 42}, new int[] {22, 42, 32, 22}, 4),
    new Polygon( new int[] { 42, 42, 42, 42}, new int[] { 22, 27, 41,42}, 4),
    new Polygon( new int[] { 42, 42, 28, 22}, new int[] { 42, 41, 34,32}, 4),
    new Polygon( new int[] { 22, 28, 42, 42}, new int[] { 32, 34, 27,22}, 4),
    new Polygon( new int[] { 42, 42, 201, 48}, new int[] { 22, 27, 140891201,57}, 4),
    new Arc2D.Float(12F, 2F, 20F, 20F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(14F, 4F, 16F, 16F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 0F, 360F, Arc2D.PIE),
    new Rectangle2D.Float(44.3132F, 12.049F, 20F, 16F),
    new Line2D.Float( 22F, 32F, 22F, 22F),
    new Polygon( new int[] { 22, 12, 32, 22}, new int[] {32, 52, 52, 32}, 4),
    new Polygon( new int[] { 22, 24, 17, 12}, new int[] { 32, 38, 52,52}, 4),
    new Polygon( new int[] { 12, 17, 31, 32}, new int[] { 52, 52, 52,52}, 4),
    new Polygon( new int[] { 32, 31, 24, 22}, new int[] { 52, 52, 38,32}, 4),
    new Polygon( new int[] { 22, 24, 201, 48}, new int[] { 32, 38, 140891201,57}, 4),
    new Line2D.Float( 30F, 48F, 14F, 48F),
    new Line2D.Float( 38F, 40F, 38F, 24F),
  };
  int original_width_p2 = 66;
  int original_height_p2 = 53;
  Shape[] shapes_p2 = new Shape[] { 
    new Polygon( new int[] { 2, 2, 22, 2}, new int[] {22, 42, 32, 22}, 4),
    new Polygon( new int[] { 2, 6, 6, 2}, new int[] { 22, 27, 41,42}, 4),
    new Polygon( new int[] { 2, 6, 20, 22}, new int[] { 42, 41, 34,32}, 4),
    new Polygon( new int[] { 22, 20, 6, 2}, new int[] { 32, 34, 27,22}, 4),
    new Polygon( new int[] { 2, 6, 201, 48}, new int[] { 22, 27, 140891201,57}, 4),
    new Polygon( new int[] { 42, 42, 22, 42}, new int[] {22, 42, 32, 22}, 4),
    new Polygon( new int[] { 42, 42, 42, 42}, new int[] { 22, 27, 41,42}, 4),
    new Polygon( new int[] { 42, 42, 28, 22}, new int[] { 42, 41, 34,32}, 4),
    new Polygon( new int[] { 22, 28, 42, 42}, new int[] { 32, 34, 27,22}, 4),
    new Polygon( new int[] { 42, 42, 201, 48}, new int[] { 22, 27, 140891201,57}, 4),
    new Arc2D.Float(12F, 2F, 20F, 20F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(14F, 4F, 16F, 16F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 0F, 360F, Arc2D.PIE),
    new Rectangle2D.Float(44.3132F, 12.049F, 20F, 16F),
    new Line2D.Float( 22F, 32F, 22F, 22F),
    new Polygon( new int[] { 22, 12, 32, 22}, new int[] {32, 52, 52, 32}, 4),
    new Polygon( new int[] { 22, 24, 17, 12}, new int[] { 32, 38, 52,52}, 4),
    new Polygon( new int[] { 12, 17, 31, 32}, new int[] { 52, 52, 52,52}, 4),
    new Polygon( new int[] { 32, 31, 24, 22}, new int[] { 52, 52, 38,32}, 4),
    new Polygon( new int[] { 22, 24, 201, 48}, new int[] { 32, 38, 140891201,57}, 4),
    new Line2D.Float( 30F, 48F, 14F, 48F),
    new Line2D.Float( 38F, 40F, 38F, 24F),
  };
  int original_width_p3 = 66;
  int original_height_p3 = 53;
  Shape[] shapes_p3 = new Shape[] { 
    new Polygon( new int[] { 2, 2, 22, 2}, new int[] {22, 42, 32, 22}, 4),
    new Polygon( new int[] { 2, 6, 6, 2}, new int[] { 22, 27, 41,42}, 4),
    new Polygon( new int[] { 2, 6, 20, 22}, new int[] { 42, 41, 34,32}, 4),
    new Polygon( new int[] { 22, 20, 6, 2}, new int[] { 32, 34, 27,22}, 4),
    new Polygon( new int[] { 2, 6, 201, 48}, new int[] { 22, 27, 140891201,57}, 4),
    new Polygon( new int[] { 42, 42, 22, 42}, new int[] {22, 42, 32, 22}, 4),
    new Polygon( new int[] { 42, 42, 42, 42}, new int[] { 22, 27, 41,42}, 4),
    new Polygon( new int[] { 42, 42, 28, 22}, new int[] { 42, 41, 34,32}, 4),
    new Polygon( new int[] { 22, 28, 42, 42}, new int[] { 32, 34, 27,22}, 4),
    new Polygon( new int[] { 42, 42, 201, 48}, new int[] { 22, 27, 140891201,57}, 4),
    new Arc2D.Float(12F, 2F, 20F, 20F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(14F, 4F, 16F, 16F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(12F, 2F, 20F, 20F, 0F, 360F, Arc2D.PIE),
    new Rectangle2D.Float(44.3132F, 12.049F, 20F, 16F),
    new Line2D.Float( 22F, 32F, 22F, 22F),
    new Polygon( new int[] { 22, 12, 32, 22}, new int[] {32, 52, 52, 32}, 4),
    new Polygon( new int[] { 22, 24, 17, 12}, new int[] { 32, 38, 52,52}, 4),
    new Polygon( new int[] { 12, 17, 31, 32}, new int[] { 52, 52, 52,52}, 4),
    new Polygon( new int[] { 32, 31, 24, 22}, new int[] { 52, 52, 38,32}, 4),
    new Polygon( new int[] { 22, 24, 201, 48}, new int[] { 32, 38, 140891201,57}, 4),
    new Line2D.Float( 30F, 48F, 14F, 48F),
    new Line2D.Float( 38F, 40F, 38F, 24F),
  };
  public void paintComponent(Graphics g1) {
    switch ( page) {
      case 2 :
        paintComponent_p2(g1);
        return;
      case 3 :
        paintComponent_p3(g1);
        return;
      default:
        ;
    }
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,22F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[3]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[5]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,22F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[5]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[8]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    ((Arc2D)shapes[10]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[11]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[12]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[13]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[14]).setArcType(Arc2D.PIE);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[10]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[11]);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[12]);
    g.fill( shapes[13]);
    g.fill( shapes[14]);
    } else {
    GeGradient.paint( g, 9,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[15]);
    GeGradient.paint( g, gradient,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[14]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[15]);
    } else {
    GeGradient.paint( g, gradient,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[15]);
    }
    }
    }
    ((Arc2D)shapes[15]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[15]);
    {
    int fcolor = 300;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[16]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 44 * original_height / height * width / original_width, 26F);
    g.setTransform( save_tmp);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[17]);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[18]);
    } else {
    GeGradient.paint( g, gradient,2,-2,12F,32F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[18]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[19]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[21]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[18]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[23]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[24]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public void paintComponent_p2(Graphics g1) {
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,22F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[2]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[3]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[0]);
    }
    {
    int fcolor = 30;
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[5]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,22F,20F,20F, true,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[5]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[7]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[8]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[5]);
    }
    ((Arc2D)shapes_p2[10]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p2[11]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p2[12]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p2[13]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p2[14]).setArcType(Arc2D.PIE);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[10]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[11]);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[12]);
    g.fill( shapes_p2[13]);
    g.fill( shapes_p2[14]);
    } else {
    GeGradient.paint( g, 9,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[15]);
    GeGradient.paint( g, gradient,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[14]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[15]);
    } else {
    GeGradient.paint( g, gradient,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[15]);
    }
    }
    }
    ((Arc2D)shapes_p2[15]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[15]);
    {
    int fcolor = 300;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[16]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[17]);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[18]);
    } else {
    GeGradient.paint( g, gradient,2,-2,12F,32F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[18]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[19]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[21]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[18]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[23]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[24]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public void paintComponent_p3(Graphics g1) {
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,22F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[2]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[3]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[5]);
    } else {
    GeGradient.paint( g, gradient,2,-2,22F,22F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[5]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[7]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[8]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[5]);
    }
    ((Arc2D)shapes_p3[10]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p3[11]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p3[12]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p3[13]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes_p3[14]).setArcType(Arc2D.PIE);
    {
    int fcolor = GeColor.getDrawtype(74, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[10]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[11]);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[12]);
    g.fill( shapes_p3[13]);
    g.fill( shapes_p3[14]);
    } else {
    GeGradient.paint( g, 9,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[15]);
    GeGradient.paint( g, gradient,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[14]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[15]);
    } else {
    GeGradient.paint( g, gradient,2,-2,12F,2F,20F,20F, false,74, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[15]);
    }
    }
    }
    ((Arc2D)shapes_p3[15]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[15]);
    {
    int fcolor = 300;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[16]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[17]);
    {
    int fcolor = 30;
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p3[18]);
    } else {
    GeGradient.paint( g, gradient,2,-2,12F,32F,20F,20F, true,30, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p3[18]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[19]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p3[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p3[21]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[18]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[23]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p3[24]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_conglue extends GeComponent {
  // Dimension size;
  public pwr_conglue( JopSession session)
  {
    super( session);
    size = new Dimension( 24, 24);
  }
  public int original_width = 24;
  public int original_height = 24;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 20F, 20F),
    new Polygon( new int[] { 2, 22, 21, 3, 3, 2}, new int[] { 2, 2, 3, 3, 21, 22}, 6), 
    new Polygon( new int[] { 22, 22, 21, 21, 3, 2}, new int[] { 22, 2, 3, 21, 21, 22}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
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
protected class pwr_indround extends GeComponent {
  // Dimension size;
  public pwr_indround( JopSession session)
  {
    super( session);
    size = new Dimension( 18, 18);
  }
  public int original_width = 18;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 2F, 14F, 14F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 14F, 14F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 14F, 14F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 14F, 14F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(4.1F, 4.1F, 9.8F, 9.8F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 14F, 14F, 0F, 360F, Arc2D.PIE),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    ((Arc2D)shapes[0]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[1]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[2]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[3]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[4]).setArcType(Arc2D.PIE);
    {
    int fcolor = GeColor.getDrawtype(293, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[0]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    g.fill( shapes[3]);
    g.fill( shapes[4]);
    } else {
    GeGradient.paint( g, 9,2,-2,2F,2F,14F,14F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[5]);
    GeGradient.paint( g, gradient,2,-2,2F,2F,14F,14F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[4]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[5]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,14F,14F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[5]);
    }
    }
    }
    ((Arc2D)shapes[5]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuelong extends GeComponent {
  // Dimension size;
  public pwr_valuelong( JopSession session)
  {
    super( session);
    size = new Dimension( 536, 18);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 536;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 532F, 14F),
    new Polygon( new int[] { 2, 534, 533, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 534, 534, 533, 533, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,532F,14F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
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
      g.drawString( annot1, 7 * original_height / height * width / original_width, 13F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuesmall extends GeComponent {
  // Dimension size;
  public pwr_valuesmall( JopSession session)
  {
    super( session);
    size = new Dimension( 49, 18);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 49;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 46F, 14F),
    new Polygon( new int[] { 2, 48, 47, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 48, 48, 47, 47, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,46F,14F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
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
      g.drawString( annot1, 7 * original_height / height * width / original_width, 13F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valueinputreliefup extends GeTextField {
  public pwr_valueinputreliefup( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( 41);
  }
  int original_width = 49;
  int original_height = 18;
  boolean fontSet = false;
  public void paintComponent(Graphics g1) {
    if ( !fontSet) {
      float width = getWidth();
      float height = getHeight();
      setFont( annotFont.deriveFont((float)(height / original_height * annotFont.getSize())));
      fontSet = true;
    }
    super.paintComponent( g1);
  }
}
protected class pwr_sliderbackground3 extends GeComponent {
  // Dimension size;
  public pwr_sliderbackground3( JopSession session)
  {
    super( session);
    size = new Dimension( 64, 244);
  }
  public int original_width = 64;
  public int original_height = 244;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 60F, 240F),
    new Polygon( new int[] { 2, 62, 61, 3, 3, 2}, new int[] { 2, 2, 3, 3, 241, 242}, 6), 
    new Polygon( new int[] { 62, 62, 61, 61, 3, 2}, new int[] { 242, 2, 3, 241, 241, 242}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(35, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,60F,240F, false,35, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_slider1 extends GeComponent {
  // Dimension size;
  public pwr_slider1( JopSession session)
  {
    super( session);
    size = new Dimension( 34, 32);
  }
  public int original_width = 34;
  public int original_height = 32;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 24, 32, 32, 12, 2, 24, 24}, new int[] {2, 24, 30, 30, 22, 22, 2}, 7),
    new Rectangle2D.Float(2F, 2F, 22F, 20F),
    new Rectangle2D.Float(2F, 2F, 22F, 6F),
    new Rectangle2D.Float(2F, 16F, 22F, 6F),
    new Line2D.Float( 24F, 11.3482F, 2F, 11.3482F),
    new Line2D.Float( 24F, 12.391F, 2F, 12.391F),
    new Rectangle2D.Float(2F, 2F, 22F, 20F),
    new Polygon( new int[] { 2, 24, 23, 3, 3, 2}, new int[] { 2, 2, 3, 3, 21, 22}, 6), 
    new Polygon( new int[] { 24, 24, 23, 23, 3, 2}, new int[] { 22, 2, 3, 21, 21, 22}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(102, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    }
    {
    int fcolor = GeColor.getDrawtype(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    {
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[8]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(29, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valueinputsmallrd extends GeTextField {
  public pwr_valueinputsmallrd( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( 21);
  }
  int original_width = 56;
  int original_height = 24;
  boolean fontSet = false;
  public void paintComponent(Graphics g1) {
    if ( !fontSet) {
      float width = getWidth();
      float height = getHeight();
      setFont( annotFont.deriveFont((float)(height / original_height * annotFont.getSize())));
      fontSet = true;
    }
    super.paintComponent( g1);
  }
}
protected class pwr_sliderbackground1_2 extends GeComponent {
  // Dimension size;
  public pwr_sliderbackground1_2( JopSession session)
  {
    super( session);
    size = new Dimension( 44, 204);
  }
  public int original_width = 44;
  public int original_height = 204;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 40F, 200F),
    new Polygon( new int[] { 2, 42, 40, 4, 4, 2}, new int[] { 2, 2, 4, 4, 200, 202}, 6), 
    new Polygon( new int[] { 42, 42, 40, 40, 4, 2}, new int[] { 202, 2, 4, 200, 200, 202}, 6), 
    new Rectangle2D.Float(18F, 12F, 8F, 180F),
    new Polygon( new int[] { 18, 26, 26, 18, 18, 18}, new int[] { 12, 12, 12, 12, 192, 192}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 18, 18}, new int[] { 192, 12, 12, 192, 192, 192}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,40F,200F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    } else {
    GeGradient.paint( g, gradient,2,-2,18F,12F,8F,180F, false,38, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[3]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_actuator extends GeComponent {
  // Dimension size;
  public pwr_actuator( JopSession session)
  {
    super( session);
    size = new Dimension( 84, 54);
  }
  public int original_width = 84;
  public int original_height = 54;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 72, 82, 82, 72, 72}, new int[] {30, 30, 40, 40, 30}, 5),
    new Polygon( new int[] { 72, 82, 82, 72, 72}, new int[] {18, 18, 30, 30, 18}, 5),
    new Polygon( new int[] { 72, 82, 82, 72, 72}, new int[] {8, 8, 18, 18, 8}, 5),
    new Polygon( new int[] { 2, 12, 12, 2, 2}, new int[] {30, 30, 40, 40, 30}, 5),
    new Polygon( new int[] { 2, 12, 12, 2, 2}, new int[] {18, 18, 30, 30, 18}, 5),
    new Polygon( new int[] { 2, 12, 12, 2, 2}, new int[] {8, 8, 18, 18, 8}, 5),
    new Rectangle2D.Float(2F, 8F, 10F, 32F),
    new Rectangle2D.Float(44F, 46F, 8F, 6F),
    new Rectangle2D.Float(32F, 46F, 12F, 6F),
    new Rectangle2D.Float(34F, 46F, 4F, 6F),
    new Rectangle2D.Float(32F, 46F, 20F, 6F),
    new Rectangle2D.Float(12F, 2F, 60F, 44F),
    new Polygon( new int[] { 12, 72, 69, 15, 15, 12}, new int[] { 2, 2, 5, 5, 43, 46}, 6), 
    new Polygon( new int[] { 72, 72, 69, 69, 15, 12}, new int[] { 46, 2, 5, 43, 43, 46}, 6), 
    new Rectangle2D.Float(72F, 8F, 10F, 32F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(36, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(36, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(36, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(36, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(36, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(36, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[4]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(36, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(36, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[5]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    {
    int fcolor = GeColor.getDrawtype(37, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[7]);
    }
    {
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[8]);
    }
    {
    int fcolor = GeColor.getDrawtype(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[9]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[10]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,12F,2F,60F,44F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[13]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[11]);
    }
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[14]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_menubar2 extends GeComponent {
  // Dimension size;
  public pwr_menubar2( JopSession session)
  {
    super( session);
    size = new Dimension( 554, 24);
  }
  public int original_width = 554;
  public int original_height = 24;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 550F, 20F),
    new Polygon( new int[] { 2, 2, 552, 552, 2}, new int[] {22, 20, 20, 22, 22}, 5),
    new Line2D.Float( 552F, 22F, 2F, 22F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(31, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(31, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(31, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[2]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_pulldownmenu2 extends GeComponent {
  // Dimension size;
  public pwr_pulldownmenu2( JopSession session)
  {
    super( session);
    size = new Dimension( 64, 24);
    annot1Font = new Font("Helvetica", Font.PLAIN, 12);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 64;
  public int original_height = 24;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 60F, 20F),
    new Polygon( new int[] { 2, 2, 62, 62, 2}, new int[] {20, 22, 22, 20, 20}, 5),
    new Line2D.Float( 62F, 22F, 2F, 22F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(31, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(31, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(31, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[2]);
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 7 * original_height / height * width / original_width, 17F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbopenobject extends GeComponent {
  // Dimension size;
  public pwr_mbopenobject( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Rectangle2D.Float(6F, 6F, 8F, 8F),
    new Polygon( new int[] { 6, 14, 14, 6, 6, 6}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 14, 14, 14, 14, 6, 6}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Line2D.Float( 14F, 8F, 6F, 8F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(38, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbtrend extends GeComponent {
  // Dimension size;
  public pwr_mbtrend( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Line2D.Float( 14F, 14F, 6F, 14F),
    new Line2D.Float( 14F, 6F, 14F, 14F),
    new Polygon( new int[] { 14, 12, 10, 7, 5, 7, 10, 12}, new int[] {11, 11, 9, 11, 9, 11, 9, 11}, 8),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbfast extends GeComponent {
  // Dimension size;
  public pwr_mbfast( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Line2D.Float( 15.4286F, 14F, 5.99999F, 14F),
    new Line2D.Float( 6F, 6F, 6F, 14F),
    new Polygon( new int[] { 6, 8, 11, 13, 15, 13, 11, 8}, new int[] {10, 9, 12, 11, 12, 11, 12, 9}, 8),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbphoto extends GeComponent {
  // Dimension size;
  public pwr_mbphoto( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Rectangle2D.Float(6F, 6F, 8F, 8F),
    new Polygon( new int[] { 6, 14, 14, 6, 6, 6}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 14, 14, 14, 14, 6, 6}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Arc2D.Float(9F, 8F, 3F, 3F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(9F, 8F, 3F, 3F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(9F, 8F, 3F, 3F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(9F, 8F, 3F, 3F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(9.15F, 8.15F, 2.7F, 2.7F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(9F, 8F, 3F, 3F, 0F, 360F, Arc2D.PIE),
    new Polygon( new int[] { 9, 9, 14, 14, 9}, new int[] {14, 10, 10, 14, 14}, 5),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[8]);
    g.fill( shapes[9]);
    g.fill( shapes[10]);
    } else {
    GeGradient.paint( g, 9,2,-2,9F,8F,3F,3F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    GeGradient.paint( g, gradient,2,-2,9F,8F,3F,3F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[10]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,9F,8F,3F,3F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    }
    }
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[12]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbdatasheet extends GeComponent {
  // Dimension size;
  public pwr_mbdatasheet( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Rectangle2D.Float(7F, 6F, 6F, 8F),
    new Polygon( new int[] { 7, 13, 13, 7, 7, 7}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 13, 13, 13, 13, 7, 7}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Polygon( new int[] { 7, 9, 7, 7}, new int[] {10, 6, 6, 10}, 4),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[6]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbopenplc extends GeComponent {
  // Dimension size;
  public pwr_mbopenplc( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Rectangle2D.Float(8F, 6F, 4F, 8F),
    new Polygon( new int[] { 8, 12, 12, 8, 8, 8}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 12, 12, 12, 12, 8, 8}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Line2D.Float( 8F, 8F, 6F, 8F),
    new Line2D.Float( 8F, 12F, 6F, 12F),
    new Line2D.Float( 14F, 8F, 12F, 8F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[7]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[8]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbcircuitdiagram extends GeComponent {
  // Dimension size;
  public pwr_mbcircuitdiagram( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Rectangle2D.Float(5F, 7F, 10F, 7F),
    new Polygon( new int[] { 5, 15, 15, 5, 5, 5}, new int[] { 7, 7, 7, 7, 14, 14}, 6), 
    new Polygon( new int[] { 15, 15, 15, 15, 5, 5}, new int[] { 14, 7, 7, 14, 14, 14}, 6), 
    new Line2D.Float( 15F, 13F, 5F, 13F),
    new Line2D.Float( 11F, 9F, 5F, 9F),
    new Line2D.Float( 11F, 11F, 11F, 9F),
    new Rectangle2D.Float(7F, 11F, 4F, 1F),
    new Polygon( new int[] { 7, 11, 11, 7, 7, 7}, new int[] { 11, 11, 11, 11, 12, 12}, 6), 
    new Polygon( new int[] { 11, 11, 11, 11, 7, 7}, new int[] { 12, 11, 11, 12, 12, 12}, 6), 
    new Line2D.Float( 15F, 10F, 11F, 10F),
    new Line2D.Float( 15F, 8F, 11F, 8F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[7]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[8]);
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[9]);
    } else {
    GeGradient.paint( g, gradient,2,-2,7F,11F,4F,1F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[9]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[10]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[11]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[9]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[12]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[13]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbrtnavigator extends GeComponent {
  // Dimension size;
  public pwr_mbrtnavigator( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Rectangle2D.Float(6F, 6F, 4F, 3F),
    new Polygon( new int[] { 6, 10, 10, 6, 6, 6}, new int[] { 6, 6, 6, 6, 9, 9}, 6), 
    new Polygon( new int[] { 10, 10, 10, 10, 6, 6}, new int[] { 9, 6, 6, 9, 9, 9}, 6), 
    new Rectangle2D.Float(8F, 10F, 4F, 5F),
    new Polygon( new int[] { 8, 12, 12, 8, 8, 8}, new int[] { 10, 10, 10, 10, 15, 15}, 6), 
    new Polygon( new int[] { 12, 12, 12, 12, 8, 8}, new int[] { 15, 10, 10, 15, 15, 15}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    } else {
    GeGradient.paint( g, gradient,2,-2,6F,6F,4F,3F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[3]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[6]);
    } else {
    GeGradient.paint( g, gradient,2,-2,8F,10F,4F,5F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[6]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[8]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbhelpclass extends GeComponent {
  // Dimension size;
  public pwr_mbhelpclass( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Polygon( new int[] { 8, 8, 12, 12, 10, 10, 10, 12, 12, 8}, new int[] {8, 7, 7, 10, 10, 12, 10, 10, 7, 7}, 10),
    new Line2D.Float( 10F, 13.772F, 10F, 13F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbblockevents extends GeComponent {
  // Dimension size;
  public pwr_mbblockevents( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Line2D.Float( 10F, 13.772F, 10F, 13F),
    new Line2D.Float( 10F, 11F, 10F, 6F),
    new Line2D.Float( 14F, 14F, 6F, 6F),
    new Line2D.Float( 14F, 6F, 6F, 14F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbhistevent extends GeComponent {
  // Dimension size;
  public pwr_mbhistevent( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Line2D.Float( 10F, 13.772F, 10F, 13F),
    new Polygon( new int[] { 8, 6, 10, 14, 12, 14, 10, 6}, new int[] {14, 14, 6, 14, 14, 14, 6, 14}, 8),
    new Line2D.Float( 10F, 11F, 10F, 8F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    {
    int fcolor = GeColor.getDrawtype(36, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbnote extends GeComponent {
  // Dimension size;
  public pwr_mbnote( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Line2D.Float( 10F, 13.772F, 10F, 13F),
    new Line2D.Float( 10F, 11F, 10F, 6F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbhelp extends GeComponent {
  // Dimension size;
  public pwr_mbhelp( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Line2D.Float( 10F, 6.772F, 10F, 6F),
    new Line2D.Float( 10.8199F, 13F, 8.81989F, 13F),
    new Polygon( new int[] { 10, 10, 9, 10}, new int[] {13, 9, 9, 9}, 4),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbsimulate extends GeComponent {
  // Dimension size;
  public pwr_mbsimulate( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Polygon( new int[] { 12, 7, 6, 14, 13, 5, 13, 14, 6, 7}, new int[] {6, 6, 8, 12, 14, 14, 14, 12, 8, 6}, 10),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(23, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_mbup extends GeComponent {
  // Dimension size;
  public pwr_mbup( JopSession session)
  {
    super( session);
    size = new Dimension( 20, 20);
  }
  public int original_width = 20;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 16F, 16F),
    new Polygon( new int[] { 2, 18, 16, 4, 4, 2}, new int[] { 2, 2, 4, 4, 16, 18}, 6), 
    new Polygon( new int[] { 18, 18, 16, 16, 4, 2}, new int[] { 18, 2, 4, 16, 16, 18}, 6), 
    new Polygon( new int[] { 4, 10, 16, 4}, new int[] {14, 5, 14, 14}, 4),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,16F,16F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(39, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp167_ extends GeComponent {
  // Dimension size;
  public Grp167_( JopSession session)
  {
    super( session);
    size = new Dimension( 44, 18);
  }
  public int original_width = 44;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Manual",2, 13);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class Grp175_ extends GeComponent {
  // Dimension size;
  public Grp175_( JopSession session)
  {
    super( session);
    size = new Dimension( 37, 17);
  }
  public int original_width = 37;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Local",2, 13);
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
    if ( utilityAref != null)
      return utilityAref.getObjid();
    else
      return null;
  }
  public PwrtAttrRef getUtilityAttrRef() {
    return utilityAref;
  }
  public String getUtilityName() {
    return this.getClass().getName();
  }
}
