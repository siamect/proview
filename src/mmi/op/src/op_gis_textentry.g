// 
//  Proview   $Id: op_gis_textentry.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_gis_textentry: model
    fcolor 11
    fstyle 1
    finter 2
    fdir 0
    fpercent 100
    ecolor 7
    estyle 1
    ewidth 1
    tcolor 7
    height 2
    path 1
    font 7
    prec 0
    align 2 3
    size 0 0
    ftrect 0 0 16 4  ""
    . dynprop  \
          (#  \
            (call gms_textentry_var(callback, &variable, maxchars, type)))  \
          (__selected_object  \
            (= *   \
              (call gms_hilite_edge_selobj(2, 7, 3., 1.))))
endm
