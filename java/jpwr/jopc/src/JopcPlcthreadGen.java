package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcPlcthreadGen extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  JopBar	jopBar2;
  JopTrend	jopTrend3;
  pwr_framethin	pwr_framethin4;
  pwr_framethin	pwr_framethin5;
  pwr_framethin	pwr_framethin6;
  JopButtontoggle	jopButtontoggle8;
  pwr_valuelong	pwr_valuelong9;
  pwr_valuesmall	pwr_valuesmall10;
  pwr_valuesmall	pwr_valuesmall12;
  pwr_valuesmall	pwr_valuesmall14;
  pwr_valuesmall	pwr_valuesmall16;
  pwr_valuesmall	pwr_valuesmall18;
  pwr_valuesmall	pwr_valuesmall20;
  pwr_valuesmall	pwr_valuesmall22;
  pwr_valuesmall	pwr_valuesmall24;
  pwr_valuesmall	pwr_valuesmall26;
  pwr_valuesmall	pwr_valuesmall28;
  pwr_valueinputreliefup	pwr_valueinputreliefup29;
  pwr_valueinputreliefup	pwr_valueinputreliefup30;
  pwr_valueinputreliefup	pwr_valueinputreliefup31;
  public JopcPlcthreadGen( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcPlcthreadGen( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "‰Y");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 811, 274);
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
      this.setTitle("JopcPlcthreadGen");
    jopBar2 = new JopBar(session);
    jopBar2.setBounds(new Rectangle(260,48,16,149));
    jopBar2.setFillColor(59);
    jopBar2.setBorderColor(56);
    jopBar2.setFillColorBar(148);
    jopBar2.setBorderColorBar(142);
    jopBar2.setDrawFill(1);
    jopBar2.setDrawBorder(1);
    jopBar2.setBarBorderWidth(3);
    jopBar2.setLineWidth(1);
    jopBar2.setMinValue(0F);
    jopBar2.setMaxValue(1F);
    jopBar2.setRotate(0);
    localPanel.add(jopBar2, new Proportion(jopBar2.getBounds(), dsize));
    jopTrend3 = new JopTrend( session);
    jopTrend3.setBounds(new Rectangle(298,47,492,149));
    jopTrend3.setFillColor(59);
    jopTrend3.setBorderColor(57);
    jopTrend3.setBorderColorTrend1(142);
    jopTrend3.setBorderColorTrend2(9999);
    jopTrend3.setFillColorTrend1(28);
    jopTrend3.setFillColorTrend2(9999);
    jopTrend3.setDrawFill(1);
    jopTrend3.setDrawBorder(1);
    jopTrend3.setTrendBorderWidth(1);
    jopTrend3.setLineWidth(1);
    jopTrend3.setMinValue1(0F);
    jopTrend3.setMaxValue1(1F);
    jopTrend3.setMinValue2(0F);
    jopTrend3.setMaxValue2(0F);
    jopTrend3.setNoOfPoints(200);
    jopTrend3.setHorizontalLines(4);
    jopTrend3.setVerticalLines(9);
    jopTrend3.setRotate(0);
    localPanel.add(jopTrend3, new Proportion(jopTrend3.getBounds(), dsize));
    pwr_framethin4 = new pwr_framethin(session);
    pwr_framethin4.setBounds(new Rectangle(10,35,200,191));
    localPanel.add(pwr_framethin4, new Proportion(pwr_framethin4.getBounds(), dsize));
    pwr_framethin5 = new pwr_framethin(session);
    pwr_framethin5.setBounds(new Rectangle(288,41,509,181));
    localPanel.add(pwr_framethin5, new Proportion(pwr_framethin5.getBounds(), dsize));
    pwr_framethin6 = new pwr_framethin(session);
    pwr_framethin6.setBounds(new Rectangle(255,42,25,161));
    localPanel.add(pwr_framethin6, new Proportion(pwr_framethin6.getBounds(), dsize));
    jopButtontoggle8 = new JopButtontoggle(session);
    jopButtontoggle8.setBounds(new Rectangle(521,197,61,20));
    jopButtontoggle8.setColorTone(1);
    localPanel.add(jopButtontoggle8, new Proportion(jopButtontoggle8.getBounds(), dsize));
    pwr_valuelong9 = new pwr_valuelong(session);
    pwr_valuelong9.setBounds(new Rectangle(130,10,697,22));
    pwr_valuelong9.setFillColor(41);
    pwr_valuelong9.setBorderColor(46);
    localPanel.add(pwr_valuelong9, new Proportion(pwr_valuelong9.getBounds(), dsize));
    pwr_valuesmall10 = new pwr_valuesmall(session);
    pwr_valuesmall10.setBounds(new Rectangle(129,45,68,17));
    pwr_valuesmall10.setBorderColor(33);
    localPanel.add(pwr_valuesmall10, new Proportion(pwr_valuesmall10.getBounds(), dsize));
    pwr_valuesmall12 = new pwr_valuesmall(session);
    pwr_valuesmall12.setBounds(new Rectangle(129,62,68,17));
    pwr_valuesmall12.setBorderColor(33);
    localPanel.add(pwr_valuesmall12, new Proportion(pwr_valuesmall12.getBounds(), dsize));
    pwr_valuesmall14 = new pwr_valuesmall(session);
    pwr_valuesmall14.setBounds(new Rectangle(129,79,68,17));
    pwr_valuesmall14.setBorderColor(33);
    localPanel.add(pwr_valuesmall14, new Proportion(pwr_valuesmall14.getBounds(), dsize));
    pwr_valuesmall16 = new pwr_valuesmall(session);
    pwr_valuesmall16.setBounds(new Rectangle(129,96,68,17));
    pwr_valuesmall16.setBorderColor(33);
    localPanel.add(pwr_valuesmall16, new Proportion(pwr_valuesmall16.getBounds(), dsize));
    pwr_valuesmall18 = new pwr_valuesmall(session);
    pwr_valuesmall18.setBounds(new Rectangle(129,113,68,17));
    pwr_valuesmall18.setBorderColor(33);
    localPanel.add(pwr_valuesmall18, new Proportion(pwr_valuesmall18.getBounds(), dsize));
    pwr_valuesmall20 = new pwr_valuesmall(session);
    pwr_valuesmall20.setBounds(new Rectangle(129,130,68,17));
    pwr_valuesmall20.setBorderColor(33);
    localPanel.add(pwr_valuesmall20, new Proportion(pwr_valuesmall20.getBounds(), dsize));
    pwr_valuesmall22 = new pwr_valuesmall(session);
    pwr_valuesmall22.setBounds(new Rectangle(129,146,68,17));
    pwr_valuesmall22.setBorderColor(33);
    localPanel.add(pwr_valuesmall22, new Proportion(pwr_valuesmall22.getBounds(), dsize));
    pwr_valuesmall24 = new pwr_valuesmall(session);
    pwr_valuesmall24.setBounds(new Rectangle(129,163,68,17));
    pwr_valuesmall24.setBorderColor(33);
    localPanel.add(pwr_valuesmall24, new Proportion(pwr_valuesmall24.getBounds(), dsize));
    pwr_valuesmall26 = new pwr_valuesmall(session);
    pwr_valuesmall26.setBounds(new Rectangle(129,180,68,17));
    pwr_valuesmall26.setBorderColor(33);
    localPanel.add(pwr_valuesmall26, new Proportion(pwr_valuesmall26.getBounds(), dsize));
    pwr_valuesmall28 = new pwr_valuesmall(session);
    pwr_valuesmall28.setBounds(new Rectangle(129,197,68,17));
    pwr_valuesmall28.setBorderColor(33);
    localPanel.add(pwr_valuesmall28, new Proportion(pwr_valuesmall28.getBounds(), dsize));
    pwr_valueinputreliefup29 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup29.setBounds(new Rectangle(213,41,42,19));
    pwr_valueinputreliefup29.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup29, new Proportion(pwr_valueinputreliefup29.getBounds(), dsize));
    pwr_valueinputreliefup30 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup30.setBounds(new Rectangle(213,186,42,19));
    pwr_valueinputreliefup30.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup30, new Proportion(pwr_valueinputreliefup30.getBounds(), dsize));
    pwr_valueinputreliefup31 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup31.setBounds(new Rectangle(299,198,42,19));
    pwr_valueinputreliefup31.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup31, new Proportion(pwr_valueinputreliefup31.getBounds(), dsize));
    jopBar2.setPwrAttribute("$object.ActualScanTime##Float32");
    jopTrend3.setPwrAttribute1("$object.ActualScanTime##Float32");
    pwr_framethin4.dd.setDynType(1);
    pwr_framethin4.dd.setActionType(0);
    pwr_framethin5.dd.setDynType(1);
    pwr_framethin5.dd.setActionType(0);
    pwr_framethin6.dd.setDynType(1);
    pwr_framethin6.dd.setActionType(0);
    jopButtontoggle8.dd.setDynType(0);
    jopButtontoggle8.dd.setActionType(4);
    jopButtontoggle8.dd.setAccess(65535);
    jopButtontoggle8.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(jopButtontoggle8.dd, "$local.TrendHold##Boolean")
      });
    jopButtontoggle8.setAnnot1("   Hold");
    pwr_valuelong9.dd.setDynType(1025);
    pwr_valuelong9.dd.setActionType(0);
    pwr_valuelong9.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong9.dd, "$object.Description##String80","%s")
      });
    pwr_valuesmall10.dd.setDynType(1025);
    pwr_valuesmall10.dd.setActionType(0);
    pwr_valuesmall10.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall10.dd, "$object.Prio##Int32","%d")
      });
    pwr_valuesmall12.dd.setDynType(1025);
    pwr_valuesmall12.dd.setActionType(0);
    pwr_valuesmall12.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall12.dd, "$object.ScanTime##Float32","%5.3f")
      });
    pwr_valuesmall14.dd.setDynType(1025);
    pwr_valuesmall14.dd.setActionType(0);
    pwr_valuesmall14.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall14.dd, "$object.ActualScanTime##Float32","%5.3f")
      });
    pwr_valuesmall16.dd.setDynType(1025);
    pwr_valuesmall16.dd.setActionType(0);
    pwr_valuesmall16.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall16.dd, "$object.ScanTimeMin##Float32","%5.3f")
      });
    pwr_valuesmall18.dd.setDynType(1025);
    pwr_valuesmall18.dd.setActionType(0);
    pwr_valuesmall18.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall18.dd, "$object.ScanTimeMean##Float32","%5.3f")
      });
    pwr_valuesmall20.dd.setDynType(1025);
    pwr_valuesmall20.dd.setActionType(0);
    pwr_valuesmall20.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall20.dd, "$object.ScanTimeMax##Float32","%5.3f")
      });
    pwr_valuesmall22.dd.setDynType(1025);
    pwr_valuesmall22.dd.setActionType(0);
    pwr_valuesmall22.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall22.dd, "$object.Coverage##Float32","%7.5f")
      });
    pwr_valuesmall24.dd.setDynType(1025);
    pwr_valuesmall24.dd.setActionType(0);
    pwr_valuesmall24.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall24.dd, "$object.Count##Int32","%d")
      });
    pwr_valuesmall26.dd.setDynType(1025);
    pwr_valuesmall26.dd.setActionType(0);
    pwr_valuesmall26.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall26.dd, "$object.Count_1_8##Int32","%d")
      });
    pwr_valuesmall28.dd.setDynType(1025);
    pwr_valuesmall28.dd.setActionType(0);
    pwr_valuesmall28.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall28.dd, "$object.Count_1_4##Int32","%d")
      });
    pwr_valueinputreliefup29.dd.setDynType(1024);
    pwr_valueinputreliefup29.dd.setActionType(4096);
    pwr_valueinputreliefup29.dd.setAccess(65535);
    pwr_valueinputreliefup29.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup29.dd, "$local.MaxShow##Float32","%5.3f")
      ,new GeDynValueInput(pwr_valueinputreliefup29.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup30.dd.setDynType(1024);
    pwr_valueinputreliefup30.dd.setActionType(4096);
    pwr_valueinputreliefup30.dd.setAccess(65535);
    pwr_valueinputreliefup30.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup30.dd, "$local.MinShow##Float32","%5.3f")
      ,new GeDynValueInput(pwr_valueinputreliefup30.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup31.dd.setDynType(1024);
    pwr_valueinputreliefup31.dd.setActionType(4096);
    pwr_valueinputreliefup31.dd.setAccess(65535);
    pwr_valueinputreliefup31.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup31.dd, "$local.ScanTime##Float32","%5.0f")
      ,new GeDynValueInput(pwr_valueinputreliefup31.dd, 4,100000,null,null)
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
  public int original_width = 803;
  public int original_height = 230;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
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
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Description",20, 27);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "Prio",19, 58);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 8));
    g.drawString( "0",782, 209);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "ScanTime",19, 75);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "ActualScanTIme",19, 92);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "ScanTimeMin",19, 109);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "ScanTimeMean",19, 126);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "ScanTimeMax",19, 143);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "Coverage",19, 159);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "LoopCount",19, 176);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "Count 1/8",19, 193);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 10));
    g.drawString( "Count 1/4",19, 210);
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
    setFillColor( annotBackground);
  }
  int original_width = 49;
  int original_height = 17;
  boolean fontSet = false;
  Font annotFont = new Font("Helvetica", Font.BOLD, 8);
  int annotBackground = 41;
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
protected class JopButtontoggle extends GeComponent {
  Dimension size;
  public JopButtontoggle( JopSession session)
  {
    super( session);
    size = new Dimension( 48, 20);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 48;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 44.0379F, 16.9817F),
    new Polygon( new int[] { 2, 46, 45, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 46, 46, 45, 45, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
    new Polygon( new int[] { 2, 5, 5, 43, 46, 2, 2}, new int[] {19, 16, 5, 5, 2, 2, 19}, 7),
    new Polygon( new int[] { 2, 5, 43, 43, 46, 46, 2}, new int[] {19, 16, 16, 5, 2, 19, 19}, 7),
    new Rectangle2D.Float(2.00001F, 2F, 44.0379F, 16.9817F),
    new Polygon( new int[] { 2, 46, 45, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 46, 46, 45, 45, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
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
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    g.setColor(GeColor.getColor(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[3]);
    g.setColor(GeColor.getColor(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 6 * original_height / height * width / original_width, 14F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
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
protected class pwr_valuelong extends GeComponent {
  Dimension size;
  public pwr_valuelong( JopSession session)
  {
    super( session);
    size = new Dimension( 533, 17);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 8);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 533;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 529.353F, 13.9303F),
    new Polygon( new int[] { 2, 531, 531, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 531, 531, 531, 531, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
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
  Dimension size;
  public pwr_valuesmall( JopSession session)
  {
    super( session);
    size = new Dimension( 49, 17);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 8);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 49;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 45.7711F, 13.9303F),
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
  public int getUtilityType() {
    return JopUtility.GRAPH;
  }
  public PwrtObjid getUtilityObjid() {
    return utilityAref.getObjid();
  }
  public PwrtAttrRef getUtilityAttrRef() {
    return utilityAref;
  }
  public String getUtilityName() {
    return this.getClass().getName();
  }
}
