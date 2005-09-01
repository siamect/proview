/* 
 * Proview   $Id: JopScrollPane.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.JScrollBar;

public class JopScrollPane extends JScrollPane
{
  public JopScrollPane()
  {
    super();
    setViewport(new JopViewport());
  }
  public JViewport getViewport()
  {
    JScrollBar hS = getHorizontalScrollBar();
    if(hS == null) return super.getViewport();
    int val = hS.getValue();
    if(val == 0)
      return super.getViewport();
    JopViewport vp = (JopViewport)super.getViewport();
    vp.user = true;
    return vp;      
  }
}
