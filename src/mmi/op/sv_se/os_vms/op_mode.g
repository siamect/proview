// 
//  Proview   $Id: op_mode.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_mode: model
    . backgrflag 1
    fcolor 13
    fstyle 0
    finter 1
    fdir 0
    fpercent 100
    ecolor 13
    estyle 0
    ewidth 1
    bg: frect -10.8236 86.0686 116.828 -15.43 
    more: inst op_gis_button1 0 0
    . move 44.6088 1.8427
    renamedvars  \
        button_label :: "Mer..." \
        button_state :: 0 \
        callback :: mode_More \
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
    text "Bör" 20.3748 15.1933
    inst op_sub_fill_v 0 0
    . move 18.0079 19.5335
    . scale 18.0079 19.5335 1 1.85807
    . refpoint 18.0079 19.5335
    renamedvars  \
        fill_color :: 4 \
        max :: SetMax \
        min :: SetMin \
        var :: Set
    inst op_gis_slider 0 0
    . dbflag 1
    . move 13.1444 17.1593
    . scale 13.1444 17.1593 1 1.30969
    . refpoint 13.1444 17.1593
    renamedvars  \
        callback :: mode_SliderChangeUc \
        max :: SetMax \
        min :: SetMin \
        var :: SetSet
    cancel: inst op_gis_button1 0 0
    . move 84.0674 1.8427
    renamedvars  \
        button_label :: "Avbryt" \
        button_state :: 0 \
        callback :: pwr_Quit \
        reason :: 1
    group
    . refpoint 1.06154 68.0444
        fcolor 4
        fstyle 1
        ecolor 15
        estyle 1
        ewidth 2
        line 1.06168 9.41072 1.06168 68.0446 56.4803 68.0446 
        ecolor 12
        line 1.06168 9.41072 56.4803 9.41072 56.4803 68.0446 
    endg
    inst op_sub_stsline 0 0
    . move 25.3751 71.8004
    . scale 25.3751 71.8004 0.746794 0.808843
    . refpoint 25.3751 71.8004
    dec10: inst op_gis_button1 0 0
    . move 61.5378 29.5815
    renamedvars  \
        button_label :: "-10%" \
        button_state :: 0 \
        callback :: mode_DecUc10 \
        reason :: 1
    dec1: inst op_gis_button1 0 0
    . move 61.5378 35.6749
    renamedvars  \
        button_label :: "-1%" \
        button_state :: 0 \
        callback :: mode_DecUc1 \
        reason :: 1
    inc1: inst op_gis_button1 0 0
    . move 61.5378 43.317
    renamedvars  \
        button_label :: "+1%" \
        button_state :: 0 \
        callback :: mode_IncUc1 \
        reason :: 1
    inc10: inst op_gis_button1 0 0
    . move 61.5378 49.476
    renamedvars  \
        button_label :: "+10%" \
        button_state :: 0 \
        callback :: mode_IncUc10 \
        reason :: 1
    text "Bör" 67.0378 65.5658
    tega1: inst op_gis_infield 0 0
    . move 68.6982 59.1779
    renamedvars  \
        callback :: mode_NewUcVal \
        edge_color :: 13 \
        edge_color_select :: 2 \
        edge_width :: 2 \
        maxchar :: 6 \
        text_color_select :: 7 \
        textheight :: 1.75
    group
    . refpoint 57.7583 68.0444
        ecolor 15
        line 57.7583 20.6335 57.7583 68.0446 76.6959 68.0446 
        ecolor 12
        line 57.7583 20.6335 76.6959 20.6335 76.6959 68.0446 
    endg
    dec10: inst op_gis_button1 0 0
    . move 61.5378 22.0557
    renamedvars  \
        button_label :: "Linjal" \
        button_state :: 0 \
        callback :: mode_SaveSetSlider \
        reason :: 1
    inst op_sub_fill_v 0 0
    . move 35.1447 19.5335
    . scale 35.1447 19.5335 1 1.85807
    . refpoint 35.1447 19.5335
    renamedvars  \
        fill_color :: 14 \
        max :: OutMax \
        min :: OutMin \
        var :: Out
    text "Ut" 37.5116 15.1933
    inst op_sub_fill_v 0 0
    . move 26.4474 19.5335
    . scale 26.4474 19.5335 1 1.85807
    . refpoint 26.4474 19.5335
    renamedvars  \
        fill_color :: 25 \
        max :: SetMax \
        min :: SetMin \
        var :: Act
    text "Är" 28.8143 15.1933
    pid: inst op_gis_button1 0 0
    . move 60.2748 1.8427
    renamedvars  \
        button_label :: "PID..." \
        button_state :: 0 \
        callback :: mode_StartPid \
        reason :: 1
    fcolor 25
    fstyle 0
    ecolor 25
    estyle 0
    ewidth 1
    height 2
    ftrect 20.6347 66.4495 36.6347 62.4495  "Varde"
    . dynprop  \
          (*  \
            (stext ActValStr "%s"))
    fcolor 4
    fstyle 1
    finter 2
    ecolor 4
    ftrect 2.63466 66.4495 18.6347 62.4495  "Varde"
    . dynprop  \
          (*  \
            (stext SetValStr "%s"))
    fcolor 14
    fstyle 0
    finter 1
    ecolor 14
    ftrect 38.6347 66.4495 54.6347 62.4495  "Varde"
    . dynprop  \
          (*  \
            (stext OutValStr "%s"))
    group
    . refpoint 57.7583 20.0123
        fcolor 4
        fstyle 1
        ecolor 15
        estyle 1
        ewidth 2
        line 57.7583 9.41072 57.7583 20.0123 98.8285 20.0123 
        ecolor 12
        line 57.7583 9.41072 98.8285 9.41072 98.8285 20.0123 
    endg
    fcolor 13
    ecolor 13
    ewidth 1
    frect -1.62375 5.95425 37.3763 0.454254 
    height 1.5
    text "Externt Bör:" 8.52776 4.66493
    group
    . refpoint 1.06168 8.13274
        fcolor 4
        ecolor 15
        ewidth 2
        line 1.06177 0.697098 1.06177 8.13277 42.1319 8.13277 
        ecolor 12
        line 1.06177 0.697098 42.1319 0.697098 42.1319 8.13277 
    endg
    finter 2
    ecolor 4
    estyle 0
    ewidth 1
    height 2
    ftrect 19.026 6.16493 38.026 2.66493  "Varde"
    . dynprop  \
          (*  \
            (stext XSetValStr "%s"))
    group
        fcolor 14
        fstyle 0
        finter 1
        ecolor 14
        poly \
            18.526 6.66493 19.026 6.16493 19.026 2.66493 \
            18.526 2.16493 
        . filled 1
        fcolor 15
        ecolor 15
        poly \
            18.526 6.66493 38.526 6.66493 38.026 6.16493 \
            19.026 6.16493 
        . filled 1
        fcolor 14
        ecolor 14
        poly \
            38.526 6.66493 38.526 2.16493 38.026 2.66493 \
            38.026 6.16493 
        . filled 1
        fcolor 12
        ecolor 12
        poly \
            18.526 2.16493 38.526 2.16493 38.026 2.66493 \
            19.026 2.66493 
        . filled 1
    endg
    group
    . dynprop   \
          ((SetMax || SetMin)  \
            (= *   \
              (batcherase)  \
              (redraw)))
        fcolor 13
        fstyle 1
        ecolor 7
        scale_rect: frect 1.58534 59.9788 11.8664 16.2944 
        . refpoint 1.58534 59.9788
        inst op_gra_yaxis_left 0 0
        . move 11.7785 19.7056
        . scale 11.7785 19.7056 1 0.924734
        . refpoint 11.7785 19.7056
        renamedvars  \
            text_height :: 2. \
            y_max_value :: SetMax \
            y_min_value :: SetMin \
            y_tickmajor :: (SetMax - SetMin) / 2 \
            y_tickminor :: (SetMax - SetMin) / 4
    endg
    group
    . dynprop   \
          ((OutMax || OutMin)  \
            (= *   \
              (batcherase)  \
              (redraw)))
        scale_rect: frect 45.6753 60.095 55.9564 16.2944 
        . refpoint 45.6753 60.095
        inst op_gra_yaxis_right 0 0
        . move 45.7739 19.6484
        . scale 45.7739 19.6484 1 0.923294
        . refpoint 45.7739 19.6484
        renamedvars  \
            text_height :: 2. \
            y_max_value :: OutMax \
            y_min_value :: OutMin \
            y_tickmajor :: (OutMax - OutMin) / 2 \
            y_tickminor :: (OutMax - OutMin) / 4
    endg
    group
    . dynprop   \
          (Mode  \
            (= *   \
              (batcherase)  \
              (redraw)))
        fcolor 4
        rect 40.2221 61.1406 45.7989 15.7135 
        inst op_gis_slider 0 0
        . dynprop  \
              (*  \
                (vis (Mode == 1)))
        . dbflag 1
        . move 42.1899 17.1593
        . scale 42.1899 17.1593 1 1.30969
        . refpoint 42.1899 17.1593
        renamedvars  \
            callback :: mode_SliderChangeU \
            max :: OutMax \
            min :: OytMin \
            var :: SetOut
    endg
    group
    . dynprop   \
          (Mode  \
            (= *   \
              (batcherase)  \
              (redraw)))
        fcolor 13
        frect 80.421 67.763 98.313 21.2903 
        group
        . dynprop  \
              (*  \
                (vis (Mode == 1)))
            height 3
            text "Ut" 89.1705 65.5658
            tegb1: inst op_gis_infield 0 0
            . move 90.8309 59.1779
            renamedvars  \
                callback :: mode_NewUVal \
                edge_color :: 13 \
                edge_color_select :: 2 \
                edge_width :: 2 \
                maxchar :: 6 \
                text_color_select :: 7 \
                textheight :: 1.75
            inc10: inst op_gis_button1 0 0
            . move 83.6705 49.476
            renamedvars  \
                button_label :: "+10%" \
                button_state :: 0 \
                callback :: mode_IncU10 \
                reason :: 1
            inc1: inst op_gis_button1 0 0
            . move 83.6705 43.317
            renamedvars  \
                button_label :: "+1%" \
                button_state :: 0 \
                callback :: mode_IncU1 \
                reason :: 1
            dec1: inst op_gis_button1 0 0
            . move 83.6705 35.6749
            renamedvars  \
                button_label :: "-1%" \
                button_state :: 0 \
                callback :: mode_DecU1 \
                reason :: 1
            dec10: inst op_gis_button1 0 0
            . move 83.6705 29.5815
            renamedvars  \
                button_label :: "-10%" \
                button_state :: 0 \
                callback :: mode_DecU10 \
                reason :: 1
            dec10: inst op_gis_button1 0 0
            . move 83.6705 22.0557
            renamedvars  \
                button_label :: "Linjal" \
                button_state :: 0 \
                callback :: mode_SaveOutSlider \
                reason :: 1
            group
            . refpoint 79.8909 68.0444
                fcolor 4
                ecolor 15
                estyle 1
                ewidth 2
                line 79.8909 20.6335 79.8909 68.0446 98.8285 68.0446 
                ecolor 12
                line 79.8909 20.6335 98.8285 20.6335 98.8285 68.0446 
            endg
        endg
    endg
    inst op_gis_button_on_off 0 0
    . move 78.2929 14.7115
    . scale 78.2929 14.7115 0.780442 0.780442
    . refpoint 78.2929 14.7115
    renamedvars  \
        button_label :: "Auto" \
        button_state :: Mode == 2 \
        callback :: mode_ModAuto
    inst op_gis_button_on_off 0 0
    . move 65.469 14.7115
    . scale 65.469 14.7115 0.780442 0.780442
    . refpoint 65.469 14.7115
    renamedvars  \
        button_label :: "Man" \
        button_state :: Mode == 1 \
        callback :: mode_ModMan \
        reason :: 1
    inst op_gis_button_on_off 0 0
    . move 91.1906 14.7115
    . scale 91.1906 14.7115 0.780442 0.780442
    . refpoint 91.1906 14.7115
    renamedvars  \
        button_label :: "Kaskad" \
        button_state :: Mode == 4 \
        callback :: mode_ModCasc \
        reason :: 1
    fcolor 14
    ecolor 14
    estyle 0
    ewidth 1
    poly \
        2.13466 66.9495 2.63466 66.4495 2.63466 62.4495 \
        2.13466 61.9495 
    . filled 1
    fcolor 15
    ecolor 15
    poly \
        2.13466 66.9495 19.1347 66.9495 18.6347 66.4495 \
        2.63466 66.4495 
    . filled 1
    fcolor 12
    ecolor 12
    poly \
        2.13466 61.9495 19.1347 61.9495 18.6347 62.4495 \
        2.63466 62.4495 
    . filled 1
    fcolor 14
    ecolor 14
    poly \
        19.1347 66.9495 19.1347 61.9495 18.6347 62.4495 \
        18.6347 66.4495 
    . filled 1
    poly \
        20.6347 66.4495 20.1347 66.9495 20.1347 61.9495 \
        20.6347 62.4495 
    . filled 1
    fcolor 15
    ecolor 15
    poly \
        20.1347 66.9495 37.1347 66.9495 36.6347 66.4495 \
        20.6347 66.4495 
    . filled 1
    fcolor 12
    ecolor 12
    poly \
        20.1347 61.9495 37.1347 61.9495 36.6347 62.4495 \
        20.6347 62.4495 
    . filled 1
    fcolor 14
    fstyle 0
    ecolor 14
    poly \
        37.1347 66.9495 37.1347 61.9495 36.6347 62.4495 \
        36.6347 66.4495 
    . filled 1
    poly \
        38.1347 66.9495 38.6347 66.4495 38.6347 62.4495 \
        38.1347 61.9495 
    . filled 1
    fcolor 15
    ecolor 15
    poly \
        38.1347 66.9495 55.1347 66.9495 54.6347 66.4495 \
        38.6347 66.4495 
    . filled 1
    fcolor 12
    ecolor 12
    poly \
        38.1347 61.9495 55.1347 61.9495 54.6347 62.4495 \
        38.6347 62.4495 
    . filled 1
    fcolor 14
    ecolor 14
    poly \
        55.1347 66.9495 55.1347 61.9495 54.6347 62.4495 \
        54.6347 66.4495 
    . filled 1
    fcolor 13
    fstyle 1
    ecolor 7
    height 2
    font 1
    prec 0
    ftrect 1.88214 15.6846 14.1974 11.502  "Unit"
    . dynprop  \
          (*  \
            (stext SetUnit "%s"))
    ftrect 43.4752 15.6846 55.7905 11.502  "Unit"
    . dynprop  \
          (*  \
            (stext OutUnit "%s"))
    font 7
    prec 2
    align 1 3
    ftrect -31 2.5 31 -2.5  "Tvångsstyrd"
    . dynprop   \
          (Force   \
            (!= 1  \
              (stext "" "%s") ))
    . move 78.2069 71.8004
    . scale 78.2069 71.8004 0.361593 0.808843
    . refpoint 66.9975 73.8225
    group
    . dynprop  \
          (*  \
            (vis (Force)))
        fcolor 14
        ecolor 14
        poly \
            66.6241 74.2269 66.9975 73.8225 66.9975 69.7782 \
            66.6241 69.3738 
        . filled 1
        fcolor 12
        ecolor 12
        poly \
            66.6241 69.3738 87.1611 69.3738 86.8345 69.7782 \
            66.9975 69.7782 
        . filled 1
        fcolor 15
        ecolor 15
        poly \
            66.6241 74.2269 66.9975 73.8225 86.8345 73.8225 \
            87.2079 74.2269 
        . filled 1
        fcolor 14
        ecolor 14
        poly \
            89.4297 73.8225 89.4297 69.7782 89.8031 69.3738 \
            89.8031 74.2269 
        . filled 1
        . move -2.5952 0
        fcolor 10
        fstyle 0
        ecolor 10
        estyle 1
        fcir2 83.8772 72.1215 85.0093 72.1215 
        . move 9.31868 7.69151
        . scale 9.31868 7.69151 0.888901 0.888901
        . refpoint 9.31868 7.69151
    endg
endm
