package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcIvGen extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_valuemedium	pwr_valuemedium2;
  JopBar	jopBar4;
  JopTrend	jopTrend5;
  pwr_framethin	pwr_framethin6;
  pwr_framethin	pwr_framethin7;
  pwr_framethin	pwr_framethin8;
  JopButtontoggle	jopButtontoggle10;
  pwr_valuelong	pwr_valuelong11;
  pwr_sliderbackground1	pwr_sliderbackground112;
  pwr_slider1	pwr_slider113;
  JopButtontoggle	jopButtontoggle14;
  pwr_valueinputreliefup	pwr_valueinputreliefup15;
  pwr_valueinputreliefup	pwr_valueinputreliefup16;
  pwr_valueinputreliefup	pwr_valueinputreliefup17;
  pwr_valueinputmedium	pwr_valueinputmedium18;
  public JopcIvGen( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcIvGen( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "Œ-i@€l@œá@pïÿ¿8ïÿ¿ß}");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 811, 267);
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
      this.setTitle("JopcIvGen");
    pwr_valuemedium2 = new pwr_valuemedium(session);
    pwr_valuemedium2.setBounds(new Rectangle(127,107,94,23));
    pwr_valuemedium2.setFillColor(51);
    pwr_valuemedium2.setBorderColor(56);
    localPanel.add(pwr_valuemedium2, new Proportion(pwr_valuemedium2.getBounds(), dsize));
    jopBar4 = new JopBar(session);
    jopBar4.setBounds(new Rectangle(252,44,16,145));
    jopBar4.setFillColor(59);
    jopBar4.setBorderColor(56);
    jopBar4.setFillColorBar(148);
    jopBar4.setBorderColorBar(142);
    jopBar4.setDrawFill(1);
    jopBar4.setDrawBorder(1);
    jopBar4.setBarBorderWidth(3);
    jopBar4.setLineWidth(1);
    jopBar4.setMinValue(0F);
    jopBar4.setMaxValue(100F);
    jopBar4.setRotate(0);
    localPanel.add(jopBar4, new Proportion(jopBar4.getBounds(), dsize));
    jopTrend5 = new JopTrend( session);
    jopTrend5.setBounds(new Rectangle(289,43,479,145));
    jopTrend5.setFillColor(59);
    jopTrend5.setBorderColor(57);
    jopTrend5.setBorderColorTrend1(142);
    jopTrend5.setBorderColorTrend2(9999);
    jopTrend5.setFillColorTrend1(28);
    jopTrend5.setFillColorTrend2(9999);
    jopTrend5.setDrawFill(1);
    jopTrend5.setDrawBorder(1);
    jopTrend5.setTrendBorderWidth(1);
    jopTrend5.setLineWidth(1);
    jopTrend5.setMinValue1(0F);
    jopTrend5.setMaxValue1(100F);
    jopTrend5.setMinValue2(0F);
    jopTrend5.setMaxValue2(100F);
    jopTrend5.setNoOfPoints(200);
    jopTrend5.setHorizontalLines(4);
    jopTrend5.setVerticalLines(9);
    jopTrend5.setRotate(0);
    localPanel.add(jopTrend5, new Proportion(jopTrend5.getBounds(), dsize));
    pwr_framethin6 = new pwr_framethin(session);
    pwr_framethin6.setBounds(new Rectangle(9,67,223,79));
    localPanel.add(pwr_framethin6, new Proportion(pwr_framethin6.getBounds(), dsize));
    pwr_framethin7 = new pwr_framethin(session);
    pwr_framethin7.setBounds(new Rectangle(280,37,495,176));
    localPanel.add(pwr_framethin7, new Proportion(pwr_framethin7.getBounds(), dsize));
    pwr_framethin8 = new pwr_framethin(session);
    pwr_framethin8.setBounds(new Rectangle(247,38,25,157));
    localPanel.add(pwr_framethin8, new Proportion(pwr_framethin8.getBounds(), dsize));
    jopButtontoggle10 = new JopButtontoggle(session);
    jopButtontoggle10.setBounds(new Rectangle(506,189,59,20));
    jopButtontoggle10.setColorTone(1);
    localPanel.add(jopButtontoggle10, new Proportion(jopButtontoggle10.getBounds(), dsize));
    pwr_valuelong11 = new pwr_valuelong(session);
    pwr_valuelong11.setBounds(new Rectangle(125,7,678,22));
    pwr_valuelong11.setFillColor(41);
    pwr_valuelong11.setBorderColor(46);
    localPanel.add(pwr_valuelong11, new Proportion(pwr_valuelong11.getBounds(), dsize));
    pwr_sliderbackground112 = new pwr_sliderbackground1(session);
    pwr_sliderbackground112.setBounds(new Rectangle(782,32,20,167));
    pwr_sliderbackground112.setColorBrightness(2);
    localPanel.add(pwr_sliderbackground112, new Proportion(pwr_sliderbackground112.getBounds(), dsize));
    pwr_slider113 = new pwr_slider1(session);
    pwr_slider113.setBounds(new Rectangle(783,37,21,18));
    pwr_slider113.setColorTone(1);
    localPanel.add(pwr_slider113, new Proportion(pwr_slider113.getBounds(), dsize));
    jopButtontoggle14 = new JopButtontoggle(session);
    jopButtontoggle14.setBounds(new Rectangle(675,189,63,20));
    jopButtontoggle14.setColorTone(1);
    localPanel.add(jopButtontoggle14, new Proportion(jopButtontoggle14.getBounds(), dsize));
    pwr_valueinputreliefup15 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup15.setBounds(new Rectangle(203,37,41,18));
    pwr_valueinputreliefup15.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup15, new Proportion(pwr_valueinputreliefup15.getBounds(), dsize));
    pwr_valueinputreliefup16 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup16.setBounds(new Rectangle(203,180,41,18));
    pwr_valueinputreliefup16.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup16, new Proportion(pwr_valueinputreliefup16.getBounds(), dsize));
    pwr_valueinputreliefup17 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup17.setBounds(new Rectangle(289,191,41,18));
    pwr_valueinputreliefup17.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup17, new Proportion(pwr_valueinputreliefup17.getBounds(), dsize));
    pwr_valueinputmedium18 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium18.setBounds(new Rectangle(127,82,94,23));
    pwr_valueinputmedium18.setBorderColor(47);
    localPanel.add(pwr_valueinputmedium18, new Proportion(pwr_valueinputmedium18.getBounds(), dsize));
    pwr_valuemedium2.dd.setDynType(1025);
    pwr_valuemedium2.dd.setActionType(0);
    pwr_valuemedium2.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium2.dd, "$object.InitialValue##Int32","%d")
      });
    jopBar4.setPwrAttribute("$object.ActualValue##Int32");
    jopTrend5.setPwrAttribute1("$object.ActualValue##Int32");
    pwr_framethin6.dd.setDynType(1);
    pwr_framethin6.dd.setActionType(0);
    pwr_framethin7.dd.setDynType(1);
    pwr_framethin7.dd.setActionType(0);
    pwr_framethin8.dd.setDynType(1);
    pwr_framethin8.dd.setActionType(0);
    jopButtontoggle10.dd.setDynType(0);
    jopButtontoggle10.dd.setActionType(4);
    jopButtontoggle10.dd.setAccess(65535);
    jopButtontoggle10.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(jopButtontoggle10.dd, "$local.TrendHold##Boolean")
      });
    jopButtontoggle10.setAnnot1("   Hold");
    pwr_valuelong11.dd.setDynType(1025);
    pwr_valuelong11.dd.setActionType(0);
    pwr_valuelong11.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong11.dd, "$object.Description##String80","%s")
      });
    pwr_sliderbackground112.dd.setDynType(1);
    pwr_sliderbackground112.dd.setActionType(0);
    pwr_slider113.dd.setDynType(1);
    pwr_slider113.dd.setActionType(2048);
    pwr_slider113.dd.setAccess(6);
    pwr_slider113.dd.setElements(new GeDynElemIfc[] {
      new GeDynSlider(pwr_slider113.dd, "$object.ActualValue##Int32",0,100,4,37.7724,179.758)
      });
    jopButtontoggle14.dd.setDynType(0);
    jopButtontoggle14.dd.setActionType(4);
    jopButtontoggle14.dd.setAccess(6);
    jopButtontoggle14.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(jopButtontoggle14.dd, "$local.SliderDisable##Boolean")
      });
    jopButtontoggle14.setAnnot1(" Slider");
    pwr_valueinputreliefup15.dd.setDynType(1024);
    pwr_valueinputreliefup15.dd.setActionType(4096);
    pwr_valueinputreliefup15.dd.setAccess(65535);
    pwr_valueinputreliefup15.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup15.dd, "$object.PresMaxLimit##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup15.dd, 0,0)
      });
    pwr_valueinputreliefup16.dd.setDynType(1024);
    pwr_valueinputreliefup16.dd.setActionType(4096);
    pwr_valueinputreliefup16.dd.setAccess(65535);
    pwr_valueinputreliefup16.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup16.dd, "$object.PresMinLimit##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup16.dd, 0,0)
      });
    pwr_valueinputreliefup17.dd.setDynType(1024);
    pwr_valueinputreliefup17.dd.setActionType(4096);
    pwr_valueinputreliefup17.dd.setAccess(65535);
    pwr_valueinputreliefup17.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup17.dd, "$local.ScanTime##Float32","%5.0f")
      ,new GeDynValueInput(pwr_valueinputreliefup17.dd, 4,100000)
      });
    pwr_valueinputmedium18.dd.setDynType(1024);
    pwr_valueinputmedium18.dd.setActionType(4096);
    pwr_valueinputmedium18.dd.setAccess(6);
    pwr_valueinputmedium18.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium18.dd, "$object.ActualValue##Int32","%d")
      ,new GeDynValueInput(pwr_valueinputmedium18.dd, 0,0)
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
  public int original_width = 803;
  public int original_height = 223;
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
    g.drawString( "Description",20, 23);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "ActualValue",20, 98);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "InitialValue",20, 123);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.PLAIN, 8));
    g.drawString( "0",761, 200);
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
  int original_width = 48;
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
protected class pwr_valueinputmedium extends GeTextField {
  public pwr_valueinputmedium( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( annotBackground);
  }
  int original_width = 62;
  int original_height = 23;
  boolean fontSet = false;
  Font annotFont = new Font("Helvetica", Font.BOLD, 12);
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
    size = new Dimension( 46, 20);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 46;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 42.865F, 16.5294F),
    new Polygon( new int[] { 2, 45, 44, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 45, 45, 44, 44, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
    new Polygon( new int[] { 2, 5, 5, 42, 45, 2, 2}, new int[] {19, 15, 5, 5, 2, 2, 19}, 7),
    new Polygon( new int[] { 2, 5, 42, 42, 45, 45, 2}, new int[] {19, 15, 15, 5, 2, 19, 19}, 7),
    new Rectangle2D.Float(2.00001F, 2F, 42.865F, 16.5294F),
    new Polygon( new int[] { 2, 45, 44, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 45, 45, 44, 44, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
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
      g.drawString( annot1, 7 * original_height / height * width / original_width, 15F);
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
    size = new Dimension( 62, 23);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 12);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 62;
  public int original_height = 23;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 58.1114F, 19.3705F),
    new Polygon( new int[] { 2, 60, 59, 3, 3, 2}, new int[] { 2, 2, 3, 3, 20, 21}, 6), 
    new Polygon( new int[] { 60, 60, 59, 59, 3, 2}, new int[] { 21, 2, 3, 20, 20, 21}, 6), 
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
    size = new Dimension( 519, 17);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 8);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 519;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 515.254F, 13.5593F),
    new Polygon( new int[] { 2, 517, 517, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 517, 517, 517, 517, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
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
    size = new Dimension( 42, 217);
  }
  public int original_width = 42;
  public int original_height = 217;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 2, 41, 21, 2}, new int[] {2, 2, 21, 2}, 4),
    new Polygon( new int[] { 2, 4, 38, 41}, new int[] { 2, 3, 3,2}, 4),
    new Polygon( new int[] { 41, 38, 21, 21}, new int[] { 2, 3, 20,21}, 4),
    new Polygon( new int[] { 21, 21, 4, 2}, new int[] { 21, 20, 3,2}, 4),
    new Polygon( new int[] { 41, 21, 21, 41, 41}, new int[] {2, 21, 196, 215, 2}, 5),
    new Polygon( new int[] { 41, 40, 22, 21}, new int[] { 2, 4, 21,21}, 4),
    new Polygon( new int[] { 21, 22, 22, 21}, new int[] { 21, 21, 196,196}, 4),
    new Polygon( new int[] { 21, 22, 40, 41}, new int[] { 196, 196, 213,215}, 4),
    new Polygon( new int[] { 41, 40, 40, 41}, new int[] { 215, 213, 4,2}, 4),
    new Polygon( new int[] { 2, 2, 21, 21, 2}, new int[] {2, 215, 196, 21, 2}, 5),
    new Polygon( new int[] { 2, 3, 3, 2}, new int[] { 2, 4, 213,215}, 4),
    new Polygon( new int[] { 2, 3, 20, 21}, new int[] { 215, 213, 196,196}, 4),
    new Polygon( new int[] { 21, 20, 20, 21}, new int[] { 196, 196, 21,21}, 4),
    new Polygon( new int[] { 21, 20, 3, 2}, new int[] { 21, 21, 4,2}, 4),
    new Polygon( new int[] { 2, 21, 41, 2}, new int[] {215, 196, 215, 215}, 4),
    new Polygon( new int[] { 2, 4, 21, 21}, new int[] { 215, 214, 197,196}, 4),
    new Polygon( new int[] { 21, 21, 38, 41}, new int[] { 196, 197, 214,215}, 4),
    new Polygon( new int[] { 41, 38, 4, 2}, new int[] { 215, 214, 214,215}, 4),
    new Rectangle2D.Float(17.4964F, 17.4964F, 7.74818F, 182.082F),
    new Polygon( new int[] { 17, 25, 25, 18, 18, 17}, new int[] { 17, 17, 18, 18, 199, 200}, 6), 
    new Polygon( new int[] { 25, 25, 25, 25, 18, 17}, new int[] { 200, 17, 18, 199, 199, 200}, 6), 
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
    size = new Dimension( 33, 31);
  }
  public int original_width = 33;
  public int original_height = 31;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 23, 31, 31, 12, 2, 23, 23}, new int[] {2, 23, 29, 29, 21, 21, 2}, 7),
    new Polygon( new int[] { 23, 24, 30, 31}, new int[] { 2, 7, 23,23}, 4),
    new Polygon( new int[] { 31, 30, 30, 31}, new int[] { 23, 23, 28,29}, 4),
    new Polygon( new int[] { 31, 30, 12, 12}, new int[] { 29, 28, 28,29}, 4),
    new Polygon( new int[] { 12, 12, 5, 2}, new int[] { 29, 28, 22,21}, 4),
    new Polygon( new int[] { 2, 5, 24, 23}, new int[] { 21, 22, 22,21}, 4),
    new Polygon( new int[] { 23, 24, 24, 23}, new int[] { 21, 22, 7,2}, 4),
    new Rectangle2D.Float(2F, 2F, 21.3075F, 19.3705F),
    new Polygon( new int[] { 2, 23, 22, 3, 3, 2}, new int[] { 2, 2, 3, 3, 20, 21}, 6), 
    new Polygon( new int[] { 23, 23, 22, 22, 3, 2}, new int[] { 21, 2, 3, 20, 20, 21}, 6), 
    new Rectangle2D.Float(2F, 2F, 21.3075F, 5.81114F),
    new Polygon( new int[] { 2, 23, 23, 2, 2, 2}, new int[] { 2, 2, 2, 2, 8, 8}, 6), 
    new Polygon( new int[] { 23, 23, 23, 23, 2, 2}, new int[] { 8, 2, 2, 8, 8, 8}, 6), 
    new Rectangle2D.Float(2F, 15.5593F, 21.3075F, 5.81114F),
    new Polygon( new int[] { 2, 23, 23, 2, 2, 2}, new int[] { 16, 16, 16, 16, 21, 21}, 6), 
    new Polygon( new int[] { 23, 23, 23, 23, 2, 2}, new int[] { 21, 16, 16, 21, 21, 21}, 6), 
    new Line2D.Float( 23.3075F, 11.0539F, 2F, 11.0539F),
    new Line2D.Float( 23.3075F, 12.0639F, 2F, 12.0639F),
    new Rectangle2D.Float(2F, 2F, 21.3075F, 19.3705F),
    new Polygon( new int[] { 2, 23, 22, 3, 3, 2}, new int[] { 2, 2, 3, 3, 20, 21}, 6), 
    new Polygon( new int[] { 23, 23, 22, 22, 3, 2}, new int[] { 21, 2, 3, 20, 20, 21}, 6), 
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
