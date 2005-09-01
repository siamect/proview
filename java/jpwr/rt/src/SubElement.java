/* 
 * Proview   $Id: SubElement.java,v 1.3 2005-09-01 14:57:52 claes Exp $
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

  /**
   * Object som lagras i den statiska vektorn, i GdhServer, som handhar alla prenumerationer
   *
   *@author     JN3920
   *@created    den 18 november 2002
   */
  package jpwr.rt;
  import java.io.Serializable;
  import java.util.*;
  public class SubElement
  {
    public int[] reffedByThread;
    public BitSet reffedByThreadBitSet;
    Sub sub = new Sub(" ", new PwrtRefId(0, 0), 0, 0, 0,0,0);


    public SubElement(int maxConnections, int threadNumber)
    {
      reffedByThread = new int[maxConnections];
      reffedByThreadBitSet = new BitSet(maxConnections);
      reffedByThreadBitSet.set(threadNumber);
      for(int i = 0; i < reffedByThread.length; i++)
      {
        reffedByThread[i] = 0;
      }
    }
    public SubElement(String attrName)
    {
      this.sub.attrName = attrName;
    }

    public Sub getSub()
    {
      return sub;
    }
    public void setSub(GdhrRefObjectInfo obj, int subscriptionsIndex, String attrName)
    {
      this.sub.attrName = attrName;
      this.sub.refid = obj.refid;
      this.sub.id = obj.id;
      this.sub.sts = obj.sts;
      this.sub.typeId = obj.typeId;
      this.sub.size = obj.size;
      this.sub.elements = obj.elements;
      this.sub.subscriptionsIndex = subscriptionsIndex;

    }
    public boolean equals(Object o)
    {
      return (this.sub.attrName.equalsIgnoreCase(((SubElement)o).sub.attrName) && (((SubElement)o).sub.sts != 0));
    }
  }

