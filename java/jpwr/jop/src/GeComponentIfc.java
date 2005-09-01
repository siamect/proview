/* 
 * Proview   $Id: GeComponentIfc.java,v 1.4 2005-09-01 14:57:50 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

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
    public void setVisibility( int visibility);
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
