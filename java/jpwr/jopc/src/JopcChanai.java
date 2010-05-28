package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import jpwr.jopc.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcChanai extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  public LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_valuemedium	pwr_valuemedium0;
  pwr_valuemedium	pwr_valuemedium3;
  pwr_valuemedium	pwr_valuemedium5;
  pwr_valuelong	pwr_valuelong7;
  pwr_buttoncommand	pwr_buttoncommand8;
  pwr_valuelong	pwr_valuelong9;
  pwr_valuemedium	pwr_valuemedium11;
  pwr_valuemedium	pwr_valuemedium13;
  pwr_valuemedium	pwr_valuemedium15;
  pwr_valuemedium	pwr_valuemedium17;
  pwr_valuemedium	pwr_valuemedium19;
  pwr_valuemedium	pwr_valuemedium21;
  pwr_valuemedium	pwr_valuemedium23;
  pwr_valuemedium	pwr_valuemedium25;
  pwr_valuemedium	pwr_valuemedium27;
  pwr_framethin	pwr_framethin30;
  JopButtontoggle	jopButtontoggle32;
  public JopcChanai( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcChanai( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "ä'¢·D±ù·D±ù·p—uÄ¿¡·›§· “§·(q¦¿ç	Ÿ·D±ù·D±ù·x—u “§·");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 711, 386);
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
      this.setTitle("JopcChanai");
    pwr_valuemedium0 = new pwr_valuemedium(session);
    pwr_valuemedium0.setBounds(new Rectangle(132,54,91,22));
    pwr_valuemedium0.setFillColor(51);
    pwr_valuemedium0.setBorderColor(56);
    localPanel.add(pwr_valuemedium0, new Proportion(pwr_valuemedium0.getBounds(), dsize));
    pwr_valuemedium3 = new pwr_valuemedium(session);
    pwr_valuemedium3.setBounds(new Rectangle(132,79,91,22));
    pwr_valuemedium3.setFillColor(51);
    pwr_valuemedium3.setBorderColor(56);
    localPanel.add(pwr_valuemedium3, new Proportion(pwr_valuemedium3.getBounds(), dsize));
    pwr_valuemedium5 = new pwr_valuemedium(session);
    pwr_valuemedium5.setBounds(new Rectangle(132,104,91,22));
    pwr_valuemedium5.setFillColor(51);
    pwr_valuemedium5.setBorderColor(56);
    localPanel.add(pwr_valuemedium5, new Proportion(pwr_valuemedium5.getBounds(), dsize));
    pwr_valuelong7 = new pwr_valuelong(session);
    pwr_valuelong7.setBounds(new Rectangle(132,4,570,21));
    pwr_valuelong7.setFillColor(51);
    pwr_valuelong7.setBorderColor(46);
    localPanel.add(pwr_valuelong7, new Proportion(pwr_valuelong7.getBounds(), dsize));
    pwr_buttoncommand8 = new pwr_buttoncommand(session);
    pwr_buttoncommand8.setBounds(new Rectangle(8,308,83,28));
    pwr_buttoncommand8.setColorTone(1);
    localPanel.add(pwr_buttoncommand8, new Proportion(pwr_buttoncommand8.getBounds(), dsize));
    pwr_valuelong9 = new pwr_valuelong(session);
    pwr_valuelong9.setBounds(new Rectangle(132,29,570,21));
    pwr_valuelong9.setFillColor(51);
    pwr_valuelong9.setBorderColor(46);
    localPanel.add(pwr_valuelong9, new Proportion(pwr_valuelong9.getBounds(), dsize));
    pwr_valuemedium11 = new pwr_valuemedium(session);
    pwr_valuemedium11.setBounds(new Rectangle(132,129,91,22));
    pwr_valuemedium11.setFillColor(51);
    pwr_valuemedium11.setBorderColor(56);
    localPanel.add(pwr_valuemedium11, new Proportion(pwr_valuemedium11.getBounds(), dsize));
    pwr_valuemedium13 = new pwr_valuemedium(session);
    pwr_valuemedium13.setBounds(new Rectangle(169,187,91,22));
    pwr_valuemedium13.setFillColor(51);
    pwr_valuemedium13.setBorderColor(56);
    localPanel.add(pwr_valuemedium13, new Proportion(pwr_valuemedium13.getBounds(), dsize));
    pwr_valuemedium15 = new pwr_valuemedium(session);
    pwr_valuemedium15.setBounds(new Rectangle(285,187,91,22));
    pwr_valuemedium15.setFillColor(51);
    pwr_valuemedium15.setBorderColor(56);
    localPanel.add(pwr_valuemedium15, new Proportion(pwr_valuemedium15.getBounds(), dsize));
    pwr_valuemedium17 = new pwr_valuemedium(session);
    pwr_valuemedium17.setBounds(new Rectangle(169,212,91,22));
    pwr_valuemedium17.setFillColor(51);
    pwr_valuemedium17.setBorderColor(56);
    localPanel.add(pwr_valuemedium17, new Proportion(pwr_valuemedium17.getBounds(), dsize));
    pwr_valuemedium19 = new pwr_valuemedium(session);
    pwr_valuemedium19.setBounds(new Rectangle(285,212,91,22));
    pwr_valuemedium19.setFillColor(51);
    pwr_valuemedium19.setBorderColor(56);
    localPanel.add(pwr_valuemedium19, new Proportion(pwr_valuemedium19.getBounds(), dsize));
    pwr_valuemedium21 = new pwr_valuemedium(session);
    pwr_valuemedium21.setBounds(new Rectangle(169,238,91,22));
    pwr_valuemedium21.setFillColor(51);
    pwr_valuemedium21.setBorderColor(56);
    localPanel.add(pwr_valuemedium21, new Proportion(pwr_valuemedium21.getBounds(), dsize));
    pwr_valuemedium23 = new pwr_valuemedium(session);
    pwr_valuemedium23.setBounds(new Rectangle(285,238,91,22));
    pwr_valuemedium23.setFillColor(51);
    pwr_valuemedium23.setBorderColor(56);
    localPanel.add(pwr_valuemedium23, new Proportion(pwr_valuemedium23.getBounds(), dsize));
    pwr_valuemedium25 = new pwr_valuemedium(session);
    pwr_valuemedium25.setBounds(new Rectangle(169,264,91,22));
    pwr_valuemedium25.setFillColor(51);
    pwr_valuemedium25.setBorderColor(56);
    localPanel.add(pwr_valuemedium25, new Proportion(pwr_valuemedium25.getBounds(), dsize));
    pwr_valuemedium27 = new pwr_valuemedium(session);
    pwr_valuemedium27.setBounds(new Rectangle(285,264,91,22));
    pwr_valuemedium27.setFillColor(51);
    pwr_valuemedium27.setBorderColor(56);
    localPanel.add(pwr_valuemedium27, new Proportion(pwr_valuemedium27.getBounds(), dsize));
    pwr_framethin30 = new pwr_framethin(session);
    pwr_framethin30.setBounds(new Rectangle(0,157,389,142));
    localPanel.add(pwr_framethin30, new Proportion(pwr_framethin30.getBounds(), dsize));
    jopButtontoggle32 = new JopButtontoggle(session);
    jopButtontoggle32.setBounds(new Rectangle(282,161,99,19));
    jopButtontoggle32.setColorTone(1);
    localPanel.add(jopButtontoggle32, new Proportion(jopButtontoggle32.getBounds(), dsize));
    pwr_valuemedium0.dd.setDynType(1024);
    pwr_valuemedium0.dd.setActionType(0);
    pwr_valuemedium0.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium0.dd, "$object.Number##UInt16","%d")
      });
    pwr_valuemedium3.dd.setDynType(1025);
    pwr_valuemedium3.dd.setActionType(0);
    pwr_valuemedium3.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium3.dd, "$object.ConversionOn##Boolean","%d")
      });
    pwr_valuemedium5.dd.setDynType(1025);
    pwr_valuemedium5.dd.setActionType(0);
    pwr_valuemedium5.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium5.dd, "$object.ScanInterval##UInt32","%d")
      });
    pwr_valuelong7.dd.setDynType(1025);
    pwr_valuelong7.dd.setActionType(0);
    pwr_valuelong7.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong7.dd, "$object.Description##String80","%s")
      });
    pwr_buttoncommand8.dd.setDynType(1);
    pwr_buttoncommand8.dd.setActionType(64);
    pwr_buttoncommand8.dd.setAccess(65535);
    pwr_buttoncommand8.dd.setElements(new GeDynElemIfc[] {
      new GeDynCommand(pwr_buttoncommand8.dd, "open graph/classgraph/inst=&$object.SigChanCon")
      });
    pwr_buttoncommand8.setAnnot1("Signal");
    pwr_valuelong9.dd.setDynType(1025);
    pwr_valuelong9.dd.setActionType(0);
    pwr_valuelong9.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong9.dd, "$object.Identity##String80","%s")
      });
    pwr_valuemedium11.dd.setDynType(1025);
    pwr_valuemedium11.dd.setActionType(0);
    pwr_valuemedium11.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium11.dd, "$object.SensorPolyType##UInt32","%d")
      });
    pwr_valuemedium13.dd.setDynType(1024);
    pwr_valuemedium13.dd.setActionType(4096);
    pwr_valuemedium13.dd.setAccess(38);
    pwr_valuemedium13.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium13.dd, "$object.ActValRangeLow##Float32","%10.3f")
      });
    pwr_valuemedium15.dd.setDynType(1024);
    pwr_valuemedium15.dd.setActionType(4096);
    pwr_valuemedium15.dd.setAccess(38);
    pwr_valuemedium15.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium15.dd, "$object.ActValRangeHigh##Float32","%10.3f")
      });
    pwr_valuemedium17.dd.setDynType(1024);
    pwr_valuemedium17.dd.setActionType(4096);
    pwr_valuemedium17.dd.setAccess(38);
    pwr_valuemedium17.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium17.dd, "$object.SensorSigValRangeLow##Float32","%10.3f")
      });
    pwr_valuemedium19.dd.setDynType(1024);
    pwr_valuemedium19.dd.setActionType(4096);
    pwr_valuemedium19.dd.setAccess(38);
    pwr_valuemedium19.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium19.dd, "$object.SensorSigValRangeHigh##Float32","%10.3f")
      });
    pwr_valuemedium21.dd.setDynType(1024);
    pwr_valuemedium21.dd.setActionType(4096);
    pwr_valuemedium21.dd.setAccess(38);
    pwr_valuemedium21.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium21.dd, "$object.ChannelSigValRangeLow##Float32","%10.3f")
      });
    pwr_valuemedium23.dd.setDynType(1024);
    pwr_valuemedium23.dd.setActionType(4096);
    pwr_valuemedium23.dd.setAccess(38);
    pwr_valuemedium23.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium23.dd, "$object.ChannelSigValRangeHigh##Float32","%10.3f")
      });
    pwr_valuemedium25.dd.setDynType(1024);
    pwr_valuemedium25.dd.setActionType(4096);
    pwr_valuemedium25.dd.setAccess(38);
    pwr_valuemedium25.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium25.dd, "$object.RawValRangeLow##Float32","%10.3f")
      });
    pwr_valuemedium27.dd.setDynType(1024);
    pwr_valuemedium27.dd.setActionType(4096);
    pwr_valuemedium27.dd.setAccess(38);
    pwr_valuemedium27.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium27.dd, "$object.RawValRangeHigh##Float32","%10.3f")
      });
    pwr_framethin30.dd.setDynType(1);
    pwr_framethin30.dd.setActionType(0);
    jopButtontoggle32.dd.setDynType(0);
    jopButtontoggle32.dd.setActionType(4);
    jopButtontoggle32.dd.setAccess(38);
    jopButtontoggle32.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(jopButtontoggle32.dd, "$object.CalculateNewCoef##Boolean")
      });
    jopButtontoggle32.setAnnot1("Update range");
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
  public int original_width = 703;
  public int original_height = 342;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Line2D.Float( 385.677F, 182.403F, 2.93967F, 182.403F),
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
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("Description"),8, 20);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("Number"),8, 68);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("ConversionOn"),8, 92);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("ScanInterval"),8, 118);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("Identity"),8, 41);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("SensorPolyType"),8, 145);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("ActualValue"),8, 202);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("-"),268, 202);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("SensorSignalValue"),8, 228);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("-"),268, 228);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("ChannelSignalValue"),8, 254);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("-"),268, 254);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("RawValue"),8, 280);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("-"),268, 280);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("Range"),8, 175);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_buttoncommand extends GeComponent {
  // Dimension size;
  public pwr_buttoncommand( JopSession session)
  {
    super( session);
    size = new Dimension( 96, 34);
    annot1Font = new Font("Helvetica", Font.BOLD, 12);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 96;
  public int original_height = 34;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 92.0805F, 30.0671F),
    new Polygon( new int[] { 2, 94, 93, 4, 4, 2}, new int[] { 2, 2, 4, 4, 31, 32}, 6), 
    new Polygon( new int[] { 94, 94, 93, 93, 4, 2}, new int[] { 32, 2, 4, 31, 31, 32}, 6), 
    new Polygon( new int[] { 2, 8, 8, 88, 94, 2, 2}, new int[] {32, 26, 8, 8, 2, 2, 32}, 7),
    new Polygon( new int[] { 2, 4, 5, 8}, new int[] { 32, 26, 25,26}, 4),
    new Polygon( new int[] { 8, 5, 5, 8}, new int[] { 26, 25, 7,8}, 4),
    new Polygon( new int[] { 8, 5, 87, 88}, new int[] { 8, 7, 7,8}, 4),
    new Polygon( new int[] { 88, 87, 88, 94}, new int[] { 8, 7, 6,2}, 4),
    new Polygon( new int[] { 94, 88, 4, 2}, new int[] { 2, 6, 6,2}, 4),
    new Polygon( new int[] { 2, 4, 4, 2}, new int[] { 2, 6, 26,32}, 4),
    new Polygon( new int[] { 2, 4, 142449724, -75}, new int[] { 32, 26, 13,12}, 4),
    new Polygon( new int[] { 2, 8, 88, 88, 94, 94, 2}, new int[] {32, 26, 26, 8, 2, 32, 32}, 7),
    new Polygon( new int[] { 2, 6, 8, 8}, new int[] { 32, 30, 28,26}, 4),
    new Polygon( new int[] { 8, 8, 90, 88}, new int[] { 26, 28, 28,26}, 4),
    new Polygon( new int[] { 88, 90, 90, 88}, new int[] { 26, 28, 10,8}, 4),
    new Polygon( new int[] { 88, 90, 92, 94}, new int[] { 8, 10, 8,2}, 4),
    new Polygon( new int[] { 94, 92, 92, 94}, new int[] { 2, 8, 30,32}, 4),
    new Polygon( new int[] { 94, 92, 6, 2}, new int[] { 32, 30, 30,32}, 4),
    new Polygon( new int[] { 2, 6, -75, -75}, new int[] { 32, 30, 15,12}, 4),
    new Rectangle2D.Float(2F, 2F, 92.0805F, 30.0671F),
    new Polygon( new int[] { 2, 94, 93, 4, 4, 2}, new int[] { 2, 2, 4, 4, 31, 32}, 6), 
    new Polygon( new int[] { 94, 94, 93, 93, 4, 2}, new int[] { 32, 2, 4, 31, 31, 32}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,92.0805F,30.0671F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,92.0805F,30.0671F, false,100, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[3]);
    }
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
    {
    int fcolor = GeColor.getDrawtype(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,92.0805F,30.0671F, false,104, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[13]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[14]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[16]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[17]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[20]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[21]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[19]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 13 * original_height / height * width / original_width, 20F);
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
  // Dimension size;
  public pwr_valuemedium( JopSession session)
  {
    super( session);
    size = new Dimension( 60, 22);
    annot1Font = new Font("Helvetica", Font.BOLD, 12);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 60;
  public int original_height = 22;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 56.3758F, 18.7919F),
    new Polygon( new int[] { 2, 58, 57, 3, 3, 2}, new int[] { 2, 2, 3, 3, 20, 21}, 6), 
    new Polygon( new int[] { 58, 58, 57, 57, 3, 2}, new int[] { 21, 2, 3, 20, 20, 21}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,56.3758F,18.7919F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
  // Dimension size;
  public pwr_valuelong( JopSession session)
  {
    super( session);
    size = new Dimension( 503, 17);
    annot1Font = new Font("Helvetica", Font.BOLD, 8);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 503;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 499.866F, 13.1544F),
    new Polygon( new int[] { 2, 502, 501, 3, 3, 2}, new int[] { 2, 2, 3, 3, 14, 15}, 6), 
    new Polygon( new int[] { 502, 502, 501, 501, 3, 2}, new int[] { 15, 2, 3, 14, 14, 15}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,499.866F,13.1544F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
      g.drawString( annot1, 7 * original_height / height * width / original_width, 12F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class JopButtontoggle extends GeComponent {
  // Dimension size;
  public JopButtontoggle( JopSession session)
  {
    super( session);
    size = new Dimension( 45, 20);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 45;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 41.5848F, 16.0358F),
    new Polygon( new int[] { 2, 44, 43, 3, 3, 2}, new int[] { 2, 2, 3, 3, 17, 18}, 6), 
    new Polygon( new int[] { 44, 44, 43, 43, 3, 2}, new int[] { 18, 2, 3, 17, 17, 18}, 6), 
    new Polygon( new int[] { 2, 5, 5, 41, 44, 2, 2}, new int[] {18, 15, 5, 5, 2, 2, 18}, 7),
    new Polygon( new int[] { 2, 5, 41, 41, 44, 44, 2}, new int[] {18, 15, 15, 5, 2, 18, 18}, 7),
    new Rectangle2D.Float(2.00001F, 2F, 41.5848F, 16.0358F),
    new Polygon( new int[] { 2, 44, 43, 3, 3, 2}, new int[] { 2, 2, 3, 3, 17, 18}, 6), 
    new Polygon( new int[] { 44, 44, 43, 43, 3, 2}, new int[] { 18, 2, 3, 17, 17, 18}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,41.5848F,16.0358F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[4]);
    }
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
