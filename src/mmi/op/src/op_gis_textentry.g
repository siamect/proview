// op_gis_textentry.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_gis_textentry: model
    fcolor 11
    fstyle 1
    finter 2
    fdir 0
    fpercent 100
    ecolor 7
    estyle 1
    ewidth 1
    tcolor 7
    height 2
    path 1
    font 7
    prec 0
    align 2 3
    size 0 0
    ftrect 0 0 16 4  ""
    . dynprop  \
          (#  \
            (call gms_textentry_var(callback, &variable, maxchars, type)))  \
          (__selected_object  \
            (= *   \
              (call gms_hilite_edge_selobj(2, 7, 3., 1.))))
endm
