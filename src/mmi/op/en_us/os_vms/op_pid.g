// op_pid.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
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
    ecolor 13
    estyle 0
    ewidth 1
    frect -29.8207 95.5514 120.04 -15.4951 
    inst op_gra_htrend3 0 0
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
    more: inst op_gis_button1 0 0
    . move 41.6931 1.60115
    renamedvars  \
        button_label :: "More..." \
        callback :: pid_More \
        reason :: 1
    fcolor 0
    finter 1
    ecolor 7
    estyle 1
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
    text "ProcVal:" 4.93251 31.622
    . move 0.966019 -0.717407
    text "SetVal:" 4.93251 31.622
    . move 0.966019 -4.71741
    text "OutVal:" 4.93251 31.622
    . move 0.966019 -8.71741
    text "OutChange:" 4.93251 31.622
    . move 0.966019 -12.7174
    text "ControlDiff:" 4.93251 31.622
    . move 0.966019 -16.7174
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
    text "Control" 76.5 13.5
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
    height 3
    font 1
    align 1 3
    text "I" 52.5 32
    . dynprop  \
          (*  \
            (vis Inverse))
    . move -0.5 -0.5
    height 1.5
    font 7
    align 2 3
    text "XMinShow" 12.6296 37.0741
    . dynprop  \
          (*  \
            (stext XMinShow "%d"))
    . move 3.52782 -0.481064
    text "XMaxShow" 76.1302 36.4327
    . dynprop  \
          (*  \
            (stext XMaxShow "%d"))
    . move 6.4142 0.481049
    height 2
    align 1 3
    text "Off" 76.5 13.5
    . move 7.06461 6.34131
    text "On" 76.5 13.5
    . move 7.06461 13.3413
    fcolor 15
    finter 2
    align 2 3
    ftrect 2 40 6 36  "Out"
    . move -3.31711 2.12741
    . scale -3.31711 2.12741 1.82928 0.944016
    . refpoint -3.31711 2.12741
    fcolor 1
    ftrect 89 40 93 36  "Act"
    . move -47.2959 2.75795
    . scale -47.2959 2.75795 1.57307 1.04231
    . refpoint -47.2959 2.75795
    fcolor 4
    ftrect 94 40 99 36  "Set"
    . move -25.5875 0.994324
    . scale -25.5875 0.994324 1.25846 0.962128
    . refpoint -25.5875 0.994324
    fcolor 0
    finter 1
    align 3 3
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
    text "Integration" 76.5 13.5
    . move -2.5 17.5
    fcolor 4
    line 73 15 94 15 
    group
        fcolor 0
        frect 96.9763 74.9179 71.4799 68.0226 
        align 2 3
        text "Controller function" 74 72
        . move 10.1479 1.15401
        text "PIDAlg" 74 72
        . dynprop  \
              (*  \
                (stext PIDAlg "%s"))
        . move 10 -2
    endg
    align 1 3
    text "Gain:" 4.93251 31.622
    . move 35.6787 -0.596359
    text "IntTime:" 4.93251 31.622
    . move 35.6787 -4.59636
    text "DerTime:" 4.93251 31.622
    . move 35.6787 -8.59636
    text "DerGain:" 4.93251 31.622
    . move 35.6787 -12.5963
    text "BiasGain:" 4.93251 31.622
    . move 35.6787 -16.5963
    text "Bias:" 4.93251 31.622
    . move 35.74 -20.5786
    align 3 3
    ftrect 32 33 15 29  "Act"
    . dynprop  \
          (*  \
            (stext Act "%s"))
    . move 5.8065 -0.121765
    . refpoint 20.8065 28.8782
    ftrect 32 33 15 29  "Set"
    . dynprop  \
          (*  \
            (stext Set "%s"))
    . move 5.8065 -4.12177
    . refpoint 20.8065 24.8782
    ftrect 32 33 15 29  "Out"
    . dynprop  \
          (*  \
            (stext Out "%s"))
    . move 5.8065 -8.12177
    . refpoint 20.8065 20.8782
    ftrect 32 33 15 29  "OutChange"
    . dynprop  \
          (*  \
            (stext OutChange "%s"))
    . move 5.8065 -12.1218
    . refpoint 20.8065 16.8782
    ftrect 32 33 15 29  "ControlDiff"
    . dynprop  \
          (*  \
            (stext ControlDiff "%s"))
    . move 5.8065 -16.1218
    . refpoint 20.8065 12.8782
    group
    . dynprop   \
          (sts  \
            (= *   \
              (batcherase)  \
              (redraw)))
    . batcherase 1
        fcolor 13
        finter 2
        estyle 0
        frect 1.43678 74.0199 65.5788 66.9643 
        inst op_sub_stsline 0 0
        . move 33.4757 70.428
    endg
    mode: inst op_gis_button1 0 0
    . move 59.1203 1.60115
    renamedvars  \
        button_label :: "Mode..." \
        button_state :: 0 \
        callback :: pid_StartMode \
        reason :: 1
    cancel: inst op_gis_button1 0 0
    . move 84.448 1.60115
    renamedvars  \
        button_label :: "Cancel" \
        button_state :: 0 \
        callback :: pwr_Quit \
        reason :: 1
endm
