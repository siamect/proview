// op_gis_button0.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_gis_button0: model
    . dynprop  \
          (#  \
            (call gms_flash())  \
            (call gms_call(callback)))
    group
    . dynprop   \
          ((__button_hilite || button_state)  \
            (= *   \
              (batcherase)))
    . scale 0 0 0.9 2
    . refpoint 0 0
        state_1: group
        . dynprop   \
              ((__button_hilite || button_state)   \
                (= 0  \
                  (vis 0) )   \
                (= 1  \
                  (vis 1)  \
                  (redraw) ))
            fcolor 14
            fstyle 1
            finter 1
            fdir 0
            fpercent 100
            ecolor 7
            estyle 0
            ewidth 3
            tcolor 7
            height 1.5
            path 1
            font 1
            prec 0
            align 2 3
            size 0 0
            plate_1: ftrect 0 0 10 4  "X"
            . dynprop   \
                  (button_label  \
                    (= *   \
                      (stext button_label "%s")))
            fcolor 7
            ecolor 15
            estyle 1
            upper_1: line 0 0 0 4 10 4 
            ecolor 12
            lower_1: line 0 0 10 0 10 4 
        endg
        state_0: group
        . dynprop   \
              ((__button_hilite || button_state)   \
                (= 0  \
                  (vis 1)  \
                  (redraw) )   \
                (= 1  \
                  (vis 0) ))
            fcolor 13
            ecolor 7
            estyle 0
            font 7
            plate_0: ftrect 0 0 10 4  "X"
            . dynprop   \
                  (button_label  \
                    (= *   \
                      (stext button_label "%s")))
            fcolor 7
            ecolor 15
            estyle 1
            lower_0: line 0 0 10 0 10 4 
            ecolor 12
            upper_0: line 0 0 0 4 10 4 
        endg
    endg
endm
