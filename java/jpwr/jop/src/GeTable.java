
package jpwr.jop;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.Timer;
import javax.swing.table.*;
import javax.swing.event.*;

public class GeTable extends JScrollPane implements GeComponentIfc, 
		  JopDynamic, ActionListener, ListSelectionListener
{
  Dimension size;
  Object root;
  Timer timer = new Timer(500, this);
  JopSession session;
  public JopEngine en;
  public GeDyn dd = new GeDyn( this);

  JTable table;
  JTable headerColumn;
  int rows;
  int columns;
  int hRow;
  int hColumn;
  JViewport jv;

  public boolean focus = false;
  public boolean confirmActive = false;
  public Color  normalColor = null;
  public GeTable component = this;
  public GeTable( JopSession session, int rows, int columns, int hRow, int hColumn)
  {
    this.session = session;
    dd.setSession( session);
    size = new Dimension( 102, 36);
    timer.start();

    this.rows = rows;
    this.columns = columns;
    this.hRow = hRow;
    this.hColumn = hColumn;
      
    table = new JTable( rows, columns - hColumn);
    table.setColumnSelectionAllowed( false);
    table.setRowSelectionAllowed( false);
    table.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    table.getColumnModel().getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

    if ( hRow == 0) {
      table.setTableHeader( null);
    }
    if ( hColumn == 1) {
      // Set up the header column
      headerColumn = new JTable(rows, 1);
      table.setSelectionModel( headerColumn.getSelectionModel());

      headerColumn.setMaximumSize( new Dimension( 90, 10000));
      headerColumn.setColumnSelectionAllowed( false);
      headerColumn.setRowSelectionAllowed( false);
      // headerColumn.setCellSelectionEnabled( false);

      jv = new JViewport();
      jv.setView( headerColumn);
      jv.setPreferredSize( headerColumn.getMaximumSize());

      table.setAutoResizeMode( JTable.AUTO_RESIZE_OFF);
      headerColumn.setAutoResizeMode( JTable.AUTO_RESIZE_OFF);

      getViewport().setView(table);
      setRowHeader( jv);
    }
    else {
      table.setAutoResizeMode( JTable.AUTO_RESIZE_OFF);
      getViewport().setView(table);
    }
  }

  public void setHeaderText( int idx, String text) {
    if ( idx == 0 && hColumn == 1) {
      // Header of header row is not supported...
    }
    else if ( hRow != 0)
      table.getColumnModel().getColumn(idx - hColumn).setHeaderValue( text);
  }
  public void setColumnWidth( int idx, int width) {
    if ( idx == 0 && hColumn == 1) {
      headerColumn.getColumnModel().getColumn(0).setPreferredWidth( width);
      jv.setPreferredSize( new Dimension( width, 10000));
    }
    else
      table.getColumnModel().getColumn(idx - hColumn).setPreferredWidth( width);
  }
  public void setRowHeight( int height) {
    table.setRowHeight( height);
    if ( hColumn == 1)
      headerColumn.setRowHeight( height);
  }
  public void setHeaderRowHeight( int height) {
  }
  public void setFont( Font font) {
    if ( table != null)
      table.setFont( font);
    if ( hColumn == 1 && headerColumn != null)
      headerColumn.setFont( font);
  }

  public void setValueAt( String value, int row, int column) {
    if ( column == 0 && hColumn == 1)
      headerColumn.setValueAt( value, row, column);
    else
      table.setValueAt( value, row, column - hColumn);
  }

  public int getSelectedRow() {
    int row = table.getSelectedRow();
    if ( row == -1 && hColumn == 1)
      row = headerColumn.getSelectedRow();
    return row;
  }
  public int getSelectedColumn() {
    int column;

    if ( hColumn == 1)
      // Always return header column
      column = 0;
    else
      column = table.getSelectedColumn();
    return column;
  }

  public void actionPerformed(ActionEvent e) {
    boolean engine_found = false;

    root = getTopLevelAncestor();
    if ( root != null) {
      if ( root instanceof JopApplet)
	session = ((JopApplet)root).session;
      else if ( root instanceof JopFrame)
	session = ((JopFrame) root).session;
      en = session.getEngine();

      if ( !en.isReady())
        return;
      en.add(this);
      engine_found = true;
    }

    if ( engine_found) {
      timer.stop();
      timer = null;

      if ( en.gdh.isAuthorized( dd.access)) {
        table.getSelectionModel().addListSelectionListener( this);
        table.getColumnModel().getSelectionModel().addListSelectionListener( this);

        table.addMouseListener(new MouseAdapter() {
          public void mouseReleased(MouseEvent e) {
	    if ( e.isPopupTrigger())
	      dd.action( GeDyn.eEvent_MB3Press, e);
	  }

          public void mousePressed(MouseEvent e) {
	    if ( e.isPopupTrigger())
	      dd.action( GeDyn.eEvent_MB3Press, e);
	  }
        });
	if ( hColumn == 1) {
          headerColumn.addMouseListener(new MouseAdapter() {
            public void mouseReleased(MouseEvent e) {
	      if ( e.isPopupTrigger())
	        dd.action( GeDyn.eEvent_MB3Press, e);
	    }

            public void mousePressed(MouseEvent e) {
	      if ( e.isPopupTrigger())
	        dd.action( GeDyn.eEvent_MB3Press, e);
	    }
          });
	}
      }
    }
  }

  public void valueChanged(ListSelectionEvent e) {
    MouseEvent me = new MouseEvent( this, 0,0,0,0,0,0,false);
    dd.action( GeDyn.eEvent_MB1Click, me);
  }

  // GeComponents Ifc
  public void tsetFillColor( int fillColor) { 

    this.fillColor = fillColor;
    normalColor = GeColor.getColor( fillColor, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    table.setBackground( normalColor);
    if ( hColumn == 1)
      headerColumn.setBackground( normalColor);
  }
  public void tsetColorTone( int colorTone) {
    this.colorTone = colorTone;
    normalColor = GeColor.getColor( fillColor, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, GeColor.NO_COLOR);
    table.setBackground( normalColor);
    if ( hColumn == 1)
      headerColumn.setBackground( normalColor);
  }
  public void tsetBorderColor( int borderColor) {}
  public void tsetTextColor( int borderColor) {}
  public void setColorInverse( int colorInverse) {} 
  public void resetFillColor() { 
    fillColor = originalFillColor;
    normalColor = GeColor.getColor( fillColor, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    table.setBackground( normalColor);
    if ( hColumn == 1)
      headerColumn.setBackground( normalColor);
  }
  public void resetColorTone() { 
    colorTone = originalColorTone;
    normalColor = GeColor.getColor( fillColor, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    table.setBackground( normalColor);
    if ( hColumn == 1)
      headerColumn.setBackground( normalColor);
  }
  public void resetBorderColor() {}
  public void resetTextColor() {}
  public String getAnnot1() { return "";}
  public void setAnnot1( String s) {}
  public void setLastPage() {}
  public void setFirstPage() {}
  public void setPage( int page) {}
  public int setNextPage() { return 1;}
  public int setPreviousPage() { return 1;}
  public Object getDd() { return dd;}
  public void setFillLevel( float fillLevel) {}
  public void setLevelDirection( int levelDirection) {}
  public void setLevelColorTone( int levelColorTone) {}
  public void setLevelFillColor( int levelFillColor) {}

  Font cellFont = new Font("Helvetica", Font.BOLD, 14);
  public void setCellFont( Font font) { cellFont = font; setFont(font);}
  public Font getCellFont() { return cellFont;}
  public int fillColor = 9999;
  public int originalFillColor = 9999;
  public int borderColor = 9999;
  public int originalBorderColor = 9999;
  public int colorTone = 0;
  public int originalColorTone = 0;
  public int colorShift = 0;
  public int originalColorShift = 0;
  public int colorBrightness = 0;
  public int originalColorBrightness = 0;
  public int colorIntensity = 0;
  public int originalColorIntensity = 0;
  public int colorInverse = 0;
  public int originalColorInverse = 0;
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
  public int getFillColor() {
    return fillColor;
  }
  public void setBorderColor( int borderColor) {
    this.borderColor = borderColor;
    this.originalBorderColor = borderColor;
  }
  public int getBorderColor() {
    return borderColor;
  }
  public double rotate;
  public void setRotate( double rotate) {}
  public double getRotate() { return rotate;}

  public Object dynamicGetRoot() {
    return root;
  }
  public void dynamicOpen() {
    if ( en.isInstance())
      dd.setInstance( en.getInstance());

    dd.connect();

    normalColor = GeColor.getColor( fillColor, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    table.setBackground( normalColor);
    if ( hColumn == 1)
      headerColumn.setBackground( normalColor);
  }
  public void dynamicClose() {
    dd.disconnect();
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly || focus)
      return;

    dd.scan();

  }

  public void repaintForeground() {
    Graphics g = getGraphics();
    if ( g == null) {
      System.out.println("repaintForeground: can't get Graphic object");
      return;
    }
    paintComponent(g);
    paintChildren(g);
  }
}















