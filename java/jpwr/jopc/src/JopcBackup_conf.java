package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcBackup_conf extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_indsquare	pwr_indsquare3;
  pwr_indsquare	pwr_indsquare4;
  pwr_valuelong	pwr_valuelong7;
  pwr_valuelong	pwr_valuelong8;
  pwr_valuelong	pwr_valuelong9;
  pwr_valuesmall	pwr_valuesmall12;
  pwr_valuesmall	pwr_valuesmall14;
  pwr_valuesmall	pwr_valuesmall16;
  pwr_valuesmall	pwr_valuesmall18;
  pwr_valuesmall	pwr_valuesmall23;
  pwr_valuesmall	pwr_valuesmall25;
  pwr_valuesmall	pwr_valuesmall27;
  pwr_valuesmall	pwr_valuesmall29;
  pwr_indsquare	pwr_indsquare31;
  pwr_valuelong	pwr_valuelong32;
  pwr_valuesmall	pwr_valuesmall34;
  public JopcBackup_conf( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcBackup_conf( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "Œ-i@€l@œá@pïÿ¿8ïÿ¿ß}");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 612, 305);
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
      this.setTitle("JopcBackup_conf");
    pwr_indsquare3 = new pwr_indsquare(session);
    pwr_indsquare3.setBounds(new Rectangle(11,160,18,19));
    localPanel.add(pwr_indsquare3, new Proportion(pwr_indsquare3.getBounds(), dsize));
    pwr_indsquare4 = new pwr_indsquare(session);
    pwr_indsquare4.setBounds(new Rectangle(11,182,18,19));
    localPanel.add(pwr_indsquare4, new Proportion(pwr_indsquare4.getBounds(), dsize));
    pwr_valuelong7 = new pwr_valuelong(session);
    pwr_valuelong7.setBounds(new Rectangle(30,159,561,20));
    pwr_valuelong7.setBorderColor(33);
    localPanel.add(pwr_valuelong7, new Proportion(pwr_valuelong7.getBounds(), dsize));
    pwr_valuelong8 = new pwr_valuelong(session);
    pwr_valuelong8.setBounds(new Rectangle(30,182,561,20));
    pwr_valuelong8.setBorderColor(33);
    localPanel.add(pwr_valuelong8, new Proportion(pwr_valuelong8.getBounds(), dsize));
    pwr_valuelong9 = new pwr_valuelong(session);
    pwr_valuelong9.setBounds(new Rectangle(10,5,579,25));
    pwr_valuelong9.setBorderColor(33);
    localPanel.add(pwr_valuelong9, new Proportion(pwr_valuelong9.getBounds(), dsize));
    pwr_valuesmall12 = new pwr_valuesmall(session);
    pwr_valuesmall12.setBounds(new Rectangle(109,63,105,20));
    pwr_valuesmall12.setFillColor(31);
    pwr_valuesmall12.setBorderColor(33);
    localPanel.add(pwr_valuesmall12, new Proportion(pwr_valuesmall12.getBounds(), dsize));
    pwr_valuesmall14 = new pwr_valuesmall(session);
    pwr_valuesmall14.setBounds(new Rectangle(109,83,105,20));
    pwr_valuesmall14.setFillColor(31);
    pwr_valuesmall14.setBorderColor(33);
    localPanel.add(pwr_valuesmall14, new Proportion(pwr_valuesmall14.getBounds(), dsize));
    pwr_valuesmall16 = new pwr_valuesmall(session);
    pwr_valuesmall16.setBounds(new Rectangle(109,104,105,20));
    pwr_valuesmall16.setFillColor(31);
    pwr_valuesmall16.setBorderColor(33);
    localPanel.add(pwr_valuesmall16, new Proportion(pwr_valuesmall16.getBounds(), dsize));
    pwr_valuesmall18 = new pwr_valuesmall(session);
    pwr_valuesmall18.setBounds(new Rectangle(109,126,173,20));
    pwr_valuesmall18.setFillColor(31);
    pwr_valuesmall18.setBorderColor(33);
    localPanel.add(pwr_valuesmall18, new Proportion(pwr_valuesmall18.getBounds(), dsize));
    pwr_valuesmall23 = new pwr_valuesmall(session);
    pwr_valuesmall23.setBounds(new Rectangle(406,61,110,20));
    pwr_valuesmall23.setFillColor(31);
    pwr_valuesmall23.setBorderColor(33);
    localPanel.add(pwr_valuesmall23, new Proportion(pwr_valuesmall23.getBounds(), dsize));
    pwr_valuesmall25 = new pwr_valuesmall(session);
    pwr_valuesmall25.setBounds(new Rectangle(406,82,110,20));
    pwr_valuesmall25.setFillColor(31);
    pwr_valuesmall25.setBorderColor(33);
    localPanel.add(pwr_valuesmall25, new Proportion(pwr_valuesmall25.getBounds(), dsize));
    pwr_valuesmall27 = new pwr_valuesmall(session);
    pwr_valuesmall27.setBounds(new Rectangle(406,103,110,20));
    pwr_valuesmall27.setFillColor(31);
    pwr_valuesmall27.setBorderColor(33);
    localPanel.add(pwr_valuesmall27, new Proportion(pwr_valuesmall27.getBounds(), dsize));
    pwr_valuesmall29 = new pwr_valuesmall(session);
    pwr_valuesmall29.setBounds(new Rectangle(406,125,180,20));
    pwr_valuesmall29.setFillColor(31);
    pwr_valuesmall29.setBorderColor(33);
    localPanel.add(pwr_valuesmall29, new Proportion(pwr_valuesmall29.getBounds(), dsize));
    pwr_indsquare31 = new pwr_indsquare(session);
    pwr_indsquare31.setBounds(new Rectangle(11,232,18,19));
    localPanel.add(pwr_indsquare31, new Proportion(pwr_indsquare31.getBounds(), dsize));
    pwr_valuelong32 = new pwr_valuelong(session);
    pwr_valuelong32.setBounds(new Rectangle(30,230,561,20));
    pwr_valuelong32.setBorderColor(33);
    localPanel.add(pwr_valuelong32, new Proportion(pwr_valuelong32.getBounds(), dsize));
    pwr_valuesmall34 = new pwr_valuesmall(session);
    pwr_valuesmall34.setBounds(new Rectangle(10,207,229,20));
    pwr_valuesmall34.setFillColor(31);
    pwr_valuesmall34.setBorderColor(33);
    localPanel.add(pwr_valuesmall34, new Proportion(pwr_valuesmall34.getBounds(), dsize));
    pwr_indsquare3.dd.setDynType(33554432);
    pwr_indsquare3.dd.setActionType(0);
    pwr_indsquare3.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare3.dd, "$node.ProcStatus[8]##status",38)
      });
    pwr_indsquare4.dd.setDynType(33554432);
    pwr_indsquare4.dd.setActionType(0);
    pwr_indsquare4.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare4.dd, "$node.ProcMsgSeverity[8]##status",38)
      });
    pwr_valuelong7.dd.setDynType(1025);
    pwr_valuelong7.dd.setActionType(0);
    pwr_valuelong7.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong7.dd, "$node.ProcStatus[8]##Status","%1m")
      });
    pwr_valuelong8.dd.setDynType(1025);
    pwr_valuelong8.dd.setActionType(0);
    pwr_valuelong8.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong8.dd, "$node.ProcMessage[8]##String80","%s")
      });
    pwr_valuelong9.dd.setDynType(1025);
    pwr_valuelong9.dd.setActionType(0);
    pwr_valuelong9.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong9.dd, "$object.BackupFile##String80","%s")
      });
    pwr_valuesmall12.dd.setDynType(1025);
    pwr_valuesmall12.dd.setActionType(0);
    pwr_valuesmall12.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall12.dd, "$object.CycleSlow##UInt32","%d")
      });
    pwr_valuesmall14.dd.setDynType(1025);
    pwr_valuesmall14.dd.setActionType(0);
    pwr_valuesmall14.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall14.dd, "$object.CntSlow##UInt32","%d")
      });
    pwr_valuesmall16.dd.setDynType(1025);
    pwr_valuesmall16.dd.setActionType(0);
    pwr_valuesmall16.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall16.dd, "$object.SegSlow##UInt32","%d")
      });
    pwr_valuesmall18.dd.setDynType(1025);
    pwr_valuesmall18.dd.setActionType(0);
    pwr_valuesmall18.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall18.dd, "$object.ObjTimeSlow##Time","%t")
      });
    pwr_valuesmall23.dd.setDynType(1025);
    pwr_valuesmall23.dd.setActionType(0);
    pwr_valuesmall23.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall23.dd, "$object.CycleFast##UInt32","%d")
      });
    pwr_valuesmall25.dd.setDynType(1025);
    pwr_valuesmall25.dd.setActionType(0);
    pwr_valuesmall25.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall25.dd, "$object.CntFast##UInt32","%d")
      });
    pwr_valuesmall27.dd.setDynType(1025);
    pwr_valuesmall27.dd.setActionType(0);
    pwr_valuesmall27.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall27.dd, "$object.SegFast##UInt32","%d")
      });
    pwr_valuesmall29.dd.setDynType(1025);
    pwr_valuesmall29.dd.setActionType(0);
    pwr_valuesmall29.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall29.dd, "$object.ObjTimeFast##Time","%t")
      });
    pwr_indsquare31.dd.setDynType(33554432);
    pwr_indsquare31.dd.setActionType(0);
    pwr_indsquare31.dd.setElements(new GeDynElemIfc[] {
      new GeDynStatusColor(pwr_indsquare31.dd, "$object.DiskStatus##status",38)
      });
    pwr_valuelong32.dd.setDynType(1025);
    pwr_valuelong32.dd.setActionType(0);
    pwr_valuelong32.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuelong32.dd, "$object.DiskStatus##Status","%1m")
      });
    pwr_valuesmall34.dd.setDynType(1025);
    pwr_valuesmall34.dd.setActionType(0);
    pwr_valuesmall34.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall34.dd, "$object.LastWrite##Time","%t")
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
  public int original_width = 604;
  public int original_height = 261;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Line2D.Float( 285.325F, 60.4726F, 11.5238F, 60.4726F),
    new Rectangle2D.Float(11.5238F, 37.7143F, 273.802F, 118.059F),
    new Polygon( new int[] { 12, 285, 284, 13, 13, 12}, new int[] { 38, 38, 39, 39, 155, 156}, 6), 
    new Polygon( new int[] { 285, 285, 284, 284, 13, 12}, new int[] { 156, 38, 39, 155, 155, 156}, 6), 
    new Line2D.Float( 589.069F, 60.4726F, 301.999F, 60.4726F),
    new Rectangle2D.Float(301.999F, 37.7143F, 287.07F, 118.059F),
    new Polygon( new int[] { 302, 589, 588, 303, 303, 302}, new int[] { 38, 38, 39, 39, 155, 156}, 6), 
    new Polygon( new int[] { 589, 589, 588, 588, 303, 302}, new int[] { 156, 38, 39, 155, 155, 156}, 6), 
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
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[0]);
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[3]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Status",-113, 174);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "LogMessage",-113, 197);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "CycleSlow",16, 53);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "BackupFile",-113, 23);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "CycleTime",16, 76);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "LoopCount",16, 96);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Objects",16, 118);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "UpdateTime",16, 139);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[7]);
    }
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "CycleFast",307, 53);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "CycleTime",307, 76);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "LoopCount",307, 96);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Objects",307, 118);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "UpdateTime",307, 139);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "DiskStatus",-113, 245);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "LastWrite",-113, 221);
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
    size = new Dimension( 637, 20);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 637;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 633.333F, 16.6667F),
    new Polygon( new int[] { 2, 635, 635, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 635, 635, 635, 635, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
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
    size = new Dimension( 18, 19);
  }
  public int original_width = 18;
  public int original_height = 19;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 14.2857F, 15.4762F),
    new Polygon( new int[] { 2, 16, 14, 4, 4, 2}, new int[] { 2, 2, 4, 4, 15, 17}, 6), 
    new Polygon( new int[] { 16, 16, 14, 14, 4, 2}, new int[] { 17, 2, 4, 15, 15, 17}, 6), 
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
    int fcolor = GeColor.getDrawtype(293, colorTone,
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
    size = new Dimension( 58, 20);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 58;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 54.7619F, 16.6667F),
    new Polygon( new int[] { 2, 57, 56, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 57, 57, 56, 56, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
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
    return utilityObjid;
  }
  public String getUtilityName() {
    return this.getClass().getName();
  }
}
