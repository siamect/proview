// op_gis_textentry1.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_gis_textentry1: model
    fcolor 0
    fstyle 1
    finter 1
    fdir 0
    fpercent 100
    ecolor 7
    estyle 1
    ewidth 1
    tcolor 7
    height 0
    path 1
    font 0
    prec 2
    align 2 3
    size 2 2
    ftrect -5 4 1 1  "Text"
    . dynprop  \
          (#  \
            (call gms_textentry_var(callback, &variable, maxchars, type))) \
          (*  \
            (tfont font)  \
            (theight size))  \
          (__selected_object  \
            (= *   \
              (call gms_hilite_edge_selobj(2, 7, 3., 1.))))   \
          ((upperlimit == 0)   \
            (= 1  \
              (tfont font) ))  \
          ((lowerlimit == 0)   \
            (= 1  \
              (theight size) ))
    . move 2 -2.5
endm
