package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcAiGen extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_valuemedium	pwr_valuemedium2;
  pwr_valuemedium	pwr_valuemedium4;
  JopBar	jopBar5;
  JopTrend	jopTrend6;
  pwr_framethin	pwr_framethin8;
  pwr_framethin	pwr_framethin9;
  pwr_framethin	pwr_framethin10;
  JopButtontoggle	jopButtontoggle12;
  pwr_valuelong	pwr_valuelong13;
  pwr_sliderbackground1	pwr_sliderbackground114;
  pwr_slider1	pwr_slider115;
  JopButtontoggle	jopButtontoggle16;
  pwr_buttoncommand	pwr_buttoncommand17;
  pwr_valueinputreliefup	pwr_valueinputreliefup18;
  pwr_valueinputreliefup	pwr_valueinputreliefup19;
  pwr_valueinputreliefup	pwr_valueinputreliefup20;
  pwr_valueinputmedium	pwr_valueinputmedium21;
  public JopcAiGen( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcAiGen( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "Œ-i@€l@œá@pïÿ¿8ïÿ¿ß}");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 829, 257);
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
      this.setTitle("JopcAiGen");
    pwr_valuemedium2 = new pwr_valuemedium(session);
    pwr_valuemedium2.setBounds(new Rectangle(121,103,97,24));
    pwr_valuemedium2.setFillColor(51);
    pwr_valuemedium2.setBorderColor(56);
    localPanel.add(pwr_valuemedium2, new Proportion(pwr_valuemedium2.getBounds(), dsize));
    pwr_valuemedium4 = new pwr_valuemedium(session);
    pwr_valuemedium4.setBounds(new Rectangle(121,130,97,24));
    pwr_valuemedium4.setFillColor(51);
    pwr_valuemedium4.setBorderColor(56);
    localPanel.add(pwr_valuemedium4, new Proportion(pwr_valuemedium4.getBounds(), dsize));
    jopBar5 = new JopBar(session);
    jopBar5.setBounds(new Rectangle(250,38,17,149));
    jopBar5.setFillColor(59);
    jopBar5.setBorderColor(56);
    jopBar5.setFillColorBar(148);
    jopBar5.setBorderColorBar(142);
    jopBar5.setDrawFill(1);
    jopBar5.setDrawBorder(1);
    jopBar5.setBarBorderWidth(3);
    jopBar5.setLineWidth(1);
    jopBar5.setMinValue(0F);
    jopBar5.setMaxValue(100F);
    jopBar5.setRotate(0);
    localPanel.add(jopBar5, new Proportion(jopBar5.getBounds(), dsize));
    jopTrend6 = new JopTrend( session);
    jopTrend6.setBounds(new Rectangle(288,37,495,150));
    jopTrend6.setFillColor(59);
    jopTrend6.setBorderColor(57);
    jopTrend6.setBorderColorTrend1(142);
    jopTrend6.setBorderColorTrend2(9999);
    jopTrend6.setFillColorTrend1(28);
    jopTrend6.setFillColorTrend2(9999);
    jopTrend6.setDrawFill(1);
    jopTrend6.setDrawBorder(1);
    jopTrend6.setTrendBorderWidth(1);
    jopTrend6.setLineWidth(1);
    jopTrend6.setMinValue1(0F);
    jopTrend6.setMaxValue1(100F);
    jopTrend6.setMinValue2(0F);
    jopTrend6.setMaxValue2(100F);
    jopTrend6.setNoOfPoints(200);
    jopTrend6.setHorizontalLines(4);
    jopTrend6.setVerticalLines(9);
    jopTrend6.setRotate(0);
    localPanel.add(jopTrend6, new Proportion(jopTrend6.getBounds(), dsize));
    pwr_framethin8 = new pwr_framethin(session);
    pwr_framethin8.setBounds(new Rectangle(0,62,230,101));
    localPanel.add(pwr_framethin8, new Proportion(pwr_framethin8.getBounds(), dsize));
    pwr_framethin9 = new pwr_framethin(session);
    pwr_framethin9.setBounds(new Rectangle(279,31,511,182));
    localPanel.add(pwr_framethin9, new Proportion(pwr_framethin9.getBounds(), dsize));
    pwr_framethin10 = new pwr_framethin(session);
    pwr_framethin10.setBounds(new Rectangle(245,32,25,162));
    localPanel.add(pwr_framethin10, new Proportion(pwr_framethin10.getBounds(), dsize));
    jopButtontoggle12 = new JopButtontoggle(session);
    jopButtontoggle12.setBounds(new Rectangle(512,188,61,21));
    jopButtontoggle12.setColorTone(1);
    localPanel.add(jopButtontoggle12, new Proportion(jopButtontoggle12.getBounds(), dsize));
    pwr_valuelong13 = new pwr_valuelong(session);
    pwr_valuelong13.setBounds(new Rectangle(119,0,700,23));
    pwr_valuelong13.setFillColor(41);
    pwr_valuelong13.setBorderColor(46);
    localPanel.add(pwr_valuelong13, new Proportion(pwr_valuelong13.getBounds(), dsize));
    pwr_sliderbackground114 = new pwr_sliderbackground1(session);
    pwr_sliderbackground114.setBounds(new Rectangle(797,26,20,173));
    pwr_sliderbackground114.setColorBrightness(2);
    localPanel.add(pwr_sliderbackground114, new Proportion(pwr_sliderbackground114.getBounds(), dsize));
    pwr_slider115 = new pwr_slider1(session);
    pwr_slider115.setBounds(new Rectangle(799,31,22,19));
    pwr_slider115.setColorTone(1);
    localPanel.add(pwr_slider115, new Proportion(pwr_slider115.getBounds(), dsize));
    jopButtontoggle16 = new JopButtontoggle(session);
    jopButtontoggle16.setBounds(new Rectangle(687,188,65,21));
    jopButtontoggle16.setColorTone(1);
    localPanel.add(jopButtontoggle16, new Proportion(jopButtontoggle16.getBounds(), dsize));
    pwr_buttoncommand17 = new pwr_buttoncommand(session);
    pwr_buttoncommand17.setBounds(new Rectangle(1,178,89,29));
    pwr_buttoncommand17.setColorTone(1);
    localPanel.add(pwr_buttoncommand17, new Proportion(pwr_buttoncommand17.getBounds(), dsize));
    pwr_valueinputreliefup18 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup18.setBounds(new Rectangle(199,30,42,19));
    pwr_valueinputreliefup18.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup18, new Proportion(pwr_valueinputreliefup18.getBounds(), dsize));
    pwr_valueinputreliefup19 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup19.setBounds(new Rectangle(199,177,42,19));
    pwr_valueinputreliefup19.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup19, new Proportion(pwr_valueinputreliefup19.getBounds(), dsize));
    pwr_valueinputreliefup20 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup20.setBounds(new Rectangle(288,189,42,19));
    pwr_valueinputreliefup20.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup20, new Proportion(pwr_valueinputreliefup20.getBounds(), dsize));
    pwr_valueinputmedium21 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium21.setBounds(new Rectangle(121,76,97,24));
    pwr_valueinputmedium21.setBorderColor(47);
    localPanel.add(pwr_valueinputmedium21, new Proportion(pwr_valueinputmedium21.getBounds(), dsize));
    pwr_valuemedium2.dd.setDynType(1025);
    pwr_valuemedium2.dd.setActionType(0);
    pwr_valuemedium2.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium2.dd, "$object.InitialValue##Float32","%10.3f")
      });
    pwr_valuemedium4.dd.setDynType(1025);
    pwr_valuemedium4.dd.setActionType(0);
    pwr_valuemedium4.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium4.dd, "$object.Unit##String80","%s")
      });
    jopBar5.setPwrAttribute("$object.ActualValue##Float32");
    jopTrend6.setPwrAttribute1("$object.ActualValue##Float32");
    pwr_framethin8.dd.setDynType(1);
    pwr_framethin8.dd.setActionType(0);
    pwr_framethin9.dd.setDynType(1);
    pwr_framethin9.dd.setActionType(0);
    pwr_framethin10.dd.setDynType(1);
    pwr_framethin10.dd.setActionType(0);
    jopButtontoggle12.dd.setDynType(0);
    jopButtontoggle12.dd.setActionType(4);
    jopButtontoggle12.dd.setAccess(65535);
    jopButtontoggle12.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(jopButtontoggle12.dd, "$local.TrendHold##Boolean")
      });
    jopButtontoggle12.setAnnot1("   Hold");
    pwr_valuelong13.dd.setDynType(1025);
    pwr_valuelong13.dd.setActionType(0);
    pwr_valuelong13.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong13.dd, "$object.Description##String80","%s")
      });
    pwr_sliderbackground114.dd.setDynType(1);
    pwr_sliderbackground114.dd.setActionType(0);
    pwr_slider115.dd.setDynType(1);
    pwr_slider115.dd.setActionType(2048);
    pwr_slider115.dd.setAccess(6);
    pwr_slider115.dd.setElements(new GeDynElemIfc[] {
      new GeDynSlider(pwr_slider115.dd, "$object.ActualValue##Float32",0,100,4,31.7176,178.318)
      });
    jopButtontoggle16.dd.setDynType(0);
    jopButtontoggle16.dd.setActionType(4);
    jopButtontoggle16.dd.setAccess(6);
    jopButtontoggle16.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(jopButtontoggle16.dd, "$local.SliderDisable##Boolean")
      });
    jopButtontoggle16.setAnnot1(" Slider");
    pwr_buttoncommand17.dd.setDynType(1);
    pwr_buttoncommand17.dd.setActionType(64);
    pwr_buttoncommand17.dd.setAccess(65535);
    pwr_buttoncommand17.dd.setElements(new GeDynElemIfc[] {
      new GeDynCommand(pwr_buttoncommand17.dd, "open graph/classgraph/inst=&$object.SigChanCon")
      });
    pwr_buttoncommand17.setAnnot1("Channel");
    pwr_valueinputreliefup18.dd.setDynType(1024);
    pwr_valueinputreliefup18.dd.setActionType(4096);
    pwr_valueinputreliefup18.dd.setAccess(65535);
    pwr_valueinputreliefup18.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup18.dd, "$object.PresMaxLimit##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup18.dd, 0,0)
      });
    pwr_valueinputreliefup19.dd.setDynType(1024);
    pwr_valueinputreliefup19.dd.setActionType(4096);
    pwr_valueinputreliefup19.dd.setAccess(65535);
    pwr_valueinputreliefup19.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup19.dd, "$object.PresMinLimit##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup19.dd, 0,0)
      });
    pwr_valueinputreliefup20.dd.setDynType(1024);
    pwr_valueinputreliefup20.dd.setActionType(4096);
    pwr_valueinputreliefup20.dd.setAccess(65535);
    pwr_valueinputreliefup20.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup20.dd, "$local.ScanTime##Float32","%5.0f")
      ,new GeDynValueInput(pwr_valueinputreliefup20.dd, 4,100000)
      });
    pwr_valueinputmedium21.dd.setDynType(1024);
    pwr_valueinputmedium21.dd.setActionType(4096);
    pwr_valueinputmedium21.dd.setAccess(6);
    pwr_valueinputmedium21.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium21.dd, "$object.ActualValue##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium21.dd, 0,0)
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
  public int original_width = 821;
  public int original_height = 213;
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Description",9, 16);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "ActualValue",9, 93);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "InitialValue",9, 119);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Unit",9, 146);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.PLAIN, 10));
    g.drawString( "0",774, 199);
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
  int original_height = 18;
  boolean fontSet = false;
  Font annotFont = new Font("Helvetica", Font.BOLD, 10);
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
protected class pwr_valueinputmedium extends GeTextField {
  public pwr_valueinputmedium( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( annotBackground);
  }
  int original_width = 64;
  int original_height = 24;
  boolean fontSet = false;
  Font annotFont = new Font("Helvetica", Font.BOLD, 14);
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
    size = new Dimension( 48, 21);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 12);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 48;
  public int original_height = 21;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 44.2581F, 17.0667F),
    new Polygon( new int[] { 2, 46, 45, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 46, 46, 45, 45, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
    new Polygon( new int[] { 2, 5, 5, 44, 46, 2, 2}, new int[] {19, 16, 5, 5, 2, 2, 19}, 7),
    new Polygon( new int[] { 2, 5, 44, 44, 46, 46, 2}, new int[] {19, 16, 16, 5, 2, 19, 19}, 7),
    new Rectangle2D.Float(2.00001F, 2F, 44.2581F, 17.0667F),
    new Polygon( new int[] { 2, 46, 45, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 46, 46, 45, 45, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( dd.invisible) {
      if ( !dd.invisibleOld) {
        dd.invisibleOld = dd.invisible;
        repaint();
      }
      else
        dd.invisibleOld = dd.invisible;
      return;
    }
    dd.invisibleOld = dd.invisible;
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[3]);
    g.setColor(GeColor.getColor(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[5]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 7 * original_height / height * width / original_width, 16F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_buttoncommand extends GeComponent {
  Dimension size;
  public pwr_buttoncommand( JopSession session)
  {
    super( session);
    size = new Dimension( 101, 36);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 101;
  public int original_height = 36;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 98F, 32F),
    new Polygon( new int[] { 2, 100, 98, 4, 4, 2}, new int[] { 2, 2, 4, 4, 32, 34}, 6), 
    new Polygon( new int[] { 100, 100, 98, 98, 4, 2}, new int[] { 34, 2, 4, 32, 32, 34}, 6), 
    new Polygon( new int[] { 2, 8, 8, 94, 100, 2, 2}, new int[] {34, 28, 8, 8, 2, 2, 34}, 7),
    new Polygon( new int[] { 2, 6, 8, 8}, new int[] { 34, 29, 27,28}, 4),
    new Polygon( new int[] { 8, 8, 8, 8}, new int[] { 28, 27, 6,8}, 4),
    new Polygon( new int[] { 8, 8, 93, 94}, new int[] { 8, 6, 6,8}, 4),
    new Polygon( new int[] { 94, 93, 95, 100}, new int[] { 8, 6, 4,2}, 4),
    new Polygon( new int[] { 100, 95, 6, 2}, new int[] { 2, 4, 4,2}, 4),
    new Polygon( new int[] { 2, 6, 6, 2}, new int[] { 2, 4, 29,34}, 4),
    new Polygon( new int[] { 2, 8, 94, 94, 100, 100, 2}, new int[] {34, 28, 28, 8, 2, 34, 34}, 7),
    new Polygon( new int[] { 2, 9, 11, 8}, new int[] { 34, 32, 30,28}, 4),
    new Polygon( new int[] { 8, 11, 96, 94}, new int[] { 28, 30, 30,28}, 4),
    new Polygon( new int[] { 94, 96, 96, 94}, new int[] { 28, 30, 9,8}, 4),
    new Polygon( new int[] { 94, 96, 98, 100}, new int[] { 8, 9, 7,2}, 4),
    new Polygon( new int[] { 100, 98, 98, 100}, new int[] { 2, 7, 32,34}, 4),
    new Polygon( new int[] { 100, 98, 9, 2}, new int[] { 34, 32, 32,34}, 4),
    new Rectangle2D.Float(2F, 2F, 98F, 32F),
    new Polygon( new int[] { 2, 100, 98, 4, 4, 2}, new int[] { 2, 2, 4, 4, 32, 34}, 6), 
    new Polygon( new int[] { 100, 100, 98, 98, 4, 2}, new int[] { 34, 2, 4, 32, 32, 34}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( dd.invisible) {
      if ( !dd.invisibleOld) {
        dd.invisibleOld = dd.invisible;
        repaint();
      }
      else
        dd.invisibleOld = dd.invisible;
      return;
    }
    dd.invisibleOld = dd.invisible;
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[3]);
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[8]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[9]);
    }
    }
    g.setColor(GeColor.getColor(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[10]);
    {
    int fcolor = GeColor.getDrawtype(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[11]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[13]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[14]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[16]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[18]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[19]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[17]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 13 * original_height / height * width / original_width, 22F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuemedium extends GeComponent {
  Dimension size;
  public pwr_valuemedium( JopSession session)
  {
    super( session);
    size = new Dimension( 64, 24);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 64;
  public int original_height = 24;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 60F, 20F),
    new Polygon( new int[] { 2, 62, 61, 3, 3, 2}, new int[] { 2, 2, 3, 3, 21, 22}, 6), 
    new Polygon( new int[] { 62, 62, 61, 61, 3, 2}, new int[] { 22, 2, 3, 21, 21, 22}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( dd.invisible) {
      if ( !dd.invisibleOld) {
        dd.invisibleOld = dd.invisible;
        repaint();
      }
      else
        dd.invisibleOld = dd.invisible;
      return;
    }
    dd.invisibleOld = dd.invisible;
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 8 * original_height / height * width / original_width, 18F);
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
    size = new Dimension( 536, 18);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 536;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 532F, 14F),
    new Polygon( new int[] { 2, 534, 533, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 534, 534, 533, 533, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( dd.invisible) {
      if ( !dd.invisibleOld) {
        dd.invisibleOld = dd.invisible;
        repaint();
      }
      else
        dd.invisibleOld = dd.invisible;
      return;
    }
    dd.invisibleOld = dd.invisible;
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
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
protected class pwr_sliderbackground1 extends GeComponent {
  Dimension size;
  public pwr_sliderbackground1( JopSession session)
  {
    super( session);
    size = new Dimension( 44, 224);
  }
  public int original_width = 44;
  public int original_height = 224;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 2, 42, 22, 2}, new int[] {2, 2, 22, 2}, 4),
    new Polygon( new int[] { 2, 6, 41, 42}, new int[] { 2, 3, 3,2}, 4),
    new Polygon( new int[] { 42, 41, 24, 22}, new int[] { 2, 3, 21,22}, 4),
    new Polygon( new int[] { 22, 24, 6, 2}, new int[] { 22, 21, 3,2}, 4),
    new Polygon( new int[] { 42, 22, 22, 42, 42}, new int[] {2, 22, 202, 222, 2}, 5),
    new Polygon( new int[] { 42, 41, 25, 22}, new int[] { 2, 5, 22,22}, 4),
    new Polygon( new int[] { 22, 25, 25, 22}, new int[] { 22, 22, 202,202}, 4),
    new Polygon( new int[] { 22, 25, 41, 42}, new int[] { 202, 202, 219,222}, 4),
    new Polygon( new int[] { 42, 41, 41, 42}, new int[] { 222, 219, 5,2}, 4),
    new Polygon( new int[] { 2, 2, 22, 22, 2}, new int[] {2, 222, 202, 22, 2}, 5),
    new Polygon( new int[] { 2, 5, 5, 2}, new int[] { 2, 4, 220,222}, 4),
    new Polygon( new int[] { 2, 5, 23, 22}, new int[] { 222, 220, 202,202}, 4),
    new Polygon( new int[] { 22, 23, 23, 22}, new int[] { 202, 202, 22,22}, 4),
    new Polygon( new int[] { 22, 23, 5, 2}, new int[] { 22, 22, 4,2}, 4),
    new Polygon( new int[] { 2, 22, 42, 2}, new int[] {222, 202, 222, 222}, 4),
    new Polygon( new int[] { 2, 6, 24, 22}, new int[] { 222, 221, 203,202}, 4),
    new Polygon( new int[] { 22, 24, 41, 42}, new int[] { 202, 203, 221,222}, 4),
    new Polygon( new int[] { 42, 41, 6, 2}, new int[] { 222, 221, 221,222}, 4),
    new Rectangle2D.Float(18F, 18F, 8F, 188F),
    new Polygon( new int[] { 18, 26, 26, 18, 18, 18}, new int[] { 18, 18, 18, 18, 206, 206}, 6), 
    new Polygon( new int[] { 26, 26, 26, 26, 18, 18}, new int[] { 206, 18, 18, 206, 206, 206}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( dd.invisible) {
      if ( !dd.invisibleOld) {
        dd.invisibleOld = dd.invisible;
        repaint();
      }
      else
        dd.invisibleOld = dd.invisible;
      return;
    }
    dd.invisibleOld = dd.invisible;
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
    g.setColor(GeColor.getColor(43, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[0]);
    {
    int fcolor = GeColor.getDrawtype(43, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[3]);
    }
    }
    g.setColor(GeColor.getColor(46, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(46, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[5]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[8]);
    }
    }
    g.setColor(GeColor.getColor(42, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[9]);
    {
    int fcolor = GeColor.getDrawtype(42, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[10]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[11]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[13]);
    }
    }
    g.setColor(GeColor.getColor(43, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[14]);
    {
    int fcolor = GeColor.getDrawtype(43, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[16]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[17]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[18]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[19]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[20]);
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
  Dimension size;
  public pwr_slider1( JopSession session)
  {
    super( session);
    size = new Dimension( 34, 32);
  }
  public int original_width = 34;
  public int original_height = 32;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 24, 32, 32, 12, 2, 24, 24}, new int[] {2, 24, 30, 30, 22, 22, 2}, 7),
    new Polygon( new int[] { 24, 27, 33, 32}, new int[] { 2, 8, 24,24}, 4),
    new Polygon( new int[] { 32, 33, 33, 32}, new int[] { 24, 24, 29,30}, 4),
    new Polygon( new int[] { 32, 33, 14, 12}, new int[] { 30, 29, 29,30}, 4),
    new Polygon( new int[] { 12, 14, 7, 2}, new int[] { 30, 29, 23,22}, 4),
    new Polygon( new int[] { 2, 7, 27, 24}, new int[] { 22, 23, 23,22}, 4),
    new Polygon( new int[] { 24, 27, 27, 24}, new int[] { 22, 23, 8,2}, 4),
    new Rectangle2D.Float(2F, 2F, 22F, 20F),
    new Polygon( new int[] { 2, 24, 23, 3, 3, 2}, new int[] { 2, 2, 3, 3, 21, 22}, 6), 
    new Polygon( new int[] { 24, 24, 23, 23, 3, 2}, new int[] { 22, 2, 3, 21, 21, 22}, 6), 
    new Rectangle2D.Float(2F, 2F, 22F, 6F),
    new Polygon( new int[] { 2, 24, 24, 2, 2, 2}, new int[] { 2, 2, 2, 2, 8, 8}, 6), 
    new Polygon( new int[] { 24, 24, 24, 24, 2, 2}, new int[] { 8, 2, 2, 8, 8, 8}, 6), 
    new Rectangle2D.Float(2F, 16F, 22F, 6F),
    new Polygon( new int[] { 2, 24, 24, 2, 2, 2}, new int[] { 16, 16, 16, 16, 22, 22}, 6), 
    new Polygon( new int[] { 24, 24, 24, 24, 2, 2}, new int[] { 22, 16, 16, 22, 22, 22}, 6), 
    new Line2D.Float( 24F, 11.3482F, 2F, 11.3482F),
    new Line2D.Float( 24F, 12.391F, 2F, 12.391F),
    new Rectangle2D.Float(2F, 2F, 22F, 20F),
    new Polygon( new int[] { 2, 24, 23, 3, 3, 2}, new int[] { 2, 2, 3, 3, 21, 22}, 6), 
    new Polygon( new int[] { 24, 24, 23, 23, 3, 2}, new int[] { 22, 2, 3, 21, 21, 22}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( dd.invisible) {
      if ( !dd.invisibleOld) {
        dd.invisibleOld = dd.invisible;
        repaint();
      }
      else
        dd.invisibleOld = dd.invisible;
      return;
    }
    dd.invisibleOld = dd.invisible;
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
    g.setColor(GeColor.getColor(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[0]);
    {
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[3]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[5]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[6]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(102, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[7]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[8]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[9]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[10]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[11]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[12]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[13]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[14]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[15]);
    }
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[16]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[17]);
    {
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[19]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[20]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(29, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[18]);
    }
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
