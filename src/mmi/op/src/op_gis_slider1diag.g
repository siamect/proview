// op_gis_slider1diag.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
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
