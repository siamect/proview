// op_gis_slider.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_gis_slider: model
    . dynprop  \
          (#  \
            (call gms_slider_var1(callback, &var, min, max))) \
          (*  \
            (call gms_hilite_percent1(&var, min, max)))
    bg: group
        fcolor 13
        fstyle 1
        finter 1
        fdir 0
        fpercent 100
        ecolor 13
        estyle 0
        ewidth 1
        bg_plate: frect 2.5 32.5 -0.5 -0.5 
        fcolor 15
        ecolor 15
        estyle 1
        ewidth 3
        bg_topshadow: line 2.5 32.5 -0.5 32.5 -0.5 -0.5 
        fcolor 12
        ecolor 12
        bg_botshadow: line 2.5 32.5 2.5 -0.5 -0.5 -0.5 
    endg
    slider: group
    . refpoint 0 2
        fcolor 13
        ecolor 13
        estyle 0
        ewidth 1
        slider_plate: frect 0 0 2 4 
        ecolor 15
        estyle 1
        ewidth 2
        slider_botshadow: line 0 0 2 0 2 4 
        ecolor 12
        slider_topshadow: line 0 0 0 4 2 4 
        fcolor 15
        ecolor 15
        ewidth 1
        frect 2.00429 2.15848 0 2.03992 
        fcolor 12
        ecolor 12
        frect 2.0074 2.03703 0 1.91992 
    endg
    vis 0
    fcolor 1
    ref_low: line 0 2 2 2 
    . refpoint 0 2
    ref_high: line 0 30 2 30 
    . refpoint 0 30
endm
