package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcIohandler extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_indsquare	pwr_indsquare2;
  pwr_indsquare	pwr_indsquare3;
  pwr_valuesmall	pwr_valuesmall6;
  pwr_valuelong	pwr_valuelong7;
  pwr_valuelong	pwr_valuelong8;
  pwr_indsquare	pwr_indsquare10;
  pwr_indsquare	pwr_indsquare12;
  pwr_valuesmall	pwr_valuesmall16;
  pwr_valuesmall	pwr_valuesmall18;
  pwr_valuesmall	pwr_valuesmall20;
  pwr_valuesmall	pwr_valuesmall22;
  pwr_valuesmall	pwr_valuesmall24;
  pwr_valuesmall	pwr_valuesmall26;
  pwr_valuesmall	pwr_valuesmall28;
  pwr_valuesmall	pwr_valuesmall30;
  pwr_valuesmall	pwr_valuesmall32;
  pwr_valuesmall	pwr_valuesmall34;
  public JopcIohandler( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcIohandler( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "‰Y");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 712, 278);
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
      this.setTitle("JopcIohandler");
    pwr_indsquare2 = new pwr_indsquare(session);
    pwr_indsquare2.setBounds(new Rectangle(135,75,17,18));
    localPanel.add(pwr_indsquare2, new Proportion(pwr_indsquare2.getBounds(), dsize));
    pwr_indsquare3 = new pwr_indsquare(session);
    pwr_indsquare3.setBounds(new Rectangle(135,95,17,18));
    localPanel.add(pwr_indsquare3, new Proportion(pwr_indsquare3.getBounds(), dsize));
    pwr_valuesmall6 = new pwr_valuesmall(session);
    pwr_valuesmall6.setBounds(new Rectangle(134,9,55,19));
    pwr_valuesmall6.setFillColor(31);
    pwr_valuesmall6.setBorderColor(33);
    localPanel.add(pwr_valuesmall6, new Proportion(pwr_valuesmall6.getBounds(), dsize));
    pwr_valuelong7 = new pwr_valuelong(session);
    pwr_valuelong7.setBounds(new Rectangle(153,74,532,19));
    pwr_valuelong7.setBorderColor(33);
    localPanel.add(pwr_valuelong7, new Proportion(pwr_valuelong7.getBounds(), dsize));
    pwr_valuelong8 = new pwr_valuelong(session);
    pwr_valuelong8.setBounds(new Rectangle(153,95,532,19));
    pwr_valuelong8.setBorderColor(33);
    localPanel.add(pwr_valuelong8, new Proportion(pwr_valuelong8.getBounds(), dsize));
    pwr_indsquare10 = new pwr_indsquare(session);
    pwr_indsquare10.setBounds(new Rectangle(135,35,17,18));
    localPanel.add(pwr_indsquare10, new Proportion(pwr_indsquare10.getBounds(), dsize));
    pwr_indsquare12 = new pwr_indsquare(session);
    pwr_indsquare12.setBounds(new Rectangle(135,55,17,18));
    localPanel.add(pwr_indsquare12, new Proportion(pwr_indsquare12.getBounds(), dsize));
    pwr_valuesmall16 = new pwr_valuesmall(session);
    pwr_valuesmall16.setBounds(new Rectangle(57,156,55,19));
    pwr_valuesmall16.setFillColor(31);
    pwr_valuesmall16.setBorderColor(33);
    localPanel.add(pwr_valuesmall16, new Proportion(pwr_valuesmall16.getBounds(), dsize));
    pwr_valuesmall18 = new pwr_valuesmall(session);
    pwr_valuesmall18.setBounds(new Rectangle(57,177,55,19));
    pwr_valuesmall18.setFillColor(31);
    pwr_valuesmall18.setBorderColor(33);
    localPanel.add(pwr_valuesmall18, new Proportion(pwr_valuesmall18.getBounds(), dsize));
    pwr_valuesmall20 = new pwr_valuesmall(session);
    pwr_valuesmall20.setBounds(new Rectangle(57,197,55,19));
    pwr_valuesmall20.setFillColor(31);
    pwr_valuesmall20.setBorderColor(33);
    localPanel.add(pwr_valuesmall20, new Proportion(pwr_valuesmall20.getBounds(), dsize));
    pwr_valuesmall22 = new pwr_valuesmall(session);
    pwr_valuesmall22.setBounds(new Rectangle(207,156,55,19));
    pwr_valuesmall22.setFillColor(31);
    pwr_valuesmall22.setBorderColor(33);
    localPanel.add(pwr_valuesmall22, new Proportion(pwr_valuesmall22.getBounds(), dsize));
    pwr_valuesmall24 = new pwr_valuesmall(session);
    pwr_valuesmall24.setBounds(new Rectangle(207,177,55,19));
    pwr_valuesmall24.setFillColor(31);
    pwr_valuesmall24.setBorderColor(33);
    localPanel.add(pwr_valuesmall24, new Proportion(pwr_valuesmall24.getBounds(), dsize));
    pwr_valuesmall26 = new pwr_valuesmall(session);
    pwr_valuesmall26.setBounds(new Rectangle(207,197,55,19));
    pwr_valuesmall26.setFillColor(31);
    pwr_valuesmall26.setBorderColor(33);
    localPanel.add(pwr_valuesmall26, new Proportion(pwr_valuesmall26.getBounds(), dsize));
    pwr_valuesmall28 = new pwr_valuesmall(session);
    pwr_valuesmall28.setBounds(new Rectangle(364,156,55,19));
    pwr_valuesmall28.setFillColor(31);
    pwr_valuesmall28.setBorderColor(33);
    localPanel.add(pwr_valuesmall28, new Proportion(pwr_valuesmall28.getBounds(), dsize));
    pwr_valuesmall30 = new pwr_valuesmall(session);
    pwr_valuesmall30.setBounds(new Rectangle(364,177,55,19));
    pwr_valuesmall30.setFillColor(31);
    pwr_valuesmall30.setBorderColor(33);
    localPanel.add(pwr_valuesmall30, new Proportion(pwr_valuesmall30.getBounds(), dsize));
    pwr_valuesmall32 = new pwr_valuesmall(session);
    pwr_valuesmall32.setBounds(new Rectangle(364,197,55,19));
    pwr_valuesmall32.setFillColor(31);
    pwr_valuesmall32.setBorderColor(33);
    localPanel.add(pwr_valuesmall32, new Proportion(pwr_valuesmall32.getBounds(), dsize));
    pwr_valuesmall34 = new pwr_valuesmall(session);
    pwr_valuesmall34.setBounds(new Rectangle(521,156,55,19));
    pwr_valuesmall34.setFillColor(31);
    pwr_valuesmall34.setBorderColor(33);
    localPanel.add(pwr_valuesmall34, new Proportion(pwr_valuesmall34.getBounds(), dsize));
    pwr_indsquare2.dd.setDynType(33554432);
    pwr_indsquare2.dd.setActionType(0);
    pwr_indsquare2.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare2.dd, "$node.ProcStatus[4]##status",38)
      });
    pwr_indsquare3.dd.setDynType(33554432);
    pwr_indsquare3.dd.setActionType(0);
    pwr_indsquare3.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare3.dd, "$node.ProcMsgSeverity[4]##status",38)
      });
    pwr_valuesmall6.dd.setDynType(1025);
    pwr_valuesmall6.dd.setActionType(0);
    pwr_valuesmall6.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall6.dd, "$object.CycleTimeBus##Float32","%f")
      });
    pwr_valuelong7.dd.setDynType(1025);
    pwr_valuelong7.dd.setActionType(0);
    pwr_valuelong7.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong7.dd, "$node.ProcStatus[4]##Status","%1m")
      });
    pwr_valuelong8.dd.setDynType(1025);
    pwr_valuelong8.dd.setActionType(0);
    pwr_valuelong8.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong8.dd, "$node.ProcMessage[4]##String80","%s")
      });
    pwr_indsquare10.dd.setDynType(4);
    pwr_indsquare10.dd.setActionType(0);
    pwr_indsquare10.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare10.dd, "$object.IOReadWriteFlag##Boolean",29)
      });
    pwr_indsquare12.dd.setDynType(4);
    pwr_indsquare12.dd.setActionType(0);
    pwr_indsquare12.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indsquare12.dd, "$object.IOSimulFlag##Boolean",29)
      });
    pwr_valuesmall16.dd.setDynType(1025);
    pwr_valuesmall16.dd.setActionType(0);
    pwr_valuesmall16.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall16.dd, "$object.DiCount##UInt32","%d")
      });
    pwr_valuesmall18.dd.setDynType(1025);
    pwr_valuesmall18.dd.setActionType(0);
    pwr_valuesmall18.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall18.dd, "$object.DoCount##UInt32","%d")
      });
    pwr_valuesmall20.dd.setDynType(1025);
    pwr_valuesmall20.dd.setActionType(0);
    pwr_valuesmall20.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall20.dd, "$object.DvCount##UInt32","%d")
      });
    pwr_valuesmall22.dd.setDynType(1025);
    pwr_valuesmall22.dd.setActionType(0);
    pwr_valuesmall22.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall22.dd, "$object.AiCount##UInt32","%d")
      });
    pwr_valuesmall24.dd.setDynType(1025);
    pwr_valuesmall24.dd.setActionType(0);
    pwr_valuesmall24.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall24.dd, "$object.AoCount##UInt32","%d")
      });
    pwr_valuesmall26.dd.setDynType(1025);
    pwr_valuesmall26.dd.setActionType(0);
    pwr_valuesmall26.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall26.dd, "$object.AvCount##UInt32","%d")
      });
    pwr_valuesmall28.dd.setDynType(1025);
    pwr_valuesmall28.dd.setActionType(0);
    pwr_valuesmall28.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall28.dd, "$object.IiCount##UInt32","%d")
      });
    pwr_valuesmall30.dd.setDynType(1025);
    pwr_valuesmall30.dd.setActionType(0);
    pwr_valuesmall30.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall30.dd, "$object.IoCount##UInt32","%d")
      });
    pwr_valuesmall32.dd.setDynType(1025);
    pwr_valuesmall32.dd.setActionType(0);
    pwr_valuesmall32.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall32.dd, "$object.IvCount##UInt32","%d")
      });
    pwr_valuesmall34.dd.setDynType(1025);
    pwr_valuesmall34.dd.setActionType(0);
    pwr_valuesmall34.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall34.dd, "$object.CoCount##UInt32","%d")
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
  public int original_width = 704;
  public int original_height = 234;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Line2D.Float( 683.935F, 152.161F, 15.5484F, 152.161F),
    new Rectangle2D.Float(15.5484F, 129.581F, 669.417F, 93.4525F),
    new Polygon( new int[] { 16, 685, 684, 16, 16, 16}, new int[] { 130, 130, 131, 131, 222, 223}, 6), 
    new Polygon( new int[] { 685, 685, 684, 684, 16, 16}, new int[] { 223, 130, 131, 222, 222, 223}, 6), 
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
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Status",17, 89);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "LogMessage",17, 111);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "CycleTime",17, 23);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "IOReadWriteFlag",17, 48);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "IOSimulFlag",17, 68);
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[3]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Number of signals",26, 147);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Di",26, 172);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Do",26, 193);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Dv",26, 213);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Ai",177, 172);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Ao",177, 193);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Av",177, 213);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Ii",333, 172);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Io",333, 193);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Iv",333, 213);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( "Co",490, 172);
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
    size = new Dimension( 604, 19);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 604;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 600.645F, 15.8065F),
    new Polygon( new int[] { 2, 603, 602, 3, 3, 2}, new int[] { 2, 2, 3, 3, 17, 18}, 6), 
    new Polygon( new int[] { 603, 603, 602, 602, 3, 2}, new int[] { 18, 2, 3, 17, 17, 18}, 6), 
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
      g.drawString( annot1, 8 * original_height / height * width / original_width, 15F);
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
  Dimension size;
  public pwr_indsquare( JopSession session)
  {
    super( session);
    size = new Dimension( 17, 18);
  }
  public int original_width = 17;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 13.5484F, 14.6774F),
    new Polygon( new int[] { 2, 16, 14, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 17}, 6), 
    new Polygon( new int[] { 16, 16, 14, 14, 4, 2}, new int[] { 17, 2, 4, 15, 15, 17}, 6), 
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
protected class pwr_valuesmall extends GeComponent {
  Dimension size;
  public pwr_valuesmall( JopSession session)
  {
    super( session);
    size = new Dimension( 55, 19);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 55;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 51.9355F, 15.8065F),
    new Polygon( new int[] { 2, 54, 53, 3, 3, 2}, new int[] { 2, 2, 3, 3, 17, 18}, 6), 
    new Polygon( new int[] { 54, 54, 53, 53, 3, 2}, new int[] { 18, 2, 3, 17, 17, 18}, 6), 
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
      g.drawString( annot1, 8 * original_height / height * width / original_width, 15F);
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
