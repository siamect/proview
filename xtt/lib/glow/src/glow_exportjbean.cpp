#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growctx.h"
#include "glow_grownode.h"
#include "glow_exportjbean.h"

#define glow_cJBean_Offset 2

static char *str_cnv( char *instr);

void GlowExportJBean::growctx( glow_eExportPass pass, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;

  switch ( pass)
  {
    case glow_eExportPass_Declare:
      break;
    case glow_eExportPass_Init:
    {
      ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<

"  int fillColor = 9999;" << endl <<
"  int originalFillColor = 9999;" << endl <<
"  int textColor = 9999;" << endl <<
"  int originalTextColor = 9999;" << endl <<
"  int borderColor = 9999;" << endl <<
"  int colorTone = 0;" << endl <<
"  int originalColorTone = 0;" << endl <<
"  int colorShift = 0;" << endl <<
"  int originalColorShift = 0;" << endl <<
"  int colorBrightness = 0;" << endl <<
"  int originalColorBrightness = 0;" << endl <<
"  int colorIntensity = 0;" << endl <<
"  int originalColorIntensity = 0;" << endl <<
"  int colorInverse = 0;" << endl <<
"  int originalColorInverse = 0;" << endl <<
"  int shadow = 0;" << endl <<
"  public void setColorTone( int colorTone) {" << endl <<
"    this.colorTone = colorTone;" << endl <<
"    originalColorTone = colorTone;" << endl <<
"  }" << endl <<
"  public int getColorTone() {" << endl <<
"    return colorTone;" << endl <<
"  }" << endl <<
"  public void setColorShift( int colorShift) {" << endl <<
"    this.colorShift = colorShift;" << endl <<
"    originalColorShift = colorShift;" << endl <<
"  }" << endl <<
"  public int getColorShift() {" << endl <<
"    return colorShift;" << endl <<
"  }" << endl <<
"  public void setColorBrightness( int colorBrightness) {" << endl <<
"    this.colorBrightness = colorBrightness;" << endl <<
"    originalColorBrightness = colorBrightness;" << endl <<
"  }" << endl <<
"  public int getColorBrightness() {" << endl <<
"    return colorBrightness;" << endl <<
"  }" << endl <<
"  public void setColorIntensity( int colorIntensity) {" << endl <<
"    this.colorIntensity = colorIntensity;" << endl <<
"    originalColorIntensity = colorIntensity;" << endl <<
"  }" << endl <<
"  public int getColorIntensity() {" << endl <<
"    return colorIntensity;" << endl <<
"  }" << endl <<
"  public void setFillColor( int fillColor) {" << endl <<
"    this.fillColor = fillColor;" << endl <<
"    this.originalFillColor = fillColor;" << endl <<
"  }" << endl <<
"  public void resetFillColor() {" << endl <<
"    fillColor = originalFillColor;" << endl <<
"  }" << endl <<
"  public int getFillColor() {" << endl <<
"    return fillColor;" << endl <<
"  }" << endl <<
"  public void setBorderColor( int borderColor) {" << endl <<
"    this.borderColor = borderColor;" << endl <<
"  }" << endl <<
"  public int getBorderColor() {" << endl <<
"    return borderColor;" << endl <<
"  }" << endl <<
"  int original_width = " << int( dim_x1 - dim_x0) + 2 * glow_cJBean_Offset << ";" << endl <<
"  int original_height = " << int( dim_y1 - dim_y0) + 2 * glow_cJBean_Offset<< ";" << endl <<
"  double rotate;" << endl <<
"  public void setRotate( double rotate) {" << endl <<
"    if ( rotate < 0)" << endl <<
"      this.rotate = rotate % 360 + 360;" << endl <<
"    else" << endl <<
"      this.rotate = rotate % 360;" << endl <<
"  }" << endl <<
"  public double getRotate() { return rotate;}" << endl <<
"  Shape[] shapes = new Shape[] { " << endl;
      break;
    }
    case glow_eExportPass_Shape:
    {
      fp <<

"  };" << endl;

      break;
    }
    case glow_eExportPass_Attributes:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      func_cnt = 0;
      fp <<

"  public void paint(Graphics g1) {" << endl <<
"    Graphics2D g = (Graphics2D) g1;" << endl <<
"    Component c;" << endl <<
"    Point p;" << endl <<
"    paintComponent(g);" << endl <<
"    for ( int i = 0; i < getComponentCount(); i++) {" << endl <<
"      AffineTransform save = g.getTransform();" << endl <<
"      c = getComponent(i);" << endl <<
"      p = c.getLocation();" << endl <<
"      g.translate((int)p.getX(), (int)p.getY());" << endl <<
"      c.paint(g);" << endl <<
"      g.setTransform(save);" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public void paintComponent(Graphics g1) {" << endl <<
"    Graphics2D g = (Graphics2D) g1;" << endl <<
"    float width = getWidth();" << endl <<
"    float height = getHeight();" << endl <<
"    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);" << endl;

      if ( ((GrowCtx *)ctx)->subgraph)
        fp <<
"    if ( 45.0 <= rotate && rotate < 135.0) {" << endl <<
"      g.translate( width, 0.0); " << endl <<
"      g.rotate( Math.PI * rotate/180, 0.0, 0.0);" << endl <<
"      g.transform( AffineTransform.getScaleInstance( height/original_width," << endl <<
"      		width/original_height));" << endl <<
"    }" << endl <<
"    else if ( 135.0 <= rotate && rotate < 225.0)" << endl <<
"    {" << endl <<
"      g.rotate( Math.PI * rotate/180, width/2, height/2);" << endl <<
"      g.transform( AffineTransform.getScaleInstance( width/original_width," << endl <<
"      		height/original_height));" << endl <<
"    }" << endl <<
"    else if ( 225.0 <= rotate && rotate < 315.0)" << endl <<
"    {" << endl <<
"      g.translate( -height, 0.0);" << endl <<
"      g.rotate( Math.PI * rotate/180, height, 0.0);" << endl <<
"      g.transform( AffineTransform.getScaleInstance( height/original_width," << endl <<
"      		width/original_height));" << endl <<
"    }" << endl <<
"    else" << endl <<
"      g.transform( AffineTransform.getScaleInstance( width/original_width," << endl <<
"      		height/original_height));" << endl;
      else
        fp <<
"//    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight));" << endl <<
"    AffineTransform save = g.getTransform();" << endl <<
"    AffineTransform save_tmp;" << endl <<
"    g.setColor(getBackground());" << endl <<
"    g.fill(new Rectangle(0,0,getWidth(),getHeight()));" << endl;
      if ( strcmp( ((GrowCtx *)ctx)->background_image, "") != 0)
      {
	fp <<
"    if ( backgroundImage.mImage == null)" << endl <<
"      backgroundImage.paintComponent( g);" << endl <<
"    TexturePaint imagePaint = new TexturePaint( backgroundImage.mImage," << endl <<
"        new Rectangle(0,0, backgroundImage.getWidth(), backgroundImage.getHeight()));" << endl <<
"    g.setPaint( imagePaint);" << endl <<
"    g.fill( new Rectangle(0,0, getWidth(), getHeight()));" << endl <<
"    g.draw( new Rectangle(0,0, getWidth(), getHeight()));" << endl;
      }
      break;
    }
    case glow_eExportPass_End:
    {
      fp <<
"    g.setTransform(save);" << endl <<
"  }" << endl;

      break;
    }
    default: ;
  }
}

void GlowExportJBean::nodeclass( GlowNodeClass *nc, glow_eExportPass pass, 
	ofstream& fp, int page, int pages)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  int i;

  switch ( pass)
  {
    case glow_eExportPass_Declare:
      break;
    case glow_eExportPass_Init:
    {
      nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      if ( page <= 1)
      {
        fp <<

"  int original_width = " << int( dim_x1 - dim_x0) + 2 * glow_cJBean_Offset << ";" << endl <<
"  int original_height = " << int( dim_y1 - dim_y0) + 2 * glow_cJBean_Offset << ";" << endl <<
"  Shape[] shapes = new Shape[] { " << endl;
      }
      else
      {
        fp <<

"  int original_width_p" << page <<  " = " << int( dim_x1 - dim_x0) + 2 * glow_cJBean_Offset << ";" << endl <<
"  int original_height_p" << page << " = " << int( dim_y1 - dim_y0) + 2 * glow_cJBean_Offset << ";" << endl <<
"  Shape[] shapes_p" << page << " = new Shape[] { " << endl;
      }

      break;
    }
    case glow_eExportPass_Shape:
    {
      fp <<

"  };" << endl;

      break;
    }
    case glow_eExportPass_Attributes:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      char original_width[20];
      char original_height[20];
//    if ( page <= 1)
      {
        strcpy( original_width, "original_width");
        strcpy( original_height, "original_height");
      }
//    else
//    {
//      sprintf( original_width, "original_width_p%d", page);
//      sprintf( original_height, "original_height_p%d", page);
//    }
      if ( page <= 1)
      {
        fp <<

"  public void paintComponent(Graphics g1) {" << endl;
        if  ( pages > page)
        {
          fp <<
"    switch ( page) {" << endl;
          for ( i = 2; i <= pages; i++)
          {
            fp <<
"      case " << i << " :" << endl <<
"        paintComponent_p" << i << "(g1);" << endl <<
"        return;" << endl;
          }
          fp <<
"      default:" << endl <<
"        ;" << endl <<
"    }" << endl; 
        }
      }
      else
      {
        fp <<
"  public void paintComponent_p" << page << "(Graphics g1) {" << endl;
      }
      fp <<
"    animationCount = " << nc->animation_count << ";" << endl <<
"    if ( dd.invisible) {" << endl <<
"      if ( !dd.invisibleOld) {" << endl <<
"        dd.invisibleOld = dd.invisible;" << endl <<
"        repaint();" << endl <<
"      }" << endl <<
"      else" << endl <<
"        dd.invisibleOld = dd.invisible;" << endl <<
"      return;" << endl <<
"    }" << endl <<
"    dd.invisibleOld = dd.invisible;" << endl <<
"    Graphics2D g = (Graphics2D) g1;" << endl <<
"    float width = getWidth();" << endl <<
"    float height = getHeight();" << endl <<
"    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);" << endl <<
"    if ( 45.0 <= rotate && rotate < 135.0) {" << endl <<
"      g.translate( width, 0.0); " << endl <<
"      g.rotate( Math.PI * rotate/180, 0.0, 0.0);" << endl <<
"      g.transform( AffineTransform.getScaleInstance( height/" << original_width << "," << endl <<
"      		width/" << original_height << "));" << endl <<
"    }" << endl <<
"    else if ( 135.0 <= rotate && rotate < 225.0)" << endl <<
"    {" << endl <<
"      g.rotate( Math.PI * rotate/180, width/2, height/2);" << endl <<
"      g.transform( AffineTransform.getScaleInstance( width/" << original_width << "," << endl <<
"      		height/" << original_height << "));" << endl <<
"    }" << endl <<
"    else if ( 225.0 <= rotate && rotate < 315.0)" << endl <<
"    {" << endl <<
"      g.translate( -height, 0.0);" << endl <<
"      g.rotate( Math.PI * rotate/180, height, 0.0);" << endl <<
"      g.transform( AffineTransform.getScaleInstance( height/" << original_width << "," << endl <<
"      		width/" << original_height << "));" << endl <<
"    }" << endl <<
"    else" << endl <<
"      g.transform( AffineTransform.getScaleInstance( width/" << original_width << "," << endl <<
"      		height/" << original_height << "));" << endl <<
"    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {" << endl <<
"      g.rotate( Math.PI * dd.rotate/180, " << endl << endl <<
"           (dd.x0 - getX())*original_width/width,"<< endl <<
"           (dd.y0 - getY()) * original_height / height);" << endl <<
"    }" << endl <<
"    AffineTransform save = g.getTransform();" << endl <<
"    AffineTransform save_tmp;" << endl <<
"    int rounds = 1;" << endl <<
"    if ( fillLevel != 1F)" << endl <<
"      rounds = 2;" << endl <<
"    int oldColor = 0;" << endl <<
"    for ( int i = 0; i < rounds; i++) {" << endl <<
"      if ( rounds == 2) {" << endl <<
"        switch ( i) {" << endl <<
"        case 0:" << endl <<
"	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {" << endl <<
"	    oldColor = colorTone;" << endl <<
"	    colorTone = levelColorTone;" << endl <<
"	  }" << endl <<
"	  else if ( levelFillColor != GeColor.COLOR_NO) {" << endl <<
"	    oldColor = fillColor;" << endl <<
"	    fillColor = levelFillColor;" << endl <<
"	  }" << endl <<
"	  break;" << endl <<
"        case 1:" << endl <<
"	  colorTone = oldColor;" << endl <<
"	  break;" << endl <<
"        }" << endl <<
"        switch ( levelDirection) {" << endl <<
"        case Ge.DIRECTION_UP:" << endl <<
"	  if ( i == 0)" << endl <<
"	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));" << endl <<
"	  else" << endl <<
"	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));" << endl <<	  
"	  break;" << endl <<
"	case Ge.DIRECTION_DOWN:" << endl <<
"	  if ( i == 0)" << endl <<
"	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));" << endl <<
"	  else" << endl <<
"	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));" << endl <<
"	  break;" << endl <<
"	case Ge.DIRECTION_RIGHT:" << endl <<
"	  if ( i == 0)" << endl <<
"	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));" << endl <<
"	  else" << endl <<
"	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));" << endl <<
"	  break;" << endl <<
"	case Ge.DIRECTION_LEFT:" << endl <<
"	  if ( i == 0)" << endl <<
"	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));" << endl <<
"	  else" << endl <<
"	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));" << endl <<
"	  break;" << endl <<
"	}" << endl <<
"      }" << endl;

      break;
    }
    case glow_eExportPass_End:
    {
      fp <<
"    }" << endl <<
"    if ( rounds == 2)" << endl <<
"      g.setClip(null);" << endl <<
"    g.setTransform(save);" << endl <<
"  }" << endl;

      break;
    }
    default: ;
  }
}

void GlowExportJBean::polyline( glow_sPoint *points, int point_cnt, int fill, int border, 
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int fill_eq_border, int fill_eq_light, int fill_eq_shadow, int line_width, 
        int print_shadow, int shadow, int drawtype_incr, glow_sShadowInfo *sp, int sp_num, 
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  int i;
  double dim_x0, dim_x1, dim_y0, dim_y1;
  double x, y;

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      if ( fill || 
           ( points[0].x == points[point_cnt-1].x && 
	     points[0].y == points[point_cnt-1].y)) 
      {
        fp << 
"    new Polygon( new int[] { ";
        for ( i = 0; i < point_cnt; i++)
        {
          x = points[i].x - dim_x0 + glow_cJBean_Offset;
          fp << int(x + 0.5);
          if ( i != point_cnt - 1)
            fp << ", ";
        }
        fp << "}, new int[] {";
        for ( i = 0; i < point_cnt; i++)
        {
          y = points[i].y - dim_y0 + glow_cJBean_Offset;
          fp << int(y + 0.5);
          if ( i != point_cnt - 1)
            fp << ", ";
        }
        fp << "}, " << point_cnt << ")," << endl;
      }
      else {
        // Java polygon i always closed, go back int your own footprints
        fp << 
"    new Polygon( new int[] { ";
        for ( i = 0; i < point_cnt; i++)
        {
          x = points[i].x - dim_x0 + glow_cJBean_Offset;
          fp << int(x + 0.5);
          fp << ", ";
        }
        for ( i = point_cnt - 2; i >= 1; i--)
        {
          x = points[i].x - dim_x0 + glow_cJBean_Offset;
          fp << int(x + 0.5);
          if ( i != 1)
            fp << ", ";
        }
        fp << "}, new int[] {";
        for ( i = 0; i < point_cnt; i++)
        {
          y = points[i].y - dim_y0 + glow_cJBean_Offset;
          fp << int(y + 0.5);
          fp << ", ";
        }
        for ( i = point_cnt - 2; i >= 1; i--)
        {
          y = points[i].y - dim_y0 + glow_cJBean_Offset;
          fp << int(y + 0.5);
          if ( i != 1)
            fp << ", ";
        }
        fp << "}, " << 2 * point_cnt - 2 << ")," << endl;
      }
      if ( print_shadow) {
	for ( i = 0; i < sp_num - 1; i++) {
	  fp << 
"    new Polygon( new int[] { ";
          fp << int(points[i].x - dim_x0 + glow_cJBean_Offset + 0.5) << ", " <<
	    int(sp[i].x - dim_x0 + glow_cJBean_Offset + 0.5) << ", " <<
	    int(sp[i+1].x - dim_x0 + glow_cJBean_Offset + 0.5) << ", " <<
	    int(points[i+1].x - dim_x0 + glow_cJBean_Offset + 0.5) << "}, " <<
	    "new int[] { "<<
	    int(points[i].y - dim_y0 + glow_cJBean_Offset + 0.5) << ", " <<
	    int(sp[i].y - dim_y0 + glow_cJBean_Offset + 0.5) << ", " <<
	    int(sp[i+1].y - dim_y0 + glow_cJBean_Offset + 0.5) << "," <<
	    int(points[i+1].y - dim_y0 + glow_cJBean_Offset + 0.5) << "}, 4)," << endl;
	}
      }
      break;
    }
    case glow_eExportPass_Init:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      int  base_shape = *shape_cnt;
      if ( fill) {
        if ( fill_eq_border)
          fp <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
	else if ( fill_eq_light) {
          fp <<
"    if ( shadow != 0)" << endl <<
"      g.setColor(GeColor.shiftColor(" << (int)fill_drawtype << "," << -drawtype_incr << ", colorInverse));" << endl <<
"    else" << endl <<
"      g.setColor(GeColor.getColor(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));" << endl;
	}
	else if ( fill_eq_shadow) {
          fp <<
"    if ( shadow != 0)" << endl <<
"      g.setColor(GeColor.shiftColor(" <<  (int)fill_drawtype << "," << drawtype_incr << ", colorInverse));" << endl <<
"    else" << endl <<
"      g.setColor(GeColor.getColor(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));" << endl;
	}
	else
          fp <<
"    g.setColor(GeColor.getColor(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));" << endl;

        if ( page <= 1)
          fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
        else
          fp <<
"    g.fill( shapes_p" << page <<"[" << (*shape_cnt)++ << "]);" << endl;
      }
      else
	(*shape_cnt)++;
      if ( print_shadow) {
	fp <<
"    {" << endl <<  // Avoid multiple declarations of fcolor
"    int fcolor = GeColor.getDrawtype(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);" << endl;

	if ( !shadow)
	  // Shadow is choosable at runtime
	  fp <<
"    if ( shadow != 0) {" << endl;

	for ( i = 0; i < sp_num - 1; i++) {
          fp <<
"    g.setColor(GeColor.shiftColor( fcolor, " << (int)sp[i].drawtype << ", colorInverse));" << endl;
        if ( page <= 1)
          fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
        else
          fp <<
"    g.fill( shapes_p" << page <<"[" << (*shape_cnt)++ << "]);" << endl;
	}

	if ( !shadow)
	  fp <<
"    }" << endl;
	fp <<
"    }" << endl;
      }
      if (border) {
        fp <<
"    g.setStroke( new BasicStroke(" << line_width+1 << "F));" << endl <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
        if ( page <= 1)
          fp <<
"    g.draw( shapes[" << base_shape << "]);" << endl;
        else
          fp <<
"    g.draw( shapes_p" << page << "[" << base_shape << "]);" << endl;
      }
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::line( double x1, double y1, double x2, double y2,
	glow_eDrawType border_drawtype,
	int line_width, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp << 
"    new Line2D.Float( " << x1 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y1 - dim_y0 + glow_cJBean_Offset << "F, " <<
	x2 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y2 - dim_y0 + glow_cJBean_Offset << "F)," << endl;
      break;
    }
    case glow_eExportPass_Init:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      fp <<
"    g.setStroke( new BasicStroke(" << line_width+1 << "F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));" << endl <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
      if ( page <= 1)
        fp <<
"    g.draw( shapes[" << *shape_cnt << "]);" << endl;
      else
        fp <<
"    g.draw( shapes_p" << page << "[" << *shape_cnt << "]);" << endl;

      if ( *shape_cnt / 1000 > func_cnt) {
	// Max number of bytes in a java function is ~65000
	fp <<
"    paintComponent" << func_cnt << "( g, save);" << endl <<
"  }" << endl <<
"  void paintComponent" << func_cnt << "( Graphics2D g, AffineTransform save) {" << endl;
	func_cnt++;
      }
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::rect( double x0, double y0, double width, double height,
	int fill, int border,
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int line_width, double shadow_width, int shadow, 
	int drawtype_incr, glow_eExportPass pass, int *shape_cnt, 
        int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp << 
"    new Rectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y0 - dim_y0 + glow_cJBean_Offset << "F, " << 
	width << "F, " << height << "F)," << endl;
      if ( shadow_width != 0) {
	fp <<
"    new Polygon( new int[] { " <<
	  int( x0 - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + width - shadow_width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + shadow_width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + shadow_width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 - dim_x0 + glow_cJBean_Offset + 0.5) << "}, " << 
	  "new int[] { " <<
	  int( y0 - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + shadow_width - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + shadow_width - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + height - shadow_width - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + height - dim_y0 + glow_cJBean_Offset + 0.5) << "}, 6), " << endl <<
"    new Polygon( new int[] { " <<
	  int( x0 + width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + width - shadow_width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + width - shadow_width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 + shadow_width - dim_x0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( x0 - dim_x0 + glow_cJBean_Offset + 0.5) << "}, " << 
	  "new int[] { " <<
	  int( y0 + height - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + shadow_width - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + height - shadow_width - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + height - shadow_width - dim_y0 + glow_cJBean_Offset + 0.5) << ", " << 
	  int( y0 + height - dim_y0 + glow_cJBean_Offset + 0.5) << "}, 6), " << endl;
      }
      break;
    }
    case glow_eExportPass_Init:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      int base_shape = *shape_cnt;
      fp <<
"    {" << endl <<  // Avoid multiple declarations of fcolor
"    int fcolor = GeColor.getDrawtype(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);" << endl;
      if ( fill)
      {
        fp <<
"    g.setColor(GeColor.getColor( fcolor));" << endl;
        if ( page <= 1)
          fp <<
"    g.fill( shapes[" << *shape_cnt << "]);" << endl;
        else
          fp <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt << "]);" << endl;
      }
      (*shape_cnt)++;
      if (shadow_width != 0) {
	if ( !shadow)
	  fp <<
"    if ( shadow != 0) {" << endl;
        fp <<
	  "    g.setColor(GeColor.shiftColor( fcolor, " << -drawtype_incr << ", colorInverse));" << endl;
        if ( page <= 1)
          fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
        else
          fp <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;

        fp <<
	  "    g.setColor(GeColor.shiftColor( fcolor, " << drawtype_incr << ", colorInverse));" << endl;
        if ( page <= 1)
          fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
        else
          fp <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;
	if ( !shadow)
	  fp <<
"    }" << endl;
      }
      if (border) {
        fp <<
"    g.setStroke( new BasicStroke(" << line_width+1 << "F));" << endl <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
        if ( page <= 1)
          fp <<
"    g.draw( shapes[" << base_shape << "]);" << endl;
        else
          fp <<
"    g.draw( shapes_p" << page << "[" << base_shape << "]);" << endl;
      }
      fp <<
"    }" << endl;
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::rectrounded( double x0, double y0, double width, double height,
	int fill, int border,
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int line_width, double roundamount, double shadow_width, int shadow, 
	int drawtype_incr,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp << 
"    new RoundRectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y0 - dim_y0 + glow_cJBean_Offset << "F, " << 
	width << "F, " << height << "F, " << 2*roundamount << "F, " << 2*roundamount << "F)," << endl;
      if ( shadow_width != 0) {
	// Light shadow
	fp << 
"    new Rectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + roundamount << "F, " << 
	  y0 - dim_y0 + glow_cJBean_Offset << "F, " << 
	  width - 2*roundamount<< "F, " << shadow_width << "F)," << endl <<
"    new Rectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	  y0 - dim_y0 + glow_cJBean_Offset + roundamount << "F, " << 
	  shadow_width << "F, " << height - 2*roundamount << "F)," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	2*roundamount << "F, " << 2*roundamount << "F, " << 
	  90 << "F, " << 90 << "F, Arc2D.PIE)," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + width - 2*roundamount << "F, " << 
	y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	2*roundamount << "F, " << 2*roundamount << "F, " << 
	  45 << "F, " << 45<< "F, Arc2D.PIE)," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y0  - dim_y0 + glow_cJBean_Offset + height - 2*roundamount << "F, " << 
	2*roundamount << "F, " << 2*roundamount << "F, " << 
	  180 << "F, " << 45<< "F, Arc2D.PIE)," << endl;
	// Dark shadow
	fp << 
"    new Rectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + roundamount << "F, " << 
	  y0 - dim_y0 + glow_cJBean_Offset + height - shadow_width << "F, " << 
	  width - 2*roundamount<< "F, " << shadow_width << "F)," << endl <<
"    new Rectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + width - shadow_width << "F, " << 
	  y0 - dim_y0 + glow_cJBean_Offset + roundamount << "F, " << 
	  shadow_width << "F, " << height - 2*roundamount << "F)," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + width - 2*roundamount << "F, " << 
	  y0  - dim_y0 + glow_cJBean_Offset + height - 2*roundamount << "F, " << 
	2*roundamount << "F, " << 2*roundamount << "F, " << 
	  270 << "F, " << 90 << "F, Arc2D.PIE)," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + width - 2*roundamount << "F, " << 
	y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	2*roundamount << "F, " << 2*roundamount << "F, " << 
	  0 << "F, " << 45<< "F, Arc2D.PIE)," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y0  - dim_y0 + glow_cJBean_Offset + height - 2*roundamount << "F, " << 
	2*roundamount << "F, " << 2*roundamount << "F, " << 
	  225 << "F, " << 45<< "F, Arc2D.PIE)," << endl;
	if ( roundamount > shadow_width) {
	  fp << 
"    new RoundRectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + shadow_width << "F, " << 
	y0 - dim_y0 + glow_cJBean_Offset + shadow_width << "F, " << 
	width - 2*shadow_width << "F, " << height - 2*shadow_width << "F, " << 2*roundamount - 2*shadow_width << "F, " << 2*roundamount - 2*shadow_width << "F)," << endl;
	}
	else {
	  fp <<
"    new Rectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + shadow_width << "F, " << 
	y0 - dim_y0 + glow_cJBean_Offset + shadow_width << "F, " << 
	width - 2*shadow_width << "F, " << height - 2*shadow_width << "F)," << endl;
	}
	fp << 
"    new RoundRectangle2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y0 - dim_y0 + glow_cJBean_Offset << "F, " << 
	width << "F, " << height << "F, " << 2*roundamount << "F, " << 2*roundamount << "F)," << endl;
      }
      break;
    }
    case glow_eExportPass_Init:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      if ( fill) {
	fp <<
"    {" << endl <<  // Avoid multiple declarations of fcolor
"    int fcolor = GeColor.getDrawtype(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);" << endl;
	if ( shadow_width == 0) {

	  // Print noshadow shape only
	  fp <<
"    g.setColor(GeColor.getColor( fcolor));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;
	}
	else {
	  if ( !shadow) {
	    // Shadow should be choosable at runetim

	    // Print noshadow shape
	    fp <<
"    if ( shadow == 0) {" << endl;
	  fp <<
"    g.setColor(GeColor.getColor( fcolor));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;
	  fp <<
"    } else {" << endl;
	  }
	  else
	    (*shape_cnt)++;

	  // Print shadow shapes
	  fp <<
"    g.setColor(GeColor.shiftColor( fcolor, " << -drawtype_incr << ", colorInverse));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;
	  fp <<
"    g.setColor(GeColor.shiftColor( fcolor, " << drawtype_incr << ", colorInverse));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;
	  fp <<
"    g.setColor(GeColor.getColor( fcolor));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << (*shape_cnt)++ << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;
	  if ( !shadow)
	    fp <<
"    }" << endl;
	  fp <<
"    }" << endl;
	}
      }
      if (border)
      {
	if ( shadow_width == 0)
	  (*shape_cnt)--;
        fp <<
"    g.setStroke( new BasicStroke(" << line_width+1 << "F));" << endl <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
        if ( page <= 1)
          fp <<
"    g.draw( shapes[" << (*shape_cnt)++ << "]);" << endl;
        else
          fp <<
"    g.draw( shapes_p" << page << "[" << (*shape_cnt)++ << "]);" << endl;
      }
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::arc( double x0, double y0, double width, double height,
	double angel1, double angel2, int fill, int border,
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int line_width, double shadow_width, int shadow, 
	int drawtype_incr, glow_eExportPass pass, int *shape_cnt, 
	int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char arc_type_str[20];

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      if ( border && !fill)
        strcpy( arc_type_str, "OPEN");
      else
        strcpy( arc_type_str, "PIE");

      fp << 
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	width << "F, " << height << "F, " << 
	angel1 << "F, " << angel2 << "F, Arc2D." << arc_type_str <<")," << endl;

      if ( !(shadow_width == 0 || angel2 != 360)) {
	fp << 
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	  y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	  width << "F, " << height << "F, " << 
	  35 << "F, " << 140 << "F, Arc2D." << arc_type_str <<")," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	  y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	  width << "F, " << height << "F, " << 
	  215 << "F, " << 140 << "F, Arc2D." << arc_type_str <<")," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	  y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	  width << "F, " << height << "F, " << 
	  -5 << "F, " << 40 << "F, Arc2D." << arc_type_str <<")," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	  y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	  width << "F, " << height << "F, " << 
	  175 << "F, " << 40 << "F, Arc2D." << arc_type_str <<")," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset + shadow_width << "F, " << 
	  y0  - dim_y0 + glow_cJBean_Offset + shadow_width << "F, " << 
	  width - 2*shadow_width << "F, " << height - 2*shadow_width << "F, " << 
	  angel1 << "F, " << angel2 << "F, Arc2D." << arc_type_str <<")," << endl <<
"    new Arc2D.Float(" << x0 - dim_x0 + glow_cJBean_Offset << "F, " << 
	  y0  - dim_y0 + glow_cJBean_Offset << "F, " << 
	  width << "F, " << height << "F, " << 
	  angel1 << "F, " << angel2 << "F, Arc2D." << arc_type_str <<")," << endl;

      }
      break;
    }
    case glow_eExportPass_Init:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      if ( !shadow_width || angel2 != 360) {
	if ( fill) {
	  if ( border) {
	    if ( page <= 1)
	      fp <<
"    ((Arc2D)shapes[" << *shape_cnt << "]).setArcType(Arc2D.PIE);" << endl;
	    else
	      fp <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt << "]).setArcType(Arc2D.PIE);" << endl;
	  }
	  fp <<
"    g.setColor(GeColor.getColor(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << *shape_cnt << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt << "]);" << endl;
	}
	if (border) {
	  if ( fill) {
	    if ( page <= 1)
	      fp <<
"    ((Arc2D)shapes[" << *shape_cnt << "]).setArcType(Arc2D.OPEN);" << endl;
	    else
	      fp <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt << "]).setArcType(Arc2D.OPEN);" << endl;
	  }
	  fp <<
"    g.setStroke( new BasicStroke(" << line_width+1 << "F));" << endl <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.draw( shapes[" << *shape_cnt << "]);" << endl;
	  else
	    fp <<
"    g.draw( shapes_p" << page << "[" << *shape_cnt << "]);" << endl;
	}
	(*shape_cnt)++;
      }
      else {  // Shadow
	if ( !shadow) {
	  // Shadow is choosable in runtime
	  fp <<
"    if ( shadow == 0) {" << endl;
	  // Print noshadow
	  if ( fill) {
	    if ( border) {
	      if ( page <= 1)
		fp <<
"    ((Arc2D)shapes[" << *shape_cnt << "]).setArcType(Arc2D.PIE);" << endl;
	      else
		fp <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt << "]).setArcType(Arc2D.PIE);" << endl;
	    }
	    fp <<
"    g.setColor(GeColor.getColor(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));" << endl;
	    if ( page <= 1)
	      fp <<
"    g.fill( shapes[" << *shape_cnt << "]);" << endl;
	    else
	      fp <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt << "]);" << endl;
	  }
	  if (border) {
	    if ( fill) {
	      if ( page <= 1)
		fp <<
"    ((Arc2D)shapes[" << *shape_cnt << "]).setArcType(Arc2D.OPEN);" << endl;
	      else
		fp <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt << "]).setArcType(Arc2D.OPEN);" << endl;
	    }
	    fp <<
"    g.setStroke( new BasicStroke(" << line_width+1 << "F));" << endl <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
	    if ( page <= 1)
	      fp <<
"    g.draw( shapes[" << *shape_cnt << "]);" << endl;
	    else
	      fp <<
"    g.draw( shapes_p" << page << "[" << *shape_cnt << "]);" << endl;
	  }
	  (*shape_cnt)++;

	  fp <<
"    } else {" << endl;
	}
	else
	  (*shape_cnt)++;

	// Print shadow
	if ( fill) {
	  if ( border) {
	    if ( page <= 1)
	      fp <<
"    ((Arc2D)shapes[" << *shape_cnt << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes[" << *shape_cnt+1 << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes[" << *shape_cnt+2 << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes[" << *shape_cnt+3 << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes[" << *shape_cnt+4 << "]).setArcType(Arc2D.PIE);" << endl;
	    else
	      fp <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt+1 << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt+2 << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt+3 << "]).setArcType(Arc2D.PIE);" << endl <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt+4 << "]).setArcType(Arc2D.PIE);" << endl;
	  }
	  fp <<
"    {" << endl <<  // Avoid multiple declarations of fcolor
"    int fcolor = GeColor.getDrawtype(" << (int)fill_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);" << endl;


	  // Light shadow
	  fp <<
"    g.setColor(GeColor.shiftColor( fcolor, " << -drawtype_incr << ", colorInverse));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << *shape_cnt << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt << "]);" << endl;

	  // Dark shadow
	  fp <<
"    g.setColor(GeColor.shiftColor( fcolor, " << drawtype_incr << ", colorInverse));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << *shape_cnt+1 << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt+1 << "]);" << endl;

	  // Medium shadow and base
	  fp <<
"    g.setColor(GeColor.getColor( fcolor));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.fill( shapes[" << *shape_cnt+2 << "]);" << endl <<
"    g.fill( shapes[" << *shape_cnt+3 << "]);" << endl <<
"    g.fill( shapes[" << *shape_cnt+4 << "]);" << endl;
	  else
	    fp <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt+2 << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt+3 << "]);" << endl <<
"    g.fill( shapes_p" << page << "[" << *shape_cnt+4 << "]);" << endl;
	  fp <<
"    }" << endl;
	}
	if (border) {
	  if ( fill) {
	    if ( page <= 1)
	      fp <<
"    ((Arc2D)shapes[" << *shape_cnt+5 << "]).setArcType(Arc2D.OPEN);" << endl;
	    else
	      fp <<
"    ((Arc2D)shapes_p" << page << "[" << *shape_cnt+5 << "]).setArcType(Arc2D.OPEN);" << endl;
	  }
	  fp <<
"    g.setStroke( new BasicStroke(" << line_width+1 << "F));" << endl <<
"    g.setColor(GeColor.getColor(" << (int)border_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));" << endl;
	  if ( page <= 1)
	    fp <<
"    g.draw( shapes[" << *shape_cnt+5 << "]);" << endl;
	  else
	    fp <<
"    g.draw( shapes_p" << page << "[" << *shape_cnt+5 << "]);" << endl;
	}
	if ( !shadow)
	  fp <<
"    }" << endl;

	(*shape_cnt) += 6;
      }
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::text( int x0, int y0, char *text,
	glow_eDrawType drawtype, glow_eDrawType color_drawtype, int bold,
	int idx, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      break;
    }
    case glow_eExportPass_Init:
    {
      break;
    }
    case glow_eExportPass_Draw:
    {
      int text_size;
      char bold_str[20];

      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      switch ( idx)
      {
        case 0: text_size = 8; break;
        case 1: text_size = 10; break;
        case 2: text_size = 12; break;
        case 3: text_size = 14; break;
        case 4: text_size = 14; break;
        case 5: text_size = 18; break;
        case 6: text_size = 18; break;
        case 7: text_size = 18; break;
        default: text_size = 24;
      }
      if ( bold)
        strcpy(bold_str, "BOLD");
      else
        strcpy(bold_str, "PLAIN");
      fp <<
"    g.setColor(GeColor.getColor(" << (int)color_drawtype << ", colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));" << endl <<
      // "    g.setColor(Color.black);" << endl <<
"    g.setFont(new Font(\"Helvetica\", Font." << bold_str << ", " << 
	text_size << "));" << endl <<
"    g.drawString( \"" << str_cnv( text) << "\"," << 
	x0 - int(dim_x0) + glow_cJBean_Offset << ", " << 
	y0 - int(dim_y0) + glow_cJBean_Offset << ");" << endl;
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::annot( int x0, int y0, int number,
	glow_eDrawType drawtype, glow_eDrawType text_drawtype, int bold,
	int idx, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  int antialiasing_on = 0;

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      break;
    }
    case glow_eExportPass_Init:
    {
      char bold_str[20];
      int text_size;

      if ( bold)
        strcpy(bold_str, "BOLD");
      else
        strcpy(bold_str, "PLAIN");
      switch ( idx)
      {
        case 0: text_size = 8; break;
        case 1: text_size = 10; break;
        case 2: text_size = 12; break;
        case 3: text_size = 14; break;
        case 4: text_size = 14; break;
        case 5: text_size = 18; break;
        case 6: text_size = 18; break;
        case 7: text_size = 18; break;
        default: text_size = 24;
      }

      fp <<
"  String annot" << number << " = new String();" << endl <<
"  public String getAnnot" << number << "() { return annot" << number << ";}" << endl <<
"  public void setAnnot" << number << "( String s) { annot" << number << 
	" = s;}" << endl <<
"  Font annot" << number << "Font = new Font(\"Helvetica\", Font." << 
	bold_str << ", " << text_size << ");" << endl <<
"  int annot" << number << "Color = " << int(text_drawtype) << ";" << endl <<
"  public void setAnnot" << number << "Font( Font font) { annot" << number << 
	"Font = font;}" << endl <<
"  public void setAnnot" << number << "Color( int color) { annot" << number << 
	"Color = color;}" << endl <<
"  public Font getAnnot" << number << "Font() { return annot" << number << 
	"Font;}" << endl;

      break;
    }
    case glow_eExportPass_Draw:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      fp <<
"    g.setColor(GeColor.getColor( annot" << number << "Color , colorTone," << endl <<
"	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor));" << endl <<
	//"    g.setColor(Color.black);" << endl <<
"    g.setFont( annot" << number << "Font);" << endl <<
"    save_tmp = g.getTransform();" << endl;
      if ( antialiasing_on)
        fp <<
"    g.transform( AffineTransform.getScaleInstance( original_width/width *" << endl <<
"      		height/original_height * 0.75, 1));" << endl <<
"    g.drawString( annot" << number << ", " << 
	x0 - int(dim_x0) + glow_cJBean_Offset << 
	" * original_height / height * width / original_width, " << 
	y0 - int(dim_y0) + glow_cJBean_Offset << "F);" << endl;
      else
        fp <<
"    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);" << endl <<
"    g.transform( AffineTransform.getScaleInstance( original_width/width *" << endl <<
"      		height/original_height, 1));" << endl <<
"    g.drawString( annot" << number << ", " << 
	x0 - int(dim_x0) + glow_cJBean_Offset << 
	" * original_height / height * width / original_width, " << 
	y0 - int(dim_y0) + glow_cJBean_Offset << "F);" << endl;
      fp <<
"    g.setTransform( save_tmp);" << endl;
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::annot_font( int number, glow_eDrawType drawtype, 
	glow_eDrawType background, int bold,
	int idx, glow_eExportPass pass, ofstream& fp)
{

  switch ( pass)
  {
    case glow_eExportPass_Shape:
    {
      break;
    }
    case glow_eExportPass_Init:
    {
      char bold_str[20];
      int text_size;

      if ( bold)
        strcpy(bold_str, "BOLD");
      else
        strcpy(bold_str, "PLAIN");
      switch ( idx)
      {
        case 0: text_size = 8; break;
        case 1: text_size = 10; break;
        case 2: text_size = 12; break;
        case 3: text_size = 14; break;
        case 4: text_size = 14; break;
        case 5: text_size = 18; break;
        case 6: text_size = 18; break;
        case 7: text_size = 18; break;
        default: text_size = 24;
      }

      fp <<
"  Font annotFont = new Font(\"Helvetica\", Font." << 
	bold_str << ", " << text_size << ");" << endl <<
"  int annotBackground = " << (int) background << ";" << endl;
      break;
    }
    default:
      ;
  }
}

void GlowExportJBean::node( double x1, double y1, double x2, double y2,
	char *class_name,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType	fill_drawtype,
	glow_eDrawType text_drawtype,
    	glow_eDrawTone	color_tone,
    	int		color_lightness,
    	int		color_intensity,
    	int		color_shift,
    	int		line_width,
	double		rotate,
	int 		shadow,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  " << class_name << "	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<
"    " << var_name << " = new " << class_name << "(session);" << endl <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(y1 - dim_y0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(x2 - x1 + 2 * glow_cJBean_Offset) << "," << 
	(int)(y2 - y1 + 2 * glow_cJBean_Offset) << "));" << endl;
      if ( fill_drawtype != glow_eDrawType_No && color_tone == glow_eDrawTone_No)
        fp <<
"    " << var_name << ".setFillColor(" << (int)fill_drawtype << ");" << endl;
      if ( border_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setBorderColor(" << (int)border_drawtype << ");" << endl;
      if ( text_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setTextColor(" << (int)text_drawtype << ");" << endl;
      if ( color_tone != glow_eDrawTone_No)
        fp <<
"    " << var_name << ".setColorTone(" << (int)color_tone << ");" << endl;
      if ( color_lightness)
        fp <<
"    " << var_name << ".setColorBrightness(" << color_lightness << ");" << endl;
      if ( color_shift)
        fp <<
"    " << var_name << ".setColorShift(" << color_shift << ");" << endl;
      if ( color_intensity)
        fp <<
"    " << var_name << ".setColorIntensity(" << color_intensity << ");" << endl;
      if ( rotate)
        fp <<
"    " << var_name << ".setRotate(" << rotate << ");" << endl;
      if ( shadow)
        fp <<
"    " << var_name << ".setShadow(" << shadow << ");" << endl;
      if ( is_nodeclass)
        fp <<
"    add(" << var_name << ");" << endl;
      else
        fp <<
"    localPanel.add(" << var_name << ", null);" << endl;

       break;
    }
    case glow_eExportPass_Draw:
      if ( is_nodeclass) {
        fp <<
"    save_tmp = g.getTransform();" << endl <<
"    g.translate( " << var_name << ".getX(), " << var_name << ".getY());" << endl <<
"    if ( " << var_name << ".dd.dynType == 0) {" << endl <<
"      " << var_name << ".colorTone = colorTone;" << endl <<
"      " << var_name << ".colorBrightness = colorBrightness;" << endl <<
"      " << var_name << ".colorIntensity = colorIntensity;" << endl <<
"      " << var_name << ".colorShift = colorShift;" << endl <<
"      " << var_name << ".colorInverse = colorInverse;" << endl <<
"    }" << endl <<
"    " << var_name << ".paintComponent(g);" << endl <<
"    g.setTransform( save_tmp);" << endl;
      }
      break;
    default:
      ;
  }
}

void GlowExportJBean::image( double x1, double y1, double x2, double y2,
	char *filename, int transparent,
    	glow_eDrawTone	color_tone,
    	int		color_lightness,
    	int		color_intensity,
    	int		color_shift,
	double		rotate,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];
  char *s;

  if ( (s = strrchr( filename, '/')) || 
       (s = strchr( filename, '>')) ||
       (s = strchr( filename, ']')) ||
       (s = strchr( filename, ':')))
    strcpy( var_name, s+1);
  else
    strcpy( var_name, filename);
  if ( (s = strrchr( var_name, '.')))
    *s = 0;
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  GeImage 	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      if ( is_nodeclass)
        nc->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);
      else
        ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<
"    " << var_name << " = new GeImage(session);" << endl <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(y1 - dim_y0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(x2 - x1 + 2 * glow_cJBean_Offset) << "," << 
	(int)(y2 - y1 + 2 * glow_cJBean_Offset) << "));" << endl <<
"    " << var_name << ".setImage( \"" << filename << "\");" << endl;
      if ( color_tone != glow_eDrawTone_No)
        fp <<
"    " << var_name << ".setColorTone(" << (int)color_tone << ");" << endl;
      if ( color_lightness)
        fp <<
"    " << var_name << ".setColorBrightness(" << color_lightness << ");" << endl;
      if ( color_shift)
        fp <<
"    " << var_name << ".setColorShift(" << color_shift << ");" << endl;
      if ( color_intensity)
        fp <<
"    " << var_name << ".setColorIntensity(" << color_intensity << ");" << endl;
      if ( rotate)
        fp <<
"    " << var_name << ".setRotate(" << rotate << ");" << endl;
      if ( transparent)
        fp <<
"    " << var_name << ".setTransparent( 1);" << endl;
      if ( is_nodeclass)
        fp <<
"    add(" << var_name << ");" << endl;
      else
        fp <<
"    localPanel.add(" << var_name << ", null);" << endl;

      break;
    }
    case glow_eExportPass_Draw:
      if ( is_nodeclass) {
        fp <<
"    save_tmp = g.getTransform();" << endl <<
"    g.translate( " << var_name << ".getX(), " << var_name << ".getY());" << endl <<
"    " << var_name << ".colorTone = colorTone;" << endl <<
"    " << var_name << ".colorBrightness = colorBrightness;" << endl <<
"    " << var_name << ".colorIntensity = colorIntensity;" << endl <<
"    " << var_name << ".colorShift = colorShift;" << endl <<
"    " << var_name << ".colorInverse = colorInverse;" << endl <<
"    " << var_name << ".paintComponent(g);" << endl <<
"    g.setTransform( save_tmp);" << endl;
      }
      break;
    default:
      ;
  }
}

void GlowExportJBean::bar( double x1, double y1, double x2, double y2,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType	fill_drawtype,
    	glow_eDrawType	bar_drawtype,
    	glow_eDrawType	bar_bordercolor,
    	int		fill,
	int		border,
	double		min_value,
	double		max_value,
	int		bar_border_width,
    	int		line_width,
	double		rotate,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];
  char class_name[] = "JopBar";

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  " << class_name << "	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<
"    " << var_name << " = new " << class_name << "(session);" << endl <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(y1 - dim_y0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(x2 - x1 + 2 * glow_cJBean_Offset) << "," << 
	(int)(y2 - y1 + 2 * glow_cJBean_Offset) << "));" << endl;
      if ( fill_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setFillColor(" << (int)fill_drawtype << ");" << endl;
      if ( border_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setBorderColor(" << (int)border_drawtype << ");" << endl;
      else
        fp <<
"    " << var_name << ".setBorderColor( 0);" << endl;
      if ( bar_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setFillColorBar(" << (int)bar_drawtype << ");" << endl;
      if ( bar_bordercolor != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setBorderColorBar(" << (int)bar_bordercolor << ");" << endl;
      if ( fill)
        fp <<
"    " << var_name << ".setDrawFill(1);" << endl;
      if ( border)
        fp <<
"    " << var_name << ".setDrawBorder(1);" << endl;
      fp <<
"    " << var_name << ".setBarBorderWidth(" << bar_border_width << ");" << endl <<
"    " << var_name << ".setLineWidth(" << line_width << ");" << endl <<
"    " << var_name << ".setMinValue(" << min_value << "F);" << endl <<
"    " << var_name << ".setMaxValue(" << max_value << "F);" << endl <<
"    " << var_name << ".setRotate(" << rotate << ");" << endl;
      if ( is_nodeclass)
	fp <<
"    add(" << var_name << ");" << endl;
      else
	fp <<
"    localPanel.add(" << var_name << ", null);" << endl;
      break;
    }
    case glow_eExportPass_Draw:
      break;
    default:
      ;
  }
}

void GlowExportJBean::trend( double x1, double y1, double x2, double y2,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType	fill_drawtype,
    	glow_eDrawType	curve_drawtype1,
    	glow_eDrawType	curve_drawtype2,
    	glow_eDrawType	curve_fill_drawtype1,
    	glow_eDrawType	curve_fill_drawtype2,
    	int		fill,
	int		border,
	double		min_value1,
	double		max_value1,
	double		min_value2,
	double		max_value2,
	int		curve_width,
	int		no_of_points,
	double		scan_time,
	int		horizontal_lines,
	int		vertical_lines,
    	int		line_width,
	double		rotate,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];
  char class_name[] = "JopTrend";

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  " << class_name << "	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<
"    " << var_name << " = new " << class_name << "( session);" << endl <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(y1 - dim_y0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(x2 - x1 + 2 * glow_cJBean_Offset) << "," << 
        (int)(y2 - y1 + 2 * glow_cJBean_Offset) << "));" << endl;
      if ( fill_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setFillColor(" << (int)fill_drawtype << ");" << endl;
      if ( border_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setBorderColor(" << (int)border_drawtype << ");" << endl;
      else
        fp <<
"    " << var_name << ".setBorderColor( 0);" << endl;
      if ( curve_drawtype1 != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setBorderColorTrend1(" << (int)curve_drawtype1 << ");" << endl;
      if ( curve_drawtype2 != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setBorderColorTrend2(" << (int)curve_drawtype2 << ");" << endl;
      if ( curve_fill_drawtype1 != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setFillColorTrend1(" << (int)curve_fill_drawtype1 << ");" << endl;
      if ( curve_fill_drawtype2 != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setFillColorTrend2(" << (int)curve_fill_drawtype2 << ");" << endl;
      if ( fill)
        fp <<
"    " << var_name << ".setDrawFill(1);" << endl;
      if ( border)
	fp <<
"    " << var_name << ".setDrawBorder(1);" << endl;
      fp <<
"    " << var_name << ".setTrendBorderWidth(" << curve_width << ");" << endl <<
"    " << var_name << ".setLineWidth(" << line_width << ");" << endl <<
"    " << var_name << ".setMinValue1(" << min_value1 << "F);" << endl <<
"    " << var_name << ".setMaxValue1(" << max_value1 << "F);" << endl <<
"    " << var_name << ".setMinValue2(" << min_value2 << "F);" << endl <<
"    " << var_name << ".setMaxValue2(" << max_value2 << "F);" << endl <<
"    " << var_name << ".setNoOfPoints(" << no_of_points << ");" << endl <<
"    " << var_name << ".setHorizontalLines(" << horizontal_lines << ");" << endl <<
"    " << var_name << ".setVerticalLines(" << vertical_lines << ");" << endl <<
"    " << var_name << ".setRotate(" << rotate << ");" << endl;
      if ( is_nodeclass)
	fp <<
"    add(" << var_name << ");" << endl;
      else
	fp <<
"    localPanel.add(" << var_name << ", null);" << endl;
      break;
    }
    case glow_eExportPass_Draw:
      break;
    default:
      ;
  }
}

void GlowExportJBean::axis( double x1, double y1, double x2, double y2,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType text_drawtype,
	double		min_value,
	double		max_value,
	int	        lines,
	int		longquotient,
	int		valuequotient,
	int		line_length,
    	int		line_width,
	double		rotate,
	int             bold,
	int             text_idx,
	char            *format,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];
  char class_name[] = "JopAxis";


  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  " << class_name << "	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      char bold_str[20];
      int text_size;

      ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      switch ( text_idx)
      {
        case 0: text_size = 8; break;
        case 1: text_size = 10; break;
        case 2: text_size = 12; break;
        case 3: text_size = 14; break;
        case 4: text_size = 14; break;
        case 5: text_size = 18; break;
        case 6: text_size = 18; break;
        case 7: text_size = 18; break;
        default: text_size = 24;
      }
      if ( bold)
        strcpy(bold_str, "BOLD");
      else
        strcpy(bold_str, "PLAIN");

      fp <<
"    " << var_name << " = new " << class_name << "();" << endl <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 + glow_cJBean_Offset) << "," << 
	(int)(y1 - dim_y0 + glow_cJBean_Offset) << "," << 
	(int)(x2 - x1 /* + 2 * glow_cJBean_Offset */) << "," << 
	(int)(y2 - y1 /* + 2 * glow_cJBean_Offset */) << "));" << endl;

      if ( border_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setBorderColor(" << (int)border_drawtype << ");" << endl;
      else
        fp <<
"    " << var_name << ".setBorderColor( 0);" << endl;
      if ( text_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setTextColor(" << (int)text_drawtype << ");" << endl;
      else
        fp <<
"    " << var_name << ".setTextColor( 0);" << endl;
      fp <<
"    " << var_name << ".setLineWidth(" << line_width << ");" << endl <<
"    " << var_name << ".setMinValue(" << min_value << "F);" << endl <<
"    " << var_name << ".setMaxValue(" << max_value << "F);" << endl <<
"    " << var_name << ".setLines(" << lines << ");" << endl <<
"    " << var_name << ".setLongQuotient(" << longquotient << ");" << endl <<
"    " << var_name << ".setValueQuotient(" << valuequotient << ");" << endl <<
"    " << var_name << ".setLineLength(" << line_length << ");" << endl <<
"    " << var_name << ".setLineWidth(" << line_width << ");" << endl <<
"    " << var_name << ".setRotate(" << rotate << ");" << endl <<
"    " << var_name << ".setFont(new Font(\"Helvetica\", Font." << bold_str 
	 << ", " << text_size << "));" << endl <<
"    " << var_name << ".setFormat(\"" << format << "\");" << endl;


      if ( is_nodeclass)
	fp <<
"    add(" << var_name << ");" << endl;
      else
	fp <<
"    localPanel.add(" << var_name << ", null);" << endl;
      break;
    }
    case glow_eExportPass_Draw:
      break;
    default:
      ;
  }
}

void GlowExportJBean::window( double x1, double y1, double x2, double y2,
	char *filename,
        int vertical_scrollbar, int horizontal_scrollbar,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];
  char class_name[] = "JScrollPane";
  char class_fname[80];
  char var_fname[80];
  char *s;

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  // Convert filename to java class
  strcpy( class_fname, filename);
  if ( (s = strchr( class_fname, '.')))
    *s = 0;
  sprintf( var_fname, "%s%d", class_fname, node_cnt);
  class_fname[0] = _toupper( class_fname[0]);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  " << class_name << "	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<
"    " << class_fname << " " << var_fname << " = new " << class_fname << "(session, null, false);" << endl <<
"    " << var_name << " = new " << class_name << "(" << var_fname << ".localPanel);" << endl <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(y1 - dim_y0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(x2 - x1 + 2 * glow_cJBean_Offset) << "," << 
	(int)(y2 - y1 + 2 * glow_cJBean_Offset) << "));" << endl <<
"    localPanel.add(" << var_name << ", null);" << endl;
      break;
    }
    case glow_eExportPass_Draw:
      break;
    default:
      ;
  }
}

void GlowExportJBean::folder( double x1, double y1, double x2, double y2,
	int folders,
	char *folder_file_names, char *folder_text, 
	int *folder_v_scrollbar, int *folder_h_scrollbar,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];
  char class_name[] = "JTabbedPane";
  char class_fname[80];
  char var_fname[80];
  char *s;

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  " << class_name << "	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<
"    " << var_name << " = new " << class_name << "();" << endl;

      char *fname_p = folder_file_names;
      char *text_p = folder_text;
      for ( int i = 0; i < folders; i++) {
	strcpy( class_fname, fname_p);
	if ( (s = strchr( class_fname, '.')))
	  *s = 0;
	sprintf( var_fname, "%s%d_%d", class_fname, node_cnt, i);
	class_fname[0] = _toupper( class_fname[0]);

	fp <<
"    " << class_fname << " " << var_fname << " = new " << class_fname << "(session, null, false);" << endl;
	if ( folder_v_scrollbar[i] || folder_h_scrollbar[i])
	  fp <<
"    " << var_name << ".addTab(\"" << text_p << "\", new JScrollPane(" << var_fname << ".localPanel));" << endl;
	else 
	  fp <<
"    " << var_name << ".addTab(\"" << text_p << "\", " << var_fname << ".localPanel);" << endl;
	fname_p += 80;
	text_p += 80;
      }

      fp <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(y1 - dim_y0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(x2 - x1 + 2 * glow_cJBean_Offset) << "," << 
	(int)(y2 - y1 + 2 * glow_cJBean_Offset) << "));" << endl <<
"    localPanel.add(" << var_name << ", null);" << endl;
      break;
    }
    case glow_eExportPass_Draw:
      break;
    default:
      ;
  }
}

void GlowExportJBean::table( double x1, double y1, double x2, double y2,
    	glow_eDrawType	fill_drawtype, int fill,
	int rows, int columns, int header_row, int header_column,
	int text_idx, glow_eDrawType text_drawtype,
	double row_height, double *column_width, char *header_text,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp)
{
  double dim_x0, dim_x1, dim_y0, dim_y1;
  char var_name[40];
  char class_name[] = "GeTable";
  int text_size;
  char bold_str[20];

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", node_cnt);

  switch ( pass)
  {
    case glow_eExportPass_Shape:
      break;
    case glow_eExportPass_Declare:
    {
      fp <<
"  " << class_name << "	" << var_name << ";" << endl;
      break;
    }
    case glow_eExportPass_Attributes:
    {
      switch ( text_idx)
      {
        case 0: text_size = 8; break;
        case 1: text_size = 10; break;
        case 2: text_size = 12; break;
        case 3: text_size = 14; break;
        case 4: text_size = 14; break;
        case 5: text_size = 18; break;
        case 6: text_size = 18; break;
        case 7: text_size = 18; break;
        default: text_size = 24;
      }
      if ( text_drawtype == glow_eDrawType_TextHelveticaBold)
        strcpy(bold_str, "BOLD");
      else
        strcpy(bold_str, "PLAIN");


      ((GrowCtx *)ctx)->measure_javabean( &dim_x1, &dim_x0, &dim_y1, &dim_y0);

      fp <<
"    " << var_name << " = new " << class_name << "(session, " << 
	rows << "," << columns << "," << header_row << "," << header_column << ");" << endl <<
"    " << var_name << ".setRowHeight(" << int(row_height) << ");" << endl;
      char *text_p = header_text;
      for ( int i = 0; i < columns; i++) {
	fp << 
"    " << var_name << ".setHeaderText(" << i << ",\"" << text_p << "\");" << endl <<
"    " << var_name << ".setColumnWidth(" << i << "," << int(column_width[i]) << ");" << endl;
	text_p += 40;
      }
      if ( fill_drawtype != glow_eDrawType_No)
        fp <<
"    " << var_name << ".setFillColor(" << (int)fill_drawtype << ");" << endl;
      fp <<
"    " << var_name << ".setFont(new Font(\"Helvetica\", Font." << bold_str 
	 << ", " << text_size << "));" << endl;

      fp <<
"    " << var_name << ".setBounds(new Rectangle(" << 
	(int)(x1 - dim_x0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(y1 - dim_y0 /* - glow_cJBean_Offset) */) << "," << 
	(int)(x2 - x1 + 2 * glow_cJBean_Offset) << "," << 
	(int)(y2 - y1 + 2 * glow_cJBean_Offset) << "));" << endl;
      fp <<
"    localPanel.add(" << var_name << ", null);" << endl;
      break;
    }
    case glow_eExportPass_Draw:
      break;
    default:
      ;
  }
}

void GlowExportJBean::slider( double x1, double y1, double x2, double y2,
	char *class_name,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType	fill_drawtype,
    	glow_eDrawType	text_drawtype,
    	glow_eDrawTone	color_tone,
    	int		color_lightness,
    	int		color_intensity,
    	int		color_shift,
    	int		line_width,
	double		rotate,
	int		shadow,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream& fp)
{
  node( x1, y1, x2, y2, class_name, border_drawtype, fill_drawtype, text_drawtype,
	color_tone, color_lightness, color_intensity, color_shift,
	line_width, rotate, shadow, pass, shape_cnt, node_cnt, in_nc, fp);
}


// Replace " to \"
static char *str_cnv( char *instr)
{
  static char outstr[200];
  char *in;
  char *out = outstr;

  for ( in = instr; *in != 0; in++)
  {
    if ( *in == '"')
      *out++ = '\\';
    *out++ = *in;
  }
  *out = *in;
  return outstr;
}



