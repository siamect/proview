// op_gis_infield.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_gis_infield: model
    fcolor 13
    fstyle 1
    finter 1
    fdir 0
    fpercent 100
    ecolor 0
    estyle 1
    ewidth 0
    tcolor 7
    height 0
    path 1
    font 7
    prec 0
    align 2 3
    size 0 0
    ftrect 24.5782 43.4061 11.6551 47.1561  ""
    . dynprop  \
          (#  \
            (call gms_textentry_var(callback, &variable, maxchar, 256))) \
          (*  \
            (ecolor edge_color)  \
            (ewidth edge_width)  \
            (theight textheight))  \
          (__self   \
            (= __selected_object  \
              (ecolor edge_color_select)  \
              (tcolor text_color_select) ))
    . move -21.1263 -60.3746
    . scale -21.1263 -60.3746 1.08333 1.33333
    . refpoint -8 -2
    fcolor 14
    ecolor 14
    estyle 0
    ewidth 1
    poly \
        -9.5 3.5 -9.5 -3.5 -8.5 -2.5 \
        -8.5 2.5 
    . filled 1
    fcolor 15
    ecolor 15
    poly \
        -9.5 3.5 6.5 3.5 5.5 2.5 \
        -8.5 2.5 
    . filled 1
    fcolor 12
    ecolor 12
    poly \
        6.5 3.5 6.5 -3.5 5.5 -2.5 \
        5.5 2.5 
    . filled 1
    poly \
        -9.5 -3.5 6.5 -3.5 5.5 -2.5 \
        -8.5 -2.5 
    . filled 1
endm
