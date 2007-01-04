/* 
 * Proview   $Id: glow_nodegroup.h,v 1.3 2007-01-04 07:57:39 claes Exp $
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

#ifndef glow_nodegroup_h
#define glow_nodegroup_h

#include "glow_nodeclass.h"

class GlowNodeGroup : public GlowNodeClass {
  public:
    GlowNodeGroup( GrowCtx *glow_ctx, char *name, GlowArray& array);
    GlowNodeGroup( GrowCtx *glow_ctx, char *name);
    GlowNodeGroup( const GlowNodeGroup& nc);
    ~GlowNodeGroup();
    glow_eObjectType type() { return glow_eObjectType_NodeGroup;};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void ungroup( GlowTransform *t);
};


#endif
