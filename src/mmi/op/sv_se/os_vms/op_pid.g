// 
//  Proview   $Id: op_pid.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_pid: model
    fcolor 13
    fstyle 1
    finter 2
    fdir 0
    fpercent 100
    ecolor 7
    estyle 0
    ewidth 1
    frect -21.5181 90.5699 122.531 -18.8162 
    fcolor 0
    finter 1
    estyle 1
    frect 96.9763 74.9179 71.4799 68.0226 
    more: inst op_gis_button1 0 0
    . move 39.8342 1.60115
    renamedvars  \
        button_label :: "Mer..." \
        callback :: pid_More \
        reason :: 1
    frect 3 36 95 10 
    . move 0 -2
    tcolor 7
    bcolor 0
    height 2
    path 1
    font 7
    prec 2
    align 1 3
    size 0 0
    text "Är:" 4.93251 31.622
    . move 0.966019 -0.717407
    text "Bör:" 4.93251 31.622
    . move 0.966019 -4.71741
    text "Ut:" 4.93251 31.622
    . move 0.966019 -8.71741
    text "Utx:" 4.93251 31.622
    . move 0.966019 -12.7174
    text "Fel:" 4.93251 31.622
    . move 0.966019 -16.7174
    text "Kp:" 4.93251 31.622
    . move 41.3713 -0.355835
    text "Ti:" 4.93251 31.622
    . move 41.3713 -4.35583
    text "Td:" 4.93251 31.622
    . move 41.3713 -8.35583
    text "Dg:" 4.93251 31.622
    . move 41.3713 -12.3558
    text "Bg:" 4.93251 31.622
    . move 41.3713 -16.3558
    text "Bias:" 4.93251 31.622
    . move 41.4326 -20.3381
    fcolor 6
    fcir2 91 30 91 28.5 
    . dynprop  \
          (*  \
            (filled (IntOff == 0)))
    . move 0 -3.5
    fcir2 91 30 91 28.5 
    . dynprop  \
          (*  \
            (filled (IntOff == 1)))
    . move 0 -10.5
    fcolor 2
    fcir2 91 30 91 28.5 
    . dynprop  \
          (*  \
            (filled (Force == 0)))
    . move 0 -18
    text "Reglering" 76.5 13.5
    . move -2.5 -1.5
    fcolor 4
    rect 73 33 94 9 
    height 1.5
    text "unit" 1.58244 54.8225
    . dynprop  \
          (*  \
            (stext OutUnit "%s"))
    . move 0.417557 -8.82249
    align 3 3
    text "unit" 94.8476 53.7547
    . dynprop  \
          (*  \
            (stext SetUnit "%s"))
    . move 3.1524 -7.7547
    height 2
    align 2 3
    text "PIDAlg" 74 72
    . dynprop  \
          (*  \
            (stext PIDAlg "%s"))
    . move 10 -2
    height 3
    font 1
    align 1 3
    text "I" 52.5 32
    . dynprop  \
          (*  \
            (vis Inverse))
    . move -0.5 -0.5
    height 2
    font 7
    align 2 3
    text "Regulatorfunktion" 74 72
    . move 10.1479 1.15401
    height 1.5
    text "XMinShow" 12.6296 37.0741
    . dynprop  \
          (*  \
            (stext XMinShow "%d"))
    . move 3.52782 -0.481064
    text "XMaxShow" 76.1302 36.4327
    . dynprop  \
          (*  \
            (stext XMaxShow "%d"))
    . move 6.89529 0.16037
    height 2
    align 1 3
    text "Av" 76.5 13.5
    . move 7.06461 6.34131
    text "På" 76.5 13.5
    . move 7.06461 13.3413
    fcolor 15
    finter 2
    align 2 3
    ftrect 2 40 6 36  "Ut"
    . move 0 -1.39667
    fcolor 25
    ftrect 89 40 93 36  "Är"
    . move 0 -1.39667
    fcolor 4
    ftrect 94 40 99 36  "Bör"
    . move 0 -1.39667
    fcolor 0
    finter 1
    estyle 0
    align 3 3
    ftrect 32 33 15 29  "Act"
    . dynprop  \
          (*  \
            (stext Act "%s"))
    . refpoint 15 29
    ftrect 32 33 15 29  "Set"
    . dynprop  \
          (*  \
            (stext Set "%s"))
    . move 0 -4
    . refpoint 15 25
    ftrect 32 33 15 29  "Out"
    . dynprop  \
          (*  \
            (stext Out "%s"))
    . move 0 -8
    . refpoint 15 21
    ftrect 32 33 15 29  "OutChange"
    . dynprop  \
          (*  \
            (stext OutChange "%s"))
    . move 0 -12
    . refpoint 15 17
    ftrect 32 33 15 29  "ControlDiff"
    . dynprop  \
          (*  \
            (stext ControlDiff "%s"))
    . move 0 -16
    . refpoint 15 13
    ftrect 32 29 15 33  "Gain"
    . dynprop  \
          (*  \
            (stext Gain "%s"))
    . move 39 0
    . refpoint 54 29
    ftrect 32 29 15 33  "IntTime"
    . dynprop  \
          (*  \
            (stext IntTime "%s"))
    . move 39 -4
    . refpoint 54 25
    ftrect 32 29 15 33  "DerTime"
    . dynprop  \
          (*  \
            (stext DerTime "%s"))
    . move 39 -8
    . refpoint 54 21
    ftrect 32 29 15 33  "DerGain"
    . dynprop  \
          (*  \
            (stext DerGain "%s"))
    . move 39 -12
    . refpoint 54 17
    ftrect 32 29 15 33  "BiasGain"
    . dynprop  \
          (*  \
            (stext BiasGain "%s"))
    . move 39 -16
    . refpoint 54 13
    ftrect 32 29 15 33  "Bias"
    . dynprop  \
          (*  \
            (stext Bias "%s"))
    . move 39 -20
    . refpoint 54 9
    align 1 3
    text "Integrering" 76.5 13.5
    . move -2.5 17.5
    fcolor 4
    estyle 1
    line 73 15 94 15 
    group
    . dynprop   \
          (sts  \
            (= *   \
              (batcherase)))
        fcolor 13
        finter 2
        estyle 0
        frect 0.667084 74.9179 67.6314 67.3492 
        . dynprop   \
              (sts  \
                (= *   \
                  (redraw)))
        inst op_sub_stsline 0 0
        . dynprop   \
              (sts  \
                (= *   \
                  (redraw)))
        . move 33.4757 70.428
    endg
    frect 0.795349 66.9643 0.795349 66.5794 
    group
    . dynprop   \
          (SetMin  \
            (= *   \
              (batcherase)))
    . batcherase 1
        group
        . dynprop   \
              (SetMin  \
                (= *   \
                  (redraw)))
        . move -0.128281 0
            frect 0.667084 67.0926 12.3409 50.1591 
            frect 1.56506 43.36 12.2126 38.9984 
            frect 86.874 67.4774 99.959 49.5177 
            frect 99.5741 44.3863 87.1305 39.1266 
        endg
        inst op_gra_htrend3 0 0
        . dynprop   \
              (SetMin  \
                (= *   \
                  (redraw)))
        . move 15.852 40.902
        . scale 15.852 40.902 2.1066 1.04751
        . refpoint 15.852 40.902
        renamedvars  \
            backgr_color :: 0 \
            num_points :: Xpoints \
            trace1_color :: 7 \
            trace1_y_value :: PlotOut \
            trace2_color :: 4 \
            trace2_y_value :: PlotSet \
            trace3_color :: 1 \
            trace3_y_value :: PlotAct \
            x_max_value :: Xmax \
            x_min_value :: Xmin \
            x_shift_val :: -1 \
            x_start :: xval \
            x_tickmajor :: Xtick \
            x_tickminor :: Xtick / 2 \
            y1_max_value :: OutMax \
            y1_min_value :: OutMin \
            y1_tickmajor :: (OutMax - OutMin) / 2 \
            y1_tickminor :: (OutMax - OutMin) / 4 \
            y2_max_value :: SetMax \
            y2_min_value :: SetMin \
            y2_tickmajor :: (SetMax - SetMin) / 2 \
            y2_tickminor :: (SetMax - SetMin) / 4 \
            y3_max_value :: SetMax \
            y3_min_value :: SetMin \
            yg_max_value :: 100 \
            yg_min_value :: 0 \
            yg_tickmajor :: 10
    endg
    mode: inst op_gis_button1 0 0
    . move 56.6805 1.60115
    renamedvars  \
        button_label :: "Mode..." \
        button_state :: 0 \
        callback :: pid_StartMode \
        reason :: 1
    cancel: inst op_gis_button1 0 0
    . move 84.5641 1.60115
    renamedvars  \
        button_label :: "Avbryt" \
        button_state :: 0 \
        callback :: pwr_Quit \
        reason :: 1
endm
