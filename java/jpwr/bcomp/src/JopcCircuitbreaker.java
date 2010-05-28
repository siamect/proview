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

public class JopcCircuitbreaker extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  public LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_indsquare	pwr_indsquare2;
  pwr_valuelong	pwr_valuelong4;
  pwr_mbopenobject	pwr_mbopenobject5;
  pwr_mbtrend	pwr_mbtrend6;
  pwr_mbfast	pwr_mbfast7;
  pwr_mbphoto	pwr_mbphoto8;
  pwr_mbdatasheet	pwr_mbdatasheet9;
  pwr_mbopenplc	pwr_mbopenplc10;
  pwr_mbcircuitdiagram	pwr_mbcircuitdiagram11;
  pwr_mbrtnavigator	pwr_mbrtnavigator12;
  pwr_mbhelpclass	pwr_mbhelpclass13;
  pwr_mbblockevents	pwr_mbblockevents14;
  pwr_mbhistevent	pwr_mbhistevent15;
  pwr_mbnote	pwr_mbnote16;
  pwr_mbhelp	pwr_mbhelp17;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter18;
  pwr_valuelong	pwr_valuelong19;
  pwr_valuelong	pwr_valuelong20;
  pwr_manswitch3	pwr_manswitch321;
  pwr_fuse	pwr_fuse22;
  pwr_menubar2	pwr_menubar223;
  pwr_pulldownmenu2	pwr_pulldownmenu224;
  pwr_pulldownmenu2	pwr_pulldownmenu225;
  pwr_pulldownmenu2	pwr_pulldownmenu226;
  pwr_pulldownmenu2	pwr_pulldownmenu227;
  pwr_mbup	pwr_mbup30;
  public JopcCircuitbreaker( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcCircuitbreaker( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "ä×˜·Dað·Dað·p—uÄo˜·Kž· Cž·xŠµ¿ç¹•·Dað·Dað·x—u Cž·");
    engine.setAnimationScanTime( 200);
    engine.setScanTime( 500);
    size = new Dimension( 402, 286);
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
      this.setTitle("JopcCircuitbreaker");
    pwr_indsquare2 = new pwr_indsquare(session);
    pwr_indsquare2.setBounds(new Rectangle(132,109,15,16));
    pwr_indsquare2.setFillColor(176);
    pwr_indsquare2.setShadow(1);
    localPanel.add(pwr_indsquare2, new Proportion(pwr_indsquare2.getBounds(), dsize));
    pwr_valuelong4 = new pwr_valuelong(session);
    pwr_valuelong4.setBounds(new Rectangle(14,39,366,22));
    pwr_valuelong4.setFillColor(31);
    pwr_valuelong4.setBorderColor(32);
    localPanel.add(pwr_valuelong4, new Proportion(pwr_valuelong4.getBounds(), dsize));
    pwr_mbopenobject5 = new pwr_mbopenobject(session);
    pwr_mbopenobject5.setBounds(new Rectangle(192,18,19,19));
    pwr_mbopenobject5.setShadow(1);
    localPanel.add(pwr_mbopenobject5, new Proportion(pwr_mbopenobject5.getBounds(), dsize));
    pwr_mbtrend6 = new pwr_mbtrend(session);
    pwr_mbtrend6.setBounds(new Rectangle(54,18,19,19));
    pwr_mbtrend6.setShadow(1);
    localPanel.add(pwr_mbtrend6, new Proportion(pwr_mbtrend6.getBounds(), dsize));
    pwr_mbfast7 = new pwr_mbfast(session);
    pwr_mbfast7.setBounds(new Rectangle(73,18,19,19));
    pwr_mbfast7.setShadow(1);
    localPanel.add(pwr_mbfast7, new Proportion(pwr_mbfast7.getBounds(), dsize));
    pwr_mbphoto8 = new pwr_mbphoto(session);
    pwr_mbphoto8.setBounds(new Rectangle(93,18,19,19));
    pwr_mbphoto8.setShadow(1);
    localPanel.add(pwr_mbphoto8, new Proportion(pwr_mbphoto8.getBounds(), dsize));
    pwr_mbdatasheet9 = new pwr_mbdatasheet(session);
    pwr_mbdatasheet9.setBounds(new Rectangle(113,18,19,19));
    pwr_mbdatasheet9.setShadow(1);
    localPanel.add(pwr_mbdatasheet9, new Proportion(pwr_mbdatasheet9.getBounds(), dsize));
    pwr_mbopenplc10 = new pwr_mbopenplc(session);
    pwr_mbopenplc10.setBounds(new Rectangle(211,18,19,19));
    pwr_mbopenplc10.setShadow(1);
    localPanel.add(pwr_mbopenplc10, new Proportion(pwr_mbopenplc10.getBounds(), dsize));
    pwr_mbcircuitdiagram11 = new pwr_mbcircuitdiagram(session);
    pwr_mbcircuitdiagram11.setBounds(new Rectangle(231,18,19,19));
    pwr_mbcircuitdiagram11.setShadow(1);
    localPanel.add(pwr_mbcircuitdiagram11, new Proportion(pwr_mbcircuitdiagram11.getBounds(), dsize));
    pwr_mbrtnavigator12 = new pwr_mbrtnavigator(session);
    pwr_mbrtnavigator12.setBounds(new Rectangle(172,18,19,19));
    pwr_mbrtnavigator12.setShadow(1);
    localPanel.add(pwr_mbrtnavigator12, new Proportion(pwr_mbrtnavigator12.getBounds(), dsize));
    pwr_mbhelpclass13 = new pwr_mbhelpclass(session);
    pwr_mbhelpclass13.setBounds(new Rectangle(251,18,19,19));
    pwr_mbhelpclass13.setShadow(1);
    localPanel.add(pwr_mbhelpclass13, new Proportion(pwr_mbhelpclass13.getBounds(), dsize));
    pwr_mbblockevents14 = new pwr_mbblockevents(session);
    pwr_mbblockevents14.setBounds(new Rectangle(152,18,19,19));
    pwr_mbblockevents14.setShadow(1);
    localPanel.add(pwr_mbblockevents14, new Proportion(pwr_mbblockevents14.getBounds(), dsize));
    pwr_mbhistevent15 = new pwr_mbhistevent(session);
    pwr_mbhistevent15.setBounds(new Rectangle(132,18,19,19));
    localPanel.add(pwr_mbhistevent15, new Proportion(pwr_mbhistevent15.getBounds(), dsize));
    pwr_mbnote16 = new pwr_mbnote(session);
    pwr_mbnote16.setBounds(new Rectangle(34,18,19,19));
    localPanel.add(pwr_mbnote16, new Proportion(pwr_mbnote16.getBounds(), dsize));
    pwr_mbhelp17 = new pwr_mbhelp(session);
    pwr_mbhelp17.setBounds(new Rectangle(14,18,19,19));
    localPanel.add(pwr_mbhelp17, new Proportion(pwr_mbhelp17.getBounds(), dsize));
    pwr_smallbuttoncenter18 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter18.setBounds(new Rectangle(15,207,47,20));
    pwr_smallbuttoncenter18.setBorderColor(37);
    localPanel.add(pwr_smallbuttoncenter18, new Proportion(pwr_smallbuttoncenter18.getBounds(), dsize));
    pwr_valuelong19 = new pwr_valuelong(session);
    pwr_valuelong19.setBounds(new Rectangle(14,63,366,22));
    pwr_valuelong19.setFillColor(31);
    pwr_valuelong19.setBorderColor(32);
    localPanel.add(pwr_valuelong19, new Proportion(pwr_valuelong19.getBounds(), dsize));
    pwr_valuelong20 = new pwr_valuelong(session);
    pwr_valuelong20.setBounds(new Rectangle(72,207,333,22));
    pwr_valuelong20.setFillColor(31);
    pwr_valuelong20.setBorderColor(31);
    localPanel.add(pwr_valuelong20, new Proportion(pwr_valuelong20.getBounds(), dsize));
    pwr_manswitch321 = new pwr_manswitch3(session);
    pwr_manswitch321.setBounds(new Rectangle(254,97,97,97));
    pwr_manswitch321.setFillColor(32);
    pwr_manswitch321.setShadow(1);
    localPanel.add(pwr_manswitch321, new Proportion(pwr_manswitch321.getBounds(), dsize));
    pwr_fuse22 = new pwr_fuse(session);
    pwr_fuse22.setBounds(new Rectangle(80,148,27,33));
    localPanel.add(pwr_fuse22, new Proportion(pwr_fuse22.getBounds(), dsize));
    pwr_menubar223 = new pwr_menubar2(session);
    pwr_menubar223.setBounds(new Rectangle(0,-5,406,23));
    pwr_menubar223.setShadow(1);
    localPanel.add(pwr_menubar223, new Proportion(pwr_menubar223.getBounds(), dsize));
    pwr_pulldownmenu224 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu224.setBounds(new Rectangle(9,-5,69,23));
    pwr_pulldownmenu224.setShadow(1);
    localPanel.add(pwr_pulldownmenu224, new Proportion(pwr_pulldownmenu224.getBounds(), dsize));
    pwr_pulldownmenu225 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu225.setBounds(new Rectangle(324,-5,69,23));
    pwr_pulldownmenu225.setShadow(1);
    localPanel.add(pwr_pulldownmenu225, new Proportion(pwr_pulldownmenu225.getBounds(), dsize));
    pwr_pulldownmenu226 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu226.setBounds(new Rectangle(148,-5,69,23));
    pwr_pulldownmenu226.setShadow(1);
    localPanel.add(pwr_pulldownmenu226, new Proportion(pwr_pulldownmenu226.getBounds(), dsize));
    pwr_pulldownmenu227 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu227.setBounds(new Rectangle(67,-5,69,23));
    pwr_pulldownmenu227.setShadow(1);
    localPanel.add(pwr_pulldownmenu227, new Proportion(pwr_pulldownmenu227.getBounds(), dsize));
    pwr_mbup30 = new pwr_mbup(session);
    pwr_mbup30.setBounds(new Rectangle(361,18,19,19));
    localPanel.add(pwr_mbup30, new Proportion(pwr_mbup30.getBounds(), dsize));
    pwr_indsquare2.dd.setDynType(5);
    pwr_indsquare2.dd.setActionType(2);
    pwr_indsquare2.dd.setAccess(65532);
    pwr_indsquare2.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare2.dd, "!$object.NotTripped.ActualValue##Boolean",29)
      ,new GeDynPopupMenu(pwr_indsquare2.dd, "$object.NotTripped")
      });
    pwr_valuelong4.dd.setDynType(1025);
    pwr_valuelong4.dd.setActionType(0);
    pwr_valuelong4.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong4.dd, "$object.Description##String80","%s")
      });
    pwr_mbopenobject5.dd.setDynType(129);
    pwr_mbopenobject5.dd.setActionType(8256);
    pwr_mbopenobject5.dd.setAccess(65532);
    pwr_mbopenobject5.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbopenobject5.dd, "$cmd(check method/method=\"Open Object\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbopenobject5.dd, "call method/method=\"Open Object\"/object=$object")
      ,new GeDynTipText(pwr_mbopenobject5.dd, "Open Object")
      });
    pwr_mbtrend6.dd.setDynType(129);
    pwr_mbtrend6.dd.setActionType(8256);
    pwr_mbtrend6.dd.setAccess(65532);
    pwr_mbtrend6.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbtrend6.dd, "$cmd(check method/method=\"Trend\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbtrend6.dd, "call method/method=\"Trend\"/object=$object")
      ,new GeDynTipText(pwr_mbtrend6.dd, "Trend")
      });
    pwr_mbfast7.dd.setDynType(129);
    pwr_mbfast7.dd.setActionType(8256);
    pwr_mbfast7.dd.setAccess(65532);
    pwr_mbfast7.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbfast7.dd, "$cmd(check method/method=\"Fast\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbfast7.dd, "call method/method=\"Fast\"/object=$object")
      ,new GeDynTipText(pwr_mbfast7.dd, "Fast")
      });
    pwr_mbphoto8.dd.setDynType(129);
    pwr_mbphoto8.dd.setActionType(8256);
    pwr_mbphoto8.dd.setAccess(65532);
    pwr_mbphoto8.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbphoto8.dd, "$cmd(check method/method=\"Photo\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbphoto8.dd, "call method/method=\"Photo\"/object=$object")
      ,new GeDynTipText(pwr_mbphoto8.dd, "Photo")
      });
    pwr_mbdatasheet9.dd.setDynType(129);
    pwr_mbdatasheet9.dd.setActionType(8256);
    pwr_mbdatasheet9.dd.setAccess(65532);
    pwr_mbdatasheet9.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbdatasheet9.dd, "$cmd(check method/method=\"DataSheet\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbdatasheet9.dd, "call method/method=\"DataSheet\"/object=$object")
      ,new GeDynTipText(pwr_mbdatasheet9.dd, "DataSheet")
      });
    pwr_mbopenplc10.dd.setDynType(129);
    pwr_mbopenplc10.dd.setActionType(8256);
    pwr_mbopenplc10.dd.setAccess(65532);
    pwr_mbopenplc10.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbopenplc10.dd, "$cmd(check method/method=\"Open Plc\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbopenplc10.dd, "call method/method=\"Open Plc\"/object=$object")
      ,new GeDynTipText(pwr_mbopenplc10.dd, "Open Plc")
      });
    pwr_mbcircuitdiagram11.dd.setDynType(129);
    pwr_mbcircuitdiagram11.dd.setActionType(8256);
    pwr_mbcircuitdiagram11.dd.setAccess(65535);
    pwr_mbcircuitdiagram11.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbcircuitdiagram11.dd, "$cmd(check method/method=\"CircuitDiagram\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbcircuitdiagram11.dd, "call method/method=\"CircuitDiagram\"/object=$object")
      ,new GeDynTipText(pwr_mbcircuitdiagram11.dd, "CircuitDiagram")
      });
    pwr_mbrtnavigator12.dd.setDynType(129);
    pwr_mbrtnavigator12.dd.setActionType(8256);
    pwr_mbrtnavigator12.dd.setAccess(65532);
    pwr_mbrtnavigator12.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbrtnavigator12.dd, "$cmd(check method/method=\"RtNavigator\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbrtnavigator12.dd, "call method/method=\"RtNavigator\"/object=$object")
      ,new GeDynTipText(pwr_mbrtnavigator12.dd, "RtNavigator")
      });
    pwr_mbhelpclass13.dd.setDynType(129);
    pwr_mbhelpclass13.dd.setActionType(8256);
    pwr_mbhelpclass13.dd.setAccess(65532);
    pwr_mbhelpclass13.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbhelpclass13.dd, "$cmd(check method/method=\"Help Class\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbhelpclass13.dd, "call method/method=\"Help Class\"/object=$object")
      ,new GeDynTipText(pwr_mbhelpclass13.dd, "Help Class")
      });
    pwr_mbblockevents14.dd.setDynType(129);
    pwr_mbblockevents14.dd.setActionType(8256);
    pwr_mbblockevents14.dd.setAccess(65532);
    pwr_mbblockevents14.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbblockevents14.dd, "$cmd(check method/method=\"Block Events...\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbblockevents14.dd, "call method/method=\"Block Events...\"/object=$object")
      ,new GeDynTipText(pwr_mbblockevents14.dd, "Block Events")
      });
    pwr_mbhistevent15.dd.setDynType(129);
    pwr_mbhistevent15.dd.setActionType(8256);
    pwr_mbhistevent15.dd.setAccess(65532);
    pwr_mbhistevent15.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbhistevent15.dd, "$cmd(check method/method=\"Hist Event...\"/object=$object)",0)
      ,new GeDynCommand(pwr_mbhistevent15.dd, "call method/method=\"Hist Event...\"/object=$object")
      ,new GeDynTipText(pwr_mbhistevent15.dd, "Hist Event")
      });
    pwr_mbnote16.dd.setDynType(129);
    pwr_mbnote16.dd.setActionType(8256);
    pwr_mbnote16.dd.setAccess(65532);
    pwr_mbnote16.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbnote16.dd, "$cmd(check method/method=\"Note\"/object=$object)",0)
      ,new GeDynCommand(pwr_mbnote16.dd, "call method/method=\"Note\"/object=$object")
      ,new GeDynTipText(pwr_mbnote16.dd, "Note")
      });
    pwr_mbhelp17.dd.setDynType(128);
    pwr_mbhelp17.dd.setActionType(8256);
    pwr_mbhelp17.dd.setAccess(65532);
    pwr_mbhelp17.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbhelp17.dd, "$cmd(check method/method=\"Help\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbhelp17.dd, "call method/method=\"Help\"/object=$object")
      ,new GeDynTipText(pwr_mbhelp17.dd, "Help")
      });
    pwr_smallbuttoncenter18.dd.setDynType(128);
    pwr_smallbuttoncenter18.dd.setActionType(64);
    pwr_smallbuttoncenter18.dd.setAccess(65532);
    pwr_smallbuttoncenter18.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_smallbuttoncenter18.dd, "$object.Note##String80",0)
      ,new GeDynCommand(pwr_smallbuttoncenter18.dd, "call method/method=\"Note\"/object=$object")
      });
    pwr_smallbuttoncenter18.setAnnot1("Note !");
    pwr_valuelong19.dd.setDynType(1025);
    pwr_valuelong19.dd.setActionType(0);
    pwr_valuelong19.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong19.dd, "$object.Specification##String40","%s")
      });
    pwr_valuelong20.dd.setDynType(1025);
    pwr_valuelong20.dd.setActionType(0);
    pwr_valuelong20.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong20.dd, "$object.Note##String80","%s")
      });
    pwr_manswitch321.dd.setDynType(32769);
    pwr_manswitch321.dd.setActionType(0);
    pwr_manswitch321.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigShift(pwr_manswitch321.dd, "$object.NotTripped.ActualValue##Boolean")
      });
    pwr_fuse22.dd.setDynType(32769);
    pwr_fuse22.dd.setActionType(0);
    pwr_fuse22.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigShift(pwr_fuse22.dd, "$object.NotTripped.ActualValue##Boolean")
      });
    pwr_menubar223.dd.setDynType(1);
    pwr_menubar223.dd.setActionType(0);
    pwr_pulldownmenu224.dd.setDynType(0);
    pwr_pulldownmenu224.dd.setActionType(524288);
    pwr_pulldownmenu224.dd.setAccess(65535);
    pwr_pulldownmenu224.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu224.dd, new String[] {
        "Print","Close",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu224,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu224.dd, "print graph/class/inst=$object")
      }),
    new GeDyn(pwr_pulldownmenu224,1,262145,65535,new GeDynElemIfc[] {
      new GeDynCloseGraph(pwr_pulldownmenu224.dd)
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu224.setAnnot1("File");
    pwr_pulldownmenu225.dd.setDynType(1);
    pwr_pulldownmenu225.dd.setActionType(524288);
    pwr_pulldownmenu225.dd.setAccess(65532);
    pwr_pulldownmenu225.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu225.dd, new String[] {
        "Help","Help Class",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu225,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu225.dd, "call method/method=\"Help\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu225,1,65,65532,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu225.dd, "call method/method=\"Help Class\"/object=$object")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu225.setAnnot1("Help");
    pwr_pulldownmenu226.dd.setDynType(1);
    pwr_pulldownmenu226.dd.setActionType(524288);
    pwr_pulldownmenu226.dd.setAccess(65535);
    pwr_pulldownmenu226.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu226.dd, new String[] {
        "NotTripped Di",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu226,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu226.dd, "open graph /class /inst=$object.NotTripped")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu226.setAnnot1("Signals");
    pwr_pulldownmenu227.dd.setDynType(1);
    pwr_pulldownmenu227.dd.setActionType(524288);
    pwr_pulldownmenu227.dd.setAccess(65535);
    pwr_pulldownmenu227.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu227.dd, new String[] {
        "Help","Note","Trend","Fast","Photo","DataSheet","Hist Event...","Block Events...","RtNavigator","Open  Object","Open Plc","CircuitDiagram","Help Class",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu227,129,65,65535,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Help\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Help\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Note\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Note\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Trend\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Trend\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Fast\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Fast\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Photo\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Photo\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"DataSheet\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"DataSheet\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Hist Event...\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Hist Event...\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Block Events...\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Block Events...\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"RtNavigator\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"RtNavigator\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Open Object\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Open Object\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Open Plc\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Open Plc\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"CircuitDiagram\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"CircuitDiagram\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu227,129,65,65532,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu227.dd, "$cmd(check method/method=\"Help Class\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu227.dd, "call method/method=\"Help Class\"/object=$object")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu227.setAnnot1("Methods");
    pwr_mbup30.dd.setDynType(129);
    pwr_mbup30.dd.setActionType(8256);
    pwr_mbup30.dd.setAccess(33619967);
    pwr_mbup30.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbup30.dd, "$cmd(check isattribute/object=$object)",0)
      ,new GeDynCommand(pwr_mbup30.dd, "open graph/class/parent/instance=$object")
      ,new GeDynTipText(pwr_mbup30.dd, "Open parent object graph")
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
  public int original_width = 394;
  public int original_height = 242;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(1.00742F, 17.3118F, 401.767F, 22.2593F),
    new Rectangle2D.Float(16.8854F, 95.8513F, 181.766F, 100.82F),
    new Polygon( new int[] { 17, 199, 198, 18, 18, 17}, new int[] { 96, 96, 97, 97, 196, 197}, 6), 
    new Polygon( new int[] { 199, 199, 198, 198, 18, 17}, new int[] { 197, 96, 97, 196, 196, 197}, 6), 
    new Line2D.Float( 155.44F, 160.779F, 105.822F, 160.779F),
    new Line2D.Float( 82.7647F, 160.779F, 33.1464F, 160.779F),
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
      GradientPaint gp = new GradientPaint( 1.00742F,39.571F,        GeColor.getColor(31,colorTone,colorShift,colorIntensity,0, colorInverse, fillColor, dimmed),
        1.00742F,17.3118F,GeColor.getColor(31,colorTone,colorShift,colorIntensity,-1, colorInverse, fillColor, dimmed),true);
      g.setPaint(gp);
    }
    g.fill( shapes[0]);
    {
    int fcolor = GeColor.getDrawtype(31, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[3]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 10));
    g.drawString( "Tripped",35, 123);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
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
    size = new Dimension( 531, 17);
    annot1Font = new Font("Helvetica", Font.BOLD, 8);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 531;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 527.939F, 13.8931F),
    new Polygon( new int[] { 2, 530, 529, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 530, 530, 529, 529, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,527.939F,13.8931F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
protected class pwr_indsquare extends GeComponent {
  // Dimension size;
  public pwr_indsquare( JopSession session)
  {
    super( session);
    size = new Dimension( 15, 16);
  }
  public int original_width = 15;
  public int original_height = 16;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 11.9084F, 12.9008F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,11.9084F,12.9008F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
protected class pwr_smallbuttoncenter extends GeComponent {
  // Dimension size;
  public pwr_smallbuttoncenter( JopSession session)
  {
    super( session);
    size = new Dimension( 47, 20);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 47;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 43.6641F, 16.8702F),
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
    GeGradient.paint( g, gradient,1,-1,2F,2F,43.6641F,16.8702F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
      g.drawString( annot1, 25 * original_height / height * width / original_width- (float)g.getFont().getStringBounds(annot1, frc).getWidth()/2, 15F);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(5.96947F, 5.96947F, 7.93893F, 7.93893F),
    new Polygon( new int[] { 6, 14, 14, 6, 6, 6}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 14, 14, 14, 14, 6, 6}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Line2D.Float( 13.9084F, 7.9542F, 5.96947F, 7.9542F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Line2D.Float( 13.9084F, 13.9084F, 5.96947F, 13.9084F),
    new Line2D.Float( 13.9084F, 5.96947F, 13.9084F, 13.9084F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Line2D.Float( 15.326F, 13.9084F, 5.96946F, 13.9084F),
    new Line2D.Float( 5.96947F, 5.96947F, 5.96947F, 13.9084F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(5.96947F, 5.96947F, 7.93893F, 7.93893F),
    new Polygon( new int[] { 6, 14, 14, 6, 6, 6}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 14, 14, 14, 14, 6, 6}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Arc2D.Float(8.94656F, 7.9542F, 2.9771F, 2.9771F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(8.94656F, 7.9542F, 2.9771F, 2.9771F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(8.94656F, 7.9542F, 2.9771F, 2.9771F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(8.94656F, 7.9542F, 2.9771F, 2.9771F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(9.09542F, 8.10305F, 2.67939F, 2.67939F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(8.94656F, 7.9542F, 2.9771F, 2.9771F, 0F, 360F, Arc2D.PIE),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, 9,2,-2,8.94656F,7.9542F,2.9771F,2.9771F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    GeGradient.paint( g, gradient,2,-2,8.94656F,7.9542F,2.9771F,2.9771F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[10]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,8.94656F,7.9542F,2.9771F,2.9771F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(6.96183F, 5.96947F, 5.9542F, 7.93893F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(7.9542F, 5.96947F, 3.96947F, 7.93893F),
    new Polygon( new int[] { 8, 12, 12, 8, 8, 8}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 12, 12, 12, 12, 8, 8}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Line2D.Float( 7.9542F, 7.9542F, 5.96947F, 7.9542F),
    new Line2D.Float( 7.9542F, 11.9237F, 5.96947F, 11.9237F),
    new Line2D.Float( 13.9084F, 7.9542F, 11.9237F, 7.9542F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(4.9771F, 6.96183F, 9.92366F, 6.94656F),
    new Polygon( new int[] { 5, 15, 15, 5, 5, 5}, new int[] { 7, 7, 7, 7, 14, 14}, 6), 
    new Polygon( new int[] { 15, 15, 15, 15, 5, 5}, new int[] { 14, 7, 7, 14, 14, 14}, 6), 
    new Line2D.Float( 14.9008F, 12.916F, 4.9771F, 12.916F),
    new Line2D.Float( 10.9313F, 8.94656F, 4.9771F, 8.94656F),
    new Line2D.Float( 10.9313F, 10.9313F, 10.9313F, 8.94656F),
    new Rectangle2D.Float(6.96183F, 10.9313F, 3.96947F, 0.992366F),
    new Polygon( new int[] { 7, 11, 11, 7, 7, 7}, new int[] { 11, 11, 11, 11, 12, 12}, 6), 
    new Polygon( new int[] { 11, 11, 11, 11, 7, 7}, new int[] { 12, 11, 11, 12, 12, 12}, 6), 
    new Line2D.Float( 14.9008F, 9.93893F, 10.9313F, 9.93893F),
    new Line2D.Float( 14.9008F, 7.9542F, 10.9313F, 7.9542F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, gradient,2,-2,6.96183F,10.9313F,3.96947F,0.992366F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(5.96947F, 5.96947F, 3.96947F, 2.9771F),
    new Polygon( new int[] { 6, 10, 10, 6, 6, 6}, new int[] { 6, 6, 6, 6, 9, 9}, 6), 
    new Polygon( new int[] { 10, 10, 10, 10, 6, 6}, new int[] { 9, 6, 6, 9, 9, 9}, 6), 
    new Rectangle2D.Float(7.9542F, 9.93893F, 3.96947F, 4.96183F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, gradient,2,-2,5.96947F,5.96947F,3.96947F,2.9771F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, gradient,2,-2,7.9542F,9.93893F,3.96947F,4.96183F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Polygon( new int[] { 8, 8, 12, 12, 10, 10, 10, 12, 12, 8}, new int[] {8, 7, 7, 10, 10, 12, 10, 10, 7, 7}, 10),
    new Line2D.Float( 9.93893F, 13.6821F, 9.93893F, 12.916F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Line2D.Float( 9.93893F, 13.6821F, 9.93893F, 12.916F),
    new Line2D.Float( 9.93893F, 10.9313F, 9.93893F, 5.96947F),
    new Line2D.Float( 13.9084F, 13.9084F, 5.96947F, 5.96947F),
    new Line2D.Float( 13.9084F, 5.96947F, 5.96947F, 13.9084F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Line2D.Float( 9.93893F, 13.6821F, 9.93893F, 12.916F),
    new Polygon( new int[] { 8, 6, 10, 14, 12, 14, 10, 6}, new int[] {14, 14, 6, 14, 14, 14, 6, 14}, 8),
    new Line2D.Float( 9.93893F, 10.9313F, 9.93893F, 7.9542F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Line2D.Float( 9.93893F, 13.6821F, 9.93893F, 12.916F),
    new Line2D.Float( 9.93893F, 10.9313F, 9.93893F, 5.96947F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Line2D.Float( 9.93893F, 6.73557F, 9.93893F, 5.96947F),
    new Line2D.Float( 10.7526F, 12.916F, 8.76783F, 12.916F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
protected class pwr_menubar2 extends GeComponent {
  // Dimension size;
  public pwr_menubar2( JopSession session)
  {
    super( session);
    size = new Dimension( 549, 23);
  }
  public int original_width = 549;
  public int original_height = 23;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 545.802F, 19.8473F),
    new Polygon( new int[] { 2, 2, 548, 548, 2}, new int[] {22, 20, 20, 22, 22}, 5),
    new Line2D.Float( 547.802F, 21.8473F, 2F, 21.8473F),
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
    size = new Dimension( 63, 23);
    annot1Font = new Font("Helvetica", Font.PLAIN, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 63;
  public int original_height = 23;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 59.542F, 19.8473F),
    new Polygon( new int[] { 2, 2, 62, 62, 2}, new int[] {20, 22, 22, 20, 20}, 5),
    new Line2D.Float( 61.542F, 21.8473F, 2F, 21.8473F),
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
protected class pwr_mbup extends GeComponent {
  // Dimension size;
  public pwr_mbup( JopSession session)
  {
    super( session);
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.8779F,15.8779F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
protected class pwr_manswitch3 extends GeComponent {
  // Dimension size;
  public pwr_manswitch3( JopSession session)
  {
    super( session);
    size = new Dimension( 63, 63);
    pages = 2;
  }
  public int original_width = 63;
  public int original_height = 63;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 59.542F, 59.542F),
    new Polygon( new int[] { 2, 62, 59, 5, 5, 2}, new int[] { 2, 2, 5, 5, 59, 62}, 6), 
    new Polygon( new int[] { 62, 62, 59, 59, 5, 2}, new int[] { 62, 2, 5, 59, 59, 62}, 6), 
    new Rectangle2D.Float(9.93893F, 23.8321F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 10, 26, 23, 12, 12, 10}, new int[] { 24, 24, 26, 26, 37, 40}, 6), 
    new Polygon( new int[] { 26, 26, 23, 23, 12, 10}, new int[] { 40, 24, 26, 37, 37, 40}, 6), 
    new Rectangle2D.Float(37.7252F, 23.8321F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 38, 54, 51, 40, 40, 38}, new int[] { 24, 24, 26, 26, 37, 40}, 6), 
    new Polygon( new int[] { 54, 54, 51, 51, 40, 38}, new int[] { 40, 24, 26, 37, 37, 40}, 6), 
    new Rectangle2D.Float(37.7252F, 23.8321F, 15.8779F, 5.9542F),
    new Rectangle2D.Float(37.7252F, 29.7863F, 3.96947F, 9.92366F),
    new Polygon( new int[] { 54, 56, 36, 36, 38, 38, 54}, new int[] {24, 22, 22, 42, 40, 24, 24}, 7),
    new Polygon( new int[] { 38, 36, 56, 56, 54, 54, 38}, new int[] {40, 42, 42, 22, 24, 40, 40}, 7),
    new Polygon( new int[] { 10, 8, 28, 28, 26, 26, 10}, new int[] {40, 42, 42, 22, 24, 40, 40}, 7),
    new Polygon( new int[] { 26, 28, 8, 8, 10, 10, 26}, new int[] {24, 22, 22, 42, 40, 24, 24}, 7),
    new Polygon( new int[] { 26, 30, 30, 16, 10, 26, 26}, new int[] {24, 28, 46, 46, 40, 40, 24}, 7),
    new Rectangle2D.Float(9.93893F, 23.8321F, 15.8779F, 15.8779F),
    new Rectangle2D.Float(37.7252F, 23.8321F, 15.8779F, 15.8779F),
  };
  int original_width_p2 = 63;
  int original_height_p2 = 63;
  Shape[] shapes_p2 = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 59.542F, 59.542F),
    new Polygon( new int[] { 2, 62, 59, 5, 5, 2}, new int[] { 2, 2, 5, 5, 59, 62}, 6), 
    new Polygon( new int[] { 62, 62, 59, 59, 5, 2}, new int[] { 62, 2, 5, 59, 59, 62}, 6), 
    new Rectangle2D.Float(9.93893F, 23.8321F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 10, 26, 23, 12, 12, 10}, new int[] { 24, 24, 26, 26, 37, 40}, 6), 
    new Polygon( new int[] { 26, 26, 23, 23, 12, 10}, new int[] { 40, 24, 26, 37, 37, 40}, 6), 
    new Rectangle2D.Float(37.7252F, 23.8321F, 15.8779F, 15.8779F),
    new Polygon( new int[] { 38, 54, 51, 40, 40, 38}, new int[] { 24, 24, 26, 26, 37, 40}, 6), 
    new Polygon( new int[] { 54, 54, 51, 51, 40, 38}, new int[] { 40, 24, 26, 37, 37, 40}, 6), 
    new Rectangle2D.Float(9.93893F, 23.8321F, 15.8779F, 5.9542F),
    new Rectangle2D.Float(9.93893F, 29.7863F, 3.96947F, 9.92366F),
    new Polygon( new int[] { 54, 56, 36, 36, 38, 38, 54}, new int[] {24, 22, 22, 42, 40, 24, 24}, 7),
    new Polygon( new int[] { 38, 36, 56, 56, 54, 54, 38}, new int[] {40, 42, 42, 22, 24, 40, 40}, 7),
    new Polygon( new int[] { 10, 8, 28, 28, 26, 26, 10}, new int[] {40, 42, 42, 22, 24, 40, 40}, 7),
    new Polygon( new int[] { 26, 28, 8, 8, 10, 10, 26}, new int[] {24, 22, 22, 42, 40, 24, 24}, 7),
    new Polygon( new int[] { 54, 58, 58, 44, 38, 54, 54}, new int[] {24, 28, 46, 46, 40, 40, 24}, 7),
    new Rectangle2D.Float(9.93893F, 23.8321F, 15.8779F, 15.8779F),
    new Rectangle2D.Float(37.7252F, 23.8321F, 15.8779F, 15.8779F),
  };
  public void paintComponent(Graphics g1) {
    switch ( page) {
      case 2 :
        paintComponent_p2(g1);
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
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,59.542F,59.542F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    {
    int fcolor = 77;
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    } else {
    GeGradient.paint( g, gradient,2,-2,9.93893F,23.8321F,15.8779F,15.8779F, true,77, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[3]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    }
    }
    {
    int fcolor = 167;
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[6]);
    } else {
    GeGradient.paint( g, gradient,2,-2,37.7252F,23.8321F,15.8779F,15.8779F, true,167, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[6]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[8]);
    }
    }
    {
    int fcolor = 169;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[9]);
    }
    {
    int fcolor = 169;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[10]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[12]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[13]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[14]);
    }
    {
    int fcolor = 36;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[15]);
    }
    {
    int fcolor = GeColor.getDrawtype(66, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[16]);
    }
    {
    int fcolor = GeColor.getDrawtype(168, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[17]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "1",15, 18);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "0",43, 18);
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
    int fcolor = GeColor.getDrawtype(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,59.542F,59.542F, false,33, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[2]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[0]);
    }
    {
    int fcolor = 77;
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[3]);
    } else {
    GeGradient.paint( g, gradient,2,-2,9.93893F,23.8321F,15.8779F,15.8779F, true,77, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[3]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[5]);
    }
    }
    {
    int fcolor = 167;
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[6]);
    } else {
    GeGradient.paint( g, gradient,2,-2,37.7252F,23.8321F,15.8779F,15.8779F, true,167, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes_p2[6]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes_p2[7]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes_p2[8]);
    }
    }
    {
    int fcolor = 79;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[9]);
    }
    {
    int fcolor = 79;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[10]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[11]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[12]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[13]);
    }
    {
    int fcolor;
    if ( shadow != 0)
      fcolor = GeColor.getDrawtype(33, colorTone,
                 colorShift, colorIntensity,-2, colorInverse, fillColor, dimmed);
    else
      fcolor = GeColor.getDrawtype(33, colorTone,
	          colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[14]);
    }
    {
    int fcolor = 36;
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[15]);
    }
    {
    int fcolor = GeColor.getDrawtype(77, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[16]);
    }
    {
    int fcolor = GeColor.getDrawtype(168, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[17]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "1",15, 18);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "0",43, 18);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_fuse extends GeComponent {
  // Dimension size;
  public pwr_fuse( JopSession session)
  {
    super( session);
    size = new Dimension( 27, 33);
    pages = 2;
  }
  public int original_width = 27;
  public int original_height = 33;
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 35F, 140F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 215F, 140F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, -5F, 40F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 175F, 40F, Arc2D.OPEN),
    new Arc2D.Float(22.0458F, 10.1374F, 3.57252F, 3.57252F, 0F, 360F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 0F, 360F, Arc2D.OPEN),
    new Line2D.Float( 21.8252F, 12.916F, 6.35575F, 21.8473F),
    new Line2D.Float( 15.7476F, 30.1756F, 9.79341F, 19.8626F),
    new Polygon( new int[] { 18, 20, 17, 19, 22, 24, 22, 19, 17, 20}, new int[] {15, 18, 20, 24, 22, 25, 22, 24, 20, 18}, 10),
    new Polygon( new int[] { 16, 13, 16, 16}, new int[] {30, 29, 27, 30}, 4),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 35F, 140F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 215F, 140F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, -5F, 40F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 175F, 40F, Arc2D.OPEN),
    new Arc2D.Float(2.19847F, 10.1374F, 3.57252F, 3.57252F, 0F, 360F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 0F, 360F, Arc2D.OPEN),
  };
  int original_width_p2 = 27;
  int original_height_p2 = 33;
  Shape[] shapes_p2 = new Shape[] { 
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 35F, 140F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 215F, 140F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, -5F, 40F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 175F, 40F, Arc2D.OPEN),
    new Arc2D.Float(22.0458F, 10.1374F, 3.57252F, 3.57252F, 0F, 360F, Arc2D.OPEN),
    new Arc2D.Float(21.8473F, 9.93893F, 3.96947F, 3.96947F, 0F, 360F, Arc2D.OPEN),
    new Line2D.Float( 21.8473F, 11.9237F, 5.27742F, 11.9237F),
    new Line2D.Float( 7.9542F, 23.8321F, 7.9542F, 11.9237F),
    new Polygon( new int[] { 8, 6, 10, 8}, new int[] {24, 22, 22, 24}, 4),
    new Polygon( new int[] { 18, 18, 14, 14, 18, 18, 18, 14, 14, 18}, new int[] {12, 16, 16, 20, 20, 24, 20, 20, 16, 16}, 10),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 35F, 140F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 215F, 140F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, -5F, 40F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 175F, 40F, Arc2D.OPEN),
    new Arc2D.Float(2.19847F, 10.1374F, 3.57252F, 3.57252F, 0F, 360F, Arc2D.OPEN),
    new Arc2D.Float(2F, 9.93893F, 3.96947F, 3.96947F, 0F, 360F, Arc2D.OPEN),
  };
  public void paintComponent(Graphics g1) {
    switch ( page) {
      case 2 :
        paintComponent_p2(g1);
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
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[7]);
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[8]);
    }
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[9]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[9]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[15]);
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
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[5]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[6]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[7]);
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes_p2[8]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[8]);
    }
    {
    int fcolor = GeColor.getDrawtype(2, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[9]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes_p2[15]);
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
