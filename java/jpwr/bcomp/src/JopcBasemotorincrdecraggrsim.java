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

public class JopcBasemotorincrdecraggrsim extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  public LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter1;
  pwr_indsquare	pwr_indsquare2;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter4;
  pwr_indsquare	pwr_indsquare5;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter7;
  pwr_indsquare	pwr_indsquare8;
  pwr_menubar2	pwr_menubar211;
  pwr_pulldownmenu2	pwr_pulldownmenu212;
  pwr_pulldownmenu2	pwr_pulldownmenu213;
  pwr_mbopenobject	pwr_mbopenobject14;
  pwr_mbopenplc	pwr_mbopenplc15;
  pwr_mbrtnavigator	pwr_mbrtnavigator16;
  pwr_mbhelpclass	pwr_mbhelpclass17;
  pwr_pulldownmenu2	pwr_pulldownmenu218;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter19;
  pwr_indsquare	pwr_indsquare20;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter22;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter23;
  pwr_smallbuttoncenter	pwr_smallbuttoncenter24;
  pwr_indsquare	pwr_indsquare25;
  public JopcBasemotorincrdecraggrsim( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcBasemotorincrdecraggrsim( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "ä§ž·D1ö·D1ö·p—uÄ?ž·¤· ¤·ˆà—¿ç‰›·D1ö·D1ö·x—u ¤·");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 312, 310);
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
    localPanel.setBackground(GeColor.getColor(218, GeColor.NO_COLOR));
    this.setSize(size);
    if ( engine.isInstance())
      setTitle( engine.getInstance());
    else
      this.setTitle("JopcBasemotorincrdecraggrsim");
    pwr_smallbuttoncenter1 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter1.setBounds(new Rectangle(242,78,37,20));
    pwr_smallbuttoncenter1.setFillColor(217);
    localPanel.add(pwr_smallbuttoncenter1, new Proportion(pwr_smallbuttoncenter1.getBounds(), dsize));
    pwr_indsquare2 = new pwr_indsquare(session);
    pwr_indsquare2.setBounds(new Rectangle(208,80,15,16));
    pwr_indsquare2.setFillColor(294);
    pwr_indsquare2.setShadow(1);
    localPanel.add(pwr_indsquare2, new Proportion(pwr_indsquare2.getBounds(), dsize));
    pwr_smallbuttoncenter4 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter4.setBounds(new Rectangle(242,176,37,20));
    pwr_smallbuttoncenter4.setFillColor(217);
    localPanel.add(pwr_smallbuttoncenter4, new Proportion(pwr_smallbuttoncenter4.getBounds(), dsize));
    pwr_indsquare5 = new pwr_indsquare(session);
    pwr_indsquare5.setBounds(new Rectangle(208,178,15,16));
    pwr_indsquare5.setFillColor(294);
    pwr_indsquare5.setShadow(1);
    localPanel.add(pwr_indsquare5, new Proportion(pwr_indsquare5.getBounds(), dsize));
    pwr_smallbuttoncenter7 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter7.setBounds(new Rectangle(242,102,37,20));
    pwr_smallbuttoncenter7.setFillColor(217);
    localPanel.add(pwr_smallbuttoncenter7, new Proportion(pwr_smallbuttoncenter7.getBounds(), dsize));
    pwr_indsquare8 = new pwr_indsquare(session);
    pwr_indsquare8.setBounds(new Rectangle(208,104,15,16));
    pwr_indsquare8.setFillColor(294);
    pwr_indsquare8.setShadow(1);
    localPanel.add(pwr_indsquare8, new Proportion(pwr_indsquare8.getBounds(), dsize));
    pwr_menubar211 = new pwr_menubar2(session);
    pwr_menubar211.setBounds(new Rectangle(0,-2,308,23));
    pwr_menubar211.setShadow(1);
    localPanel.add(pwr_menubar211, new Proportion(pwr_menubar211.getBounds(), dsize));
    pwr_pulldownmenu212 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu212.setBounds(new Rectangle(23,-2,68,23));
    pwr_pulldownmenu212.setShadow(1);
    localPanel.add(pwr_pulldownmenu212, new Proportion(pwr_pulldownmenu212.getBounds(), dsize));
    pwr_pulldownmenu213 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu213.setBounds(new Rectangle(232,-2,68,23));
    pwr_pulldownmenu213.setShadow(1);
    localPanel.add(pwr_pulldownmenu213, new Proportion(pwr_pulldownmenu213.getBounds(), dsize));
    pwr_mbopenobject14 = new pwr_mbopenobject(session);
    pwr_mbopenobject14.setBounds(new Rectangle(39,22,19,19));
    pwr_mbopenobject14.setShadow(1);
    localPanel.add(pwr_mbopenobject14, new Proportion(pwr_mbopenobject14.getBounds(), dsize));
    pwr_mbopenplc15 = new pwr_mbopenplc(session);
    pwr_mbopenplc15.setBounds(new Rectangle(58,22,19,19));
    pwr_mbopenplc15.setShadow(1);
    localPanel.add(pwr_mbopenplc15, new Proportion(pwr_mbopenplc15.getBounds(), dsize));
    pwr_mbrtnavigator16 = new pwr_mbrtnavigator(session);
    pwr_mbrtnavigator16.setBounds(new Rectangle(19,22,19,19));
    pwr_mbrtnavigator16.setShadow(1);
    localPanel.add(pwr_mbrtnavigator16, new Proportion(pwr_mbrtnavigator16.getBounds(), dsize));
    pwr_mbhelpclass17 = new pwr_mbhelpclass(session);
    pwr_mbhelpclass17.setBounds(new Rectangle(78,22,19,19));
    pwr_mbhelpclass17.setShadow(1);
    localPanel.add(pwr_mbhelpclass17, new Proportion(pwr_mbhelpclass17.getBounds(), dsize));
    pwr_pulldownmenu218 = new pwr_pulldownmenu2(session);
    pwr_pulldownmenu218.setBounds(new Rectangle(80,-2,68,23));
    pwr_pulldownmenu218.setShadow(1);
    localPanel.add(pwr_pulldownmenu218, new Proportion(pwr_pulldownmenu218.getBounds(), dsize));
    pwr_smallbuttoncenter19 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter19.setBounds(new Rectangle(242,152,37,20));
    pwr_smallbuttoncenter19.setFillColor(217);
    localPanel.add(pwr_smallbuttoncenter19, new Proportion(pwr_smallbuttoncenter19.getBounds(), dsize));
    pwr_indsquare20 = new pwr_indsquare(session);
    pwr_indsquare20.setBounds(new Rectangle(208,154,15,16));
    pwr_indsquare20.setFillColor(294);
    pwr_indsquare20.setShadow(1);
    localPanel.add(pwr_indsquare20, new Proportion(pwr_indsquare20.getBounds(), dsize));
    pwr_smallbuttoncenter22 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter22.setBounds(new Rectangle(27,203,153,20));
    pwr_smallbuttoncenter22.setFillColor(217);
    pwr_smallbuttoncenter22.setTextColor(3);
    localPanel.add(pwr_smallbuttoncenter22, new Proportion(pwr_smallbuttoncenter22.getBounds(), dsize));
    pwr_smallbuttoncenter23 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter23.setBounds(new Rectangle(27,225,153,20));
    pwr_smallbuttoncenter23.setFillColor(217);
    pwr_smallbuttoncenter23.setTextColor(3);
    localPanel.add(pwr_smallbuttoncenter23, new Proportion(pwr_smallbuttoncenter23.getBounds(), dsize));
    pwr_smallbuttoncenter24 = new pwr_smallbuttoncenter(session);
    pwr_smallbuttoncenter24.setBounds(new Rectangle(242,127,37,20));
    pwr_smallbuttoncenter24.setFillColor(217);
    localPanel.add(pwr_smallbuttoncenter24, new Proportion(pwr_smallbuttoncenter24.getBounds(), dsize));
    pwr_indsquare25 = new pwr_indsquare(session);
    pwr_indsquare25.setBounds(new Rectangle(208,129,15,16));
    pwr_indsquare25.setFillColor(294);
    pwr_indsquare25.setShadow(1);
    localPanel.add(pwr_indsquare25, new Proportion(pwr_indsquare25.getBounds(), dsize));
    pwr_smallbuttoncenter1.dd.setDynType(1);
    pwr_smallbuttoncenter1.dd.setActionType(8196);
    pwr_smallbuttoncenter1.dd.setAccess(65532);
    pwr_smallbuttoncenter1.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(pwr_smallbuttoncenter1.dd, "$object.OpToggleCircuitBreaker##Boolean")
      ,new GeDynTipText(pwr_smallbuttoncenter1.dd, "Toggle CircuitBreaker")
      });
    pwr_indsquare2.dd.setDynType(5);
    pwr_indsquare2.dd.setActionType(0);
    pwr_indsquare2.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare2.dd, "!$object.IndCircuitBreakerTripped##Boolean",176)
      });
    pwr_smallbuttoncenter4.dd.setDynType(1);
    pwr_smallbuttoncenter4.dd.setActionType(8196);
    pwr_smallbuttoncenter4.dd.setAccess(65532);
    pwr_smallbuttoncenter4.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(pwr_smallbuttoncenter4.dd, "$object.OpToggleSafetySwitch##Boolean")
      ,new GeDynTipText(pwr_smallbuttoncenter4.dd, "Toggle SafetySwitch")
      });
    pwr_indsquare5.dd.setDynType(5);
    pwr_indsquare5.dd.setActionType(0);
    pwr_indsquare5.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare5.dd, "$object.IndSafetySwitchOn##Boolean",114)
      });
    pwr_smallbuttoncenter7.dd.setDynType(1);
    pwr_smallbuttoncenter7.dd.setActionType(8196);
    pwr_smallbuttoncenter7.dd.setAccess(65532);
    pwr_smallbuttoncenter7.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(pwr_smallbuttoncenter7.dd, "$object.OpResetContactorFeedbackIncr##Boolean")
      ,new GeDynTipText(pwr_smallbuttoncenter7.dd, "Reset increase contactor feedback")
      });
    pwr_indsquare8.dd.setDynType(5);
    pwr_indsquare8.dd.setActionType(0);
    pwr_indsquare8.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare8.dd, "$object.IndContactorFeedbackIncr##Boolean",29)
      });
    pwr_menubar211.dd.setDynType(1);
    pwr_menubar211.dd.setActionType(0);
    pwr_pulldownmenu212.dd.setDynType(0);
    pwr_pulldownmenu212.dd.setActionType(524288);
    pwr_pulldownmenu212.dd.setAccess(65535);
    pwr_pulldownmenu212.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu212.dd, new String[] {
        "Print","Close",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu212,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu212.dd, "print graph/class/inst=$object")
      }),
    new GeDyn(pwr_pulldownmenu212,1,262145,65535,new GeDynElemIfc[] {
      new GeDynCloseGraph(pwr_pulldownmenu212.dd)
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu212.setAnnot1("File");
    pwr_pulldownmenu213.dd.setDynType(1);
    pwr_pulldownmenu213.dd.setActionType(524288);
    pwr_pulldownmenu213.dd.setAccess(65535);
    pwr_pulldownmenu213.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu213.dd, new String[] {
        "Help Class",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu213,1,65,65535,new GeDynElemIfc[] {
      new GeDynCommand(pwr_pulldownmenu213.dd, "call method/method=\"Help Class\"/object=$object")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu213.setAnnot1("Help");
    pwr_mbopenobject14.dd.setDynType(129);
    pwr_mbopenobject14.dd.setActionType(8256);
    pwr_mbopenobject14.dd.setAccess(65532);
    pwr_mbopenobject14.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbopenobject14.dd, "$cmd(check method/method=\"Open Object\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbopenobject14.dd, "call method/method=\"Open Object\"/object=$object")
      ,new GeDynTipText(pwr_mbopenobject14.dd, "Open Object")
      });
    pwr_mbopenplc15.dd.setDynType(129);
    pwr_mbopenplc15.dd.setActionType(8256);
    pwr_mbopenplc15.dd.setAccess(65532);
    pwr_mbopenplc15.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_mbopenplc15.dd, "$cmd(check method/method=\"Open Plc\"/object=$object)",1)
      ,new GeDynCommand(pwr_mbopenplc15.dd, "call method/method=\"Open Plc\"/object=$object")
      ,new GeDynTipText(pwr_mbopenplc15.dd, "Open Plc")
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
    pwr_pulldownmenu218.dd.setDynType(1);
    pwr_pulldownmenu218.dd.setActionType(524288);
    pwr_pulldownmenu218.dd.setAccess(65535);
    pwr_pulldownmenu218.dd.setElements(new GeDynElemIfc[] {
      new GeDynPulldownMenu(pwr_pulldownmenu218.dd, new String[] {
        "RtNavigator","Open  Object","Open Plc","Help Class",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null},
        new GeDyn[] {
            new GeDyn(pwr_pulldownmenu218,129,65,65535,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu218.dd, "$cmd(check method/method=\"RtNavigator\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu218.dd, "call method/method=\"Help\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu218,129,65,65535,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu218.dd, "$cmd(check method/method=\"Open Object\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu218.dd, "call method/method=\"Note\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu218,129,65,65535,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu218.dd, "$cmd(check method/method=\"Open Plc\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu218.dd, "call method/method=\"Trend\"/object=$object")
      }),
    new GeDyn(pwr_pulldownmenu218,129,65,65535,new GeDynElemIfc[] {
      new GeDynInvisible(pwr_pulldownmenu218.dd, "$cmd(check method/method=\"Help Class\"/object=$object)",0)
      ,new GeDynCommand(pwr_pulldownmenu218.dd, "call method/method=\"Fast\"/object=$object")
      }),
null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null
      })
      });
    pwr_pulldownmenu218.setAnnot1("Methods");
    pwr_smallbuttoncenter19.dd.setDynType(1);
    pwr_smallbuttoncenter19.dd.setActionType(8196);
    pwr_smallbuttoncenter19.dd.setAccess(65532);
    pwr_smallbuttoncenter19.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(pwr_smallbuttoncenter19.dd, "$object.OpToggleOverloadRelay##Boolean")
      ,new GeDynTipText(pwr_smallbuttoncenter19.dd, "Toggle overload relay")
      });
    pwr_indsquare20.dd.setDynType(5);
    pwr_indsquare20.dd.setActionType(0);
    pwr_indsquare20.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare20.dd, "!$object.IndOverloadRelayTripped##Boolean",176)
      });
    pwr_smallbuttoncenter22.dd.setDynType(129);
    pwr_smallbuttoncenter22.dd.setActionType(8256);
    pwr_smallbuttoncenter22.dd.setAccess(65532);
    pwr_smallbuttoncenter22.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_smallbuttoncenter22.dd, "&($object.PlcConnect).Motor.DisableTempSensor##Boolean",0)
      ,new GeDynCommand(pwr_smallbuttoncenter22.dd, "call method/method=\"Simulate\"/object=&($object.PlcConnect).Motor.TempSensor")
      ,new GeDynTipText(pwr_smallbuttoncenter22.dd, "Simulate Motor TempSensor")
      });
    pwr_smallbuttoncenter22.setAnnot1("Motor TempSensor...");
    pwr_smallbuttoncenter23.dd.setDynType(129);
    pwr_smallbuttoncenter23.dd.setActionType(8256);
    pwr_smallbuttoncenter23.dd.setAccess(65532);
    pwr_smallbuttoncenter23.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_smallbuttoncenter23.dd, "&($object.PlcConnect).Motor.DisableTempSwitch##Boolean",0)
      ,new GeDynCommand(pwr_smallbuttoncenter23.dd, "call method/method=\"Simulate\"/object=&($object.PlcConnect).Motor.TempSwitch")
      ,new GeDynTipText(pwr_smallbuttoncenter23.dd, "Simulate Motor TempSwitch")
      });
    pwr_smallbuttoncenter23.setAnnot1("Motor TempSwitch...");
    pwr_smallbuttoncenter24.dd.setDynType(1);
    pwr_smallbuttoncenter24.dd.setActionType(8196);
    pwr_smallbuttoncenter24.dd.setAccess(65532);
    pwr_smallbuttoncenter24.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(pwr_smallbuttoncenter24.dd, "$object.OpResetContactorFeedbackDecr##Boolean")
      ,new GeDynTipText(pwr_smallbuttoncenter24.dd, "Reset decrease contactor feedback")
      });
    pwr_indsquare25.dd.setDynType(5);
    pwr_indsquare25.dd.setActionType(0);
    pwr_indsquare25.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare25.dd, "$object.IndContactorFeedbackDecr##Boolean",29)
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
  public int original_width = 304;
  public int original_height = 266;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(17.6864F, 70.6272F, 273.944F, 183.731F),
    new Polygon( new int[] { 18, 292, 291, 19, 19, 18}, new int[] { 71, 71, 72, 72, 253, 254}, 6), 
    new Polygon( new int[] { 292, 292, 291, 291, 19, 18}, new int[] { 254, 71, 72, 253, 253, 254}, 6), 
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
    int fcolor = GeColor.getDrawtype(218, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[2]);
    }
    g.setColor(GeColor.getColor(3, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "CircuitBreaker tripped",27, 93);
    g.setColor(GeColor.getColor(3, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "SafetySwitch on",27, 191);
    g.setColor(GeColor.getColor(3, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "Contactor Incr feedback lost",27, 118);
    g.setColor(GeColor.getColor(3, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Simulate BaseMotorIncrDecrAggr",18, 60);
    g.setColor(GeColor.getColor(3, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "OverloadRelay tripped",27, 168);
    g.setColor(GeColor.getColor(3, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "Contactor Decr feedback lost",27, 142);
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
    new Rectangle2D.Float(2F, 2F, 11.7647F, 12.7451F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,11.7647F,12.7451F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    new Rectangle2D.Float(2F, 2F, 43.1373F, 16.6667F),
    new Polygon( new int[] { 2, 45, 42, 5, 5, 2}, new int[] { 2, 2, 5, 5, 16, 19}, 6), 
    new Polygon( new int[] { 45, 45, 42, 42, 5, 2}, new int[] { 19, 2, 5, 16, 16, 19}, 6), 
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
    GeGradient.paint( g, gradient,1,-1,2F,2F,43.1373F,16.6667F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
protected class pwr_menubar2 extends GeComponent {
  // Dimension size;
  public pwr_menubar2( JopSession session)
  {
    super( session);
    size = new Dimension( 543, 23);
  }
  public int original_width = 543;
  public int original_height = 23;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 539.216F, 19.6078F),
    new Polygon( new int[] { 2, 2, 541, 541, 2}, new int[] {22, 20, 20, 22, 22}, 5),
    new Line2D.Float( 541.216F, 21.6078F, 2F, 21.6078F),
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
    size = new Dimension( 62, 23);
    annot1Font = new Font("Helvetica", Font.PLAIN, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 62;
  public int original_height = 23;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 58.8235F, 19.6078F),
    new Polygon( new int[] { 2, 2, 61, 61, 2}, new int[] {20, 22, 22, 20, 20}, 5),
    new Line2D.Float( 60.8235F, 21.6078F, 2F, 21.6078F),
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
    size = new Dimension( 19, 19);
  }
  public int original_width = 19;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 15.6863F, 15.6863F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(5.92157F, 5.92157F, 7.84314F, 7.84314F),
    new Polygon( new int[] { 6, 14, 13, 6, 6, 6}, new int[] { 6, 6, 6, 6, 13, 14}, 6), 
    new Polygon( new int[] { 14, 14, 13, 13, 6, 6}, new int[] { 14, 6, 6, 13, 13, 14}, 6), 
    new Line2D.Float( 13.7647F, 7.88235F, 5.92157F, 7.88235F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.6863F,15.6863F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    new Rectangle2D.Float(2F, 2F, 15.6863F, 15.6863F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(7.88235F, 5.92157F, 3.92157F, 7.84314F),
    new Polygon( new int[] { 8, 12, 12, 8, 8, 8}, new int[] { 6, 6, 6, 6, 14, 14}, 6), 
    new Polygon( new int[] { 12, 12, 12, 12, 8, 8}, new int[] { 14, 6, 6, 14, 14, 14}, 6), 
    new Line2D.Float( 7.88235F, 7.88235F, 5.92157F, 7.88235F),
    new Line2D.Float( 7.88235F, 11.8039F, 5.92157F, 11.8039F),
    new Line2D.Float( 13.7647F, 7.88235F, 11.8039F, 7.88235F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.6863F,15.6863F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    new Rectangle2D.Float(2F, 2F, 15.6863F, 15.6863F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Rectangle2D.Float(5.92157F, 5.92157F, 3.92157F, 2.94118F),
    new Polygon( new int[] { 6, 10, 10, 6, 6, 6}, new int[] { 6, 6, 6, 6, 9, 9}, 6), 
    new Polygon( new int[] { 10, 10, 10, 10, 6, 6}, new int[] { 9, 6, 6, 9, 9, 9}, 6), 
    new Rectangle2D.Float(7.88235F, 9.84314F, 3.92157F, 4.90196F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.6863F,15.6863F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, gradient,2,-2,5.92157F,5.92157F,3.92157F,2.94118F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, gradient,2,-2,7.88235F,9.84314F,3.92157F,4.90196F, false,39, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    new Rectangle2D.Float(2F, 2F, 15.6863F, 15.6863F),
    new Polygon( new int[] { 2, 18, 15, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 18}, 6), 
    new Polygon( new int[] { 18, 18, 15, 15, 4, 2}, new int[] { 18, 2, 4, 15, 15, 18}, 6), 
    new Polygon( new int[] { 8, 8, 12, 12, 10, 10, 10, 12, 12, 8}, new int[] {8, 7, 7, 10, 10, 12, 10, 10, 7, 7}, 10),
    new Line2D.Float( 9.84314F, 13.5412F, 9.84314F, 12.7843F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,15.6863F,15.6863F, false,32, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
