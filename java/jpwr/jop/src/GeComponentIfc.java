package jpwr.jop;
import jpwr.rt.*;

public interface GeComponentIfc {
    public void tsetFillColor( int fillColor);
    public void tsetColorTone( int colorTone);
    public void tsetBorderColor( int borderColor);
    public void tsetTextColor( int textColor);
    public void setColorInverse( int inverse);
    public void resetFillColor();
    public void resetColorTone();
    public void resetBorderColor();
    public void resetTextColor();
    public void setFillLevel( float fillLevel);
    public void setLevelDirection( int levelDirection);
    public void setLevelColorTone( int levelColorTone);
    public void setLevelFillColor( int levelFillColor);
    public String getAnnot1();
    public void setAnnot1( String annot1);
    public void setLastPage();
    public void setFirstPage();
    public void setPage( int page);
    public int setNextPage();
    public int setPreviousPage();
    public Object getDd();
    public void repaintForeground();
    public Object dynamicGetRoot();
}
