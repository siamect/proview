// 
//  Proview   $Id: op_trend.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
// op_trend.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_trend: model
    SLIDER1DIAG: model
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
        group
            ecolor 7
            ewidth 1
            line -2 0 38 0 
            line 38 0 38 2 
            line -2 0 -2 2 
            line 18 0 18 2 
            fcolor 7
            line 2 0 2 1 
            line 6 0 6 1 
            line 10 0 10 1 
            line 14 0 14 1 
            line 22 0 22 1 
            line 26 0 26 1 
            line 30 0 30 1 
            line 34 0 34 1 
        endg
    endm
    TEXT_DISP_4: model
        fcolor 0
        ewidth 2
        tcolor 7
        height 2
        path 1
        font 7
        prec 2
        align 3 3
        size 0 0
        ftrect 51 11.25 35 14.25  "Value"
        . dynprop  \
              (*  \
                (stext y1_index "%7.2f"))
        ecolor 1
        ftrect 51 10.75 35 7.75  "Value"
        . dynprop  \
              (*  \
                (stext y2_index "%7.2f"))
        ecolor 4
        ftrect 51 7.25 35 4.25  "Value"
        . dynprop  \
              (*  \
                (stext y3_index "%7.2f"))
        ecolor 26
        ftrect 51 3.75 35 0.75  "Value"
        . dynprop  \
              (*  \
                (stext y4_index "%7.2f"))
        fstyle 0
        ecolor 7
        align 1 3
        ftrect 35 14.25 0.5 11.25  "Parameter"
        . dynprop  \
              (*  \
                (stext y1_name "%s"))
        fstyle 1
        ecolor 1
        ftrect 0.5 10.75 35 7.75  "Parameter"
        . dynprop  \
              (*  \
                (stext y2_name "%s"))
        ecolor 4
        ftrect 0.5 7.25 35 4.25  "Parameter"
        . dynprop  \
              (*  \
                (stext y3_name "%s"))
        fstyle 0
        ecolor 26
        ftrect 0.5 3.75 35 0.75  "Parameter"
        . dynprop  \
              (*  \
                (stext y4_name "%s"))
        fstyle 1
        ecolor 7
        ftrect 60 14.25 51 11.25  "Unit"
        . dynprop  \
              (*  \
                (stext y1_unit "%s"))
        ecolor 1
        ftrect 60 10.75 51 7.75  "Unit"
        . dynprop  \
              (*  \
                (stext y2_unit "%s"))
        ecolor 4
        ftrect 60 7.25 51 4.25  "Unit"
        . dynprop  \
              (*  \
                (stext y3_unit "%s"))
        ecolor 26
        ftrect 60 3.75 51 0.75  "Unit"
        . dynprop  \
              (*  \
                (stext y4_unit "%s"))
    endm
    fstyle 0
    ecolor 7
    ewidth 1
    frect 34 61 3 74 
    fcolor 3
    fstyle 1
    finter 2
    align 2 3
    ftrect 84 3 16 7  "Tid"
    . dynprop  \
          (*  \
            (stext x_time "%s"))
    vis 0
    fcolor 0
    fstyle 0
    finter 1
    font 1
    prec 0
    size 2 2
    ftrect 3 7 13 3  "Var"
    . dynprop  \
          (*  \
            (stext x_index "%d"))
    vis 1
    inst SLIDER1DIAG 0 0
    . move 19.6133 11.24
    . scale 19.6133 11.24 1.6941 1
    . refpoint 19.6133 11.24
    renamedvars  \
        callback :: trend_SliderFunction \
        max :: x_max_value \
        min :: x_min_value \
        var :: x_index
    fcolor 4
    fstyle 1
    line \
        13.36 2.84 13.36 5.03 86 5 \
        86 3 
    . move -0.100006 5.55
    inst op_gis_button0 0 0
    . move 4 68.7547
    . scale 4 68.7547 0.5 0.5
    . refpoint 4 68.7547
    renamedvars  \
        button_label :: "IN" \
        callback :: trend_ZoomIn
    inst op_gis_button0 0 0
    . move 10 68.7547
    . scale 10 68.7547 0.5 0.5
    . refpoint 10 68.7547
    renamedvars  \
        button_label :: "UT" \
        callback :: trend_ZoomOut
    inst op_gis_button0 0 0
    . move 16 68.7547
    . scale 16 68.7547 0.5 0.5
    . refpoint 16 68.7547
    renamedvars  \
        button_label :: "<" \
        callback :: trend_ScrollBack
    inst op_gis_button0 0 0
    . move 22 68.7547
    . scale 22 68.7547 0.5 0.5
    . refpoint 22 68.7547
    renamedvars  \
        button_label :: ">" \
        callback :: trend_ScrollForward
    inst op_gis_button0 0 0
    . move 28 68.7547
    . scale 28 68.7547 0.5 0.5
    . refpoint 28 68.7547
    renamedvars  \
        button_label :: "NU" \
        callback :: trend_ScrollReset
    inst TEXT_DISP_4 0 0
    . move 36 60
    inst op_gis_button1 0 0
    . move 88.326 1.52097
    renamedvars  \
        button_label :: "Mer..." \
        callback :: trend_More \
        reason :: 1
    inst op_gra_htrend4 0 0
    . move 15.7726 16.6778
    . scale 15.7726 16.6778 2.12755 1.73013
    . refpoint 15.7726 16.6778
    inst op_sub_stsline 0 0
    . move 18.242 63.6931
    . scale 18.242 63.6931 0.450777 0.5625
    . refpoint 18.242 63.6931
    fcolor 0
    fstyle 0
    ecolor 0
    font 7
    prec 2
    ftrect 3 7 13 3  "Tidsskala"
    . dynprop  \
          (*  \
            (stext x_timescale "%s"))
    . move 19.596 9.25
    . scale 19.596 9.25 3.788 1
    . refpoint 19.596 9.25
endm
