// op_gis_button_on_off.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_gis_button_on_off: model
    . dynprop  \
          (#  \
            (call gms_flash())  \
            (call gms_call1(callback, reason)))
    group
        fcolor 13
        fstyle 1
        finter 1
        fdir 0
        fpercent 100
        ecolor 13
        estyle 0
        ewidth 1
        frect -5.5 5 5.5 -5 
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 14) )   \
                (= 0  \
                  (fcolor 13) ))
        tcolor 7
        bcolor 0
        height 2
        path 1
        font 7
        prec 0
        align 2 3
        size 0 0
        text "Label" 1.678467e-04 2.09593
        . dynprop   \
              (__button_hilite  \
                (= *   \
                  (redraw)))  \
              (button_label  \
                (= *   \
                  (stext button_label "%s")))
        . move -1.678467e-04 -9.593201e-02
        fcolor 12
        fstyle 0
        ecolor 12
        poly \
            -6 5.5 -5.5 5 5.5 5 \
            6 5.5 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 15) )   \
                (= 0  \
                  (fcolor 12) ))
        fcolor 14
        ecolor 14
        poly \
            -5.5 5 -5.5 -5 -6 -5.5 \
            -6 5.5 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 13) )   \
                (= 0  \
                  (fcolor 14) ))
        poly \
            5.5 5 6 5.5 6 -5.5 \
            5.5 -5 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 14) )   \
                (= 0  \
                  (fcolor 14) ))
        fcolor 15
        ecolor 15
        poly \
            -6 -5.5 -5.5 -5 5.5 -5 \
            6 -5.5 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 12) )   \
                (= 0  \
                  (fcolor 15) ))
        fcolor 4
        ecolor 4
        led: frect 2 -3 -2 -1.5 
        . dynprop   \
              (button_state   \
                (= 1  \
                  (fcolor 4) )   \
                (= 0  \
                  (fcolor 13) ))
        fcolor 13
        ecolor 6
        line -1 -1 -1 -1.5 
        fcolor 15
        ecolor 15
        poly \
            -2.5 -1 -2 -1.5 2 -1.5 \
            2.5 -1 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 13) )   \
                (= 0  \
                  (fcolor 15) ))
        fcolor 12
        ecolor 12
        poly \
            -2.5 -3.5 -2 -3 2 -3 \
            2.5 -3.5 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 14) )   \
                (= 0  \
                  (fcolor 12) ))
        fcolor 14
        ecolor 14
        poly \
            -2 -1.5 -2 -3 -2.5 -3.5 \
            -2.5 -1 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 15) )   \
                (= 0  \
                  (fcolor 14) ))
        poly \
            2 -3 2 -1.5 2.5 -1 \
            2.5 -3.5 
        . filled 1
        . dynprop   \
              (__button_hilite   \
                (= 1  \
                  (fcolor 14) )   \
                (= 0  \
                  (fcolor 14) ))
    endg
endm
