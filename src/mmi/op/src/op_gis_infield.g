// 
//  Proview   $Id: op_gis_infield.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_gis_infield: model
    fcolor 13
    fstyle 1
    finter 1
    fdir 0
    fpercent 100
    ecolor 0
    estyle 1
    ewidth 0
    tcolor 7
    height 0
    path 1
    font 7
    prec 0
    align 2 3
    size 0 0
    ftrect 24.5782 43.4061 11.6551 47.1561  ""
    . dynprop  \
          (#  \
            (call gms_textentry_var(callback, &variable, maxchar, 256))) \
          (*  \
            (ecolor edge_color)  \
            (ewidth edge_width)  \
            (theight textheight))  \
          (__self   \
            (= __selected_object  \
              (ecolor edge_color_select)  \
              (tcolor text_color_select) ))
    . move -21.1263 -60.3746
    . scale -21.1263 -60.3746 1.08333 1.33333
    . refpoint -8 -2
    fcolor 14
    ecolor 14
    estyle 0
    ewidth 1
    poly \
        -9.5 3.5 -9.5 -3.5 -8.5 -2.5 \
        -8.5 2.5 
    . filled 1
    fcolor 15
    ecolor 15
    poly \
        -9.5 3.5 6.5 3.5 5.5 2.5 \
        -8.5 2.5 
    . filled 1
    fcolor 12
    ecolor 12
    poly \
        6.5 3.5 6.5 -3.5 5.5 -2.5 \
        5.5 2.5 
    . filled 1
    poly \
        -9.5 -3.5 6.5 -3.5 5.5 -2.5 \
        -8.5 -2.5 
    . filled 1
endm
