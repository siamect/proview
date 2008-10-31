/* 
 * Proview   $Id: flow_nodearray.h,v 1.3 2008-10-31 12:51:33 claes Exp $
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
 **/

#ifndef flow_array_h
#define flow_array_h

#include <iostream>

#include "flow_array_elem.h"

typedef void *flow_tAddress;

typedef enum {
	flow_eElemTypeRectangle,
	flow_eElemTypeCircle,
	flow_eElemTypeLine,
	flow_eElemTypePoint,
	flow_eElemTypeNode,
	flow_eElemTypeConnection
	} flow_eElemType;

class FlowArray {
  public:
    FlowArray( int allocate = 10, int incr = 10);
    FlowArrayElem *operator[] ( int idx);
    friend ostream& operator<< ( ostream& o, const FlowArray a);
    int size() { return a_size;};
    void insert( FlowArrayElem *element);
    void zoom();
    friend class FlowNodeClass;
    friend class FlowCtx;
  private:
    int	allocated;
    int alloc_incr;
    int a_size;
    FlowArrayElem **a;
};

#endif
