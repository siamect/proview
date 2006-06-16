/* 
 * Proview   $Id: FlowCmn.java,v 1.4 2006-06-16 05:09:38 claes Exp $
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
import java.io.*;
import java.util.*;


public class FlowCmn {
  static final int display_level = Flow.mDisplayLevel_1;
  boolean debug;
  boolean antiAliasing;
  Object ctx;
  Gdh gdh;
  JopSession session;
  double zoom_factor;
  double base_zoom_factor;
  int offset_x;
  int offset_y;
  double x_right;
  double x_left;
  double y_high;
  double y_low;

  //Vector<Object> a = new Vector<Object>();
  Vector a = new Vector();
  //Vector<Object> a_nc = new Vector<Object>();
  Vector a_nc = new Vector();
  //Vector<Object> a_cc = new Vector<Object>();
  Vector a_cc = new Vector();

  public FlowCmn( Object ctx, Gdh gdh, JopSession session) {
    this.ctx = ctx;
    this.gdh = gdh;
    this.session = session;
    this.debug = false;
    this.antiAliasing = true;
  }

  public void unselect() {
    ((FlowCtxInterface)ctx).unselect();
  }
}
