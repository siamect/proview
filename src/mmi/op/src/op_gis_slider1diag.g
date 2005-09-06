// 
//  Proview   $Id: op_gis_slider1diag.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_gis_slider1diag: model
    . dynprop  \
          (#  \
            (call gms_slider_var1(callback, &var, min, max))) \
          (*  \
            (call gms_hilite_percent1(&var, min, max)))
    fcolor 0
    fstyle 1
    finter 1
    fdir 0
    fpercent 100
    ecolor 0
    estyle 1
    ewidth 1
    poly \
        -4 -3 -2 -1 37.6 -1 \
        39 -3 
    . filled 1
    fcolor 4
    ecolor 4
    estyle 0
    ewidth 3
    poly -3.42 -2.42 -2 -1 -0.589996 -2.42 
    . filled 1
    . refpoint -2 -1
    vis 0
    estyle 1
    poly -2 -1.41 -3.41 -2.83 -0.589996 -2.83 
    . filled 1
    . refpoint -2 -1.41
    poly 36.18 -2.83 37.6 -1.41 39.01 -2.83 
    . filled 1
    . refpoint 37.6 -1.41
    vis 1
    ecolor 7
    ewidth 1
    line -2 0 38 0 
    line 38 0 38 2 
    line -2 0 -2 2 
    line 18 0 18 2 
endm
