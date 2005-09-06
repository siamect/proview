// 
//  Proview   $Id: op_ai.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_ai: model
    . backgrflag 1
    detect 0
    fcolor 13
    fstyle 0
    finter 1
    fdir 0
    fpercent 100
    ecolor 13
    estyle 0
    ewidth 1
    bg: frect -10.9324 85.7959 116.719 -15.7027 
    detect 1
    more: inst op_gis_button1 0 0
    . move 44.5 1.57002
    renamedvars  \
        button_label :: "More..." \
        button_state :: 0 \
        callback :: ai_More \
        edge_width :: 3 \
        reason :: 1 \
        text_align_x :: 2 \
        text_height :: 2
    dec10: inst op_gis_button1 0 0
    . move 61.429 21.0831
    renamedvars  \
        button_label :: "-10%" \
        button_state :: 0 \
        callback :: ai_Dec10 \
        edge_width :: 3 \
        reason :: 1 \
        text_align_x :: 2 \
        text_height :: 2
    dec1: inst op_gis_button1 0 0
    . move 61.429 27.6931
    renamedvars  \
        button_label :: "-1%" \
        button_state :: 0 \
        callback :: ai_Dec1 \
        edge_width :: 3 \
        reason :: 1 \
        text_align_x :: 2 \
        text_height :: 2
    inc1: inst op_gis_button1 0 0
    . move 61.429 36.404
    renamedvars  \
        button_label :: "+1%" \
        button_state :: 0 \
        callback :: ai_Inc1 \
        edge_width :: 3 \
        reason :: 1 \
        text_align_x :: 2 \
        text_height :: 2
    inc10: inst op_gis_button1 0 0
    . move 61.429 43.3777
    renamedvars  \
        button_label :: "+10%" \
        button_state :: 0 \
        callback :: ai_Inc10 \
        edge_width :: 3 \
        reason :: 1 \
        text_align_x :: 2 \
        text_height :: 2
    tcolor 7
    bcolor 0
    height 3
    path 1
    font 7
    prec 2
    align 2 3
    size 0 0
    text "Change" 66.2636 62.2714
    . move 0.665405 0
    text "Act" 23.5756 16.3534
    . move 0.424393 -5.12395
    inst op_sub_fill_v 0 0
    . move 21.6331 15.5696
    . scale 21.6331 15.5696 1 1.85807
    . refpoint 21.6331 15.5696
    renamedvars  \
        fill_color :: 25 \
        max :: ActMax \
        min :: ActMin \
        var :: ActualValue
    inst op_gis_slider 0 0
    . dbflag 1
    . move 30.2305 13.1954
    . scale 30.2305 13.1954 1 1.30969
    . refpoint 30.2305 13.1954
    renamedvars  \
        callback :: ai_SliderChange \
        max :: ActMax \
        min :: ActMin \
        var :: SetActVal
    tega1: inst op_gis_infield 0 0
    . move 68.5894 53.9542
    renamedvars  \
        callback :: ai_NewActVal \
        edge_color :: 13 \
        edge_color_select :: 2 \
        edge_width :: 2 \
        maxchar :: 6 \
        text_color_select :: 7 \
        textheight :: 1.75
    group
    . move -7.099915e-02 -9.65953
    . scale -7.099915e-02 -9.65953 1 1.14636
    . refpoint 53.929 66
        fcolor 4
        fstyle 1
        ecolor 15
        estyle 1
        ewidth 2
        line 54 16 54 66 80 66 
        ecolor 12
        line 54 16 80 16 80 66 
    endg
    cancel: inst op_gis_button1 0 0
    . move 83.9586 1.57002
    renamedvars  \
        button_label :: "Cancel" \
        button_state :: 0 \
        callback :: pwr_Quit \
        reason :: 1
    group
    . move -92.9589 -9.65953
    . scale -92.9589 -9.65953 1.76455 1.14636
    . refpoint 2.32697 66
        ecolor 15
        line 54 16 54 66 80 66 
        ecolor 12
        line 54 16 80 16 80 66 
    endg
    dec10: inst op_gis_button1 0 0
    . move 61.429 10.0661
    renamedvars  \
        button_label :: "Slider" \
        button_state :: 0 \
        callback :: ai_Save \
        reason :: 1
    inst op_sub_outfield 0 0
    . move 24 61
    renamedvars  \
        fill_color :: 13 \
        format :: "%5.2f" \
        out_var :: ActualValue \
        text_color :: 7 \
        textheight :: 1.75
    fcolor 13
    ecolor 13
    estyle 0
    height 1.5
    font 1
    prec 0
    ftrect 34 39.5 47.5 35.5  "SlideVal"
    . dynprop  \
          (*  \
            (stext SetActVal "%5.2f"))
    . move 0 -3.5
    group
    . dynprop   \
          ((ActMax || ActMin)  \
            (= *   \
              (batcherase)  \
              (redraw)))
    . batcherase 1
        scale_rect: frect 9 57 20.5 9.5 
        inst op_gra_yaxis_left 0 0
        . move 19.1932 15.7417
        . scale 19.1932 15.7417 1 0.924734
        . refpoint 19.1932 15.7417
        renamedvars  \
            text_height :: 2. \
            y_max_value :: ActMax \
            y_min_value :: ActMin \
            y_tickmajor :: (ActMax - ActMin) / 2 \
            y_tickminor :: (ActMax - ActMin) / 4
    endg
    inst op_sub_stsline 0 0
    . move 25.2663 70.9871
    . scale 25.2663 70.9871 0.746794 0.808843
    . refpoint 25.2663 70.9871
    font 7
    text "Unit" 10.9443 11.2696
    . dynprop  \
          (*  \
            (stext Unit "%s"))
endm
