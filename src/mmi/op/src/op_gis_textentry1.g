// 
//  Proview   $Id: op_gis_textentry1.g,v 1.2 2005-09-06 10:39:34 claes Exp $
//  Copyright (C) 2005 SSAB Oxelösund AB.
//
//  This program is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU General Public License as 
//  published by the Free Software Foundation, either version 2 of 
//  the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License 
//  along with the program, if not, write to the Free Software 
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
mtran0
vis 1
detect 1
op_gis_textentry1: model
    fcolor 0
    fstyle 1
    finter 1
    fdir 0
    fpercent 100
    ecolor 7
    estyle 1
    ewidth 1
    tcolor 7
    height 0
    path 1
    font 0
    prec 2
    align 2 3
    size 2 2
    ftrect -5 4 1 1  "Text"
    . dynprop  \
          (#  \
            (call gms_textentry_var(callback, &variable, maxchars, type))) \
          (*  \
            (tfont font)  \
            (theight size))  \
          (__selected_object  \
            (= *   \
              (call gms_hilite_edge_selobj(2, 7, 3., 1.))))   \
          ((upperlimit == 0)   \
            (= 1  \
              (tfont font) ))  \
          ((lowerlimit == 0)   \
            (= 1  \
              (theight size) ))
    . move 2 -2.5
endm
