// op_sub_outfield.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_sub_outfield: model
    fcolor 13
    fstyle 1
    finter 1
    fdir 0
    fpercent 100
    ecolor 2
    estyle 0
    ewidth 0
    tcolor 7
    height 1.75
    path 1
    font 7
    prec 0
    align 2 3
    size 0 0
    ftrect 28.2705 47.1561 11.6551 43.4061  "OutField"
    . dynprop  \
          (*  \
            (fcolor fill_color)  \
            (theight textheight)  \
            (tcolor text_color)  \
            (stext out_var format))
    . move -21.1263 -60.3746
    . scale -21.1263 -60.3746 1.08333 1.33333
    . refpoint -8 -2
    fcolor 14
    ecolor 14
    ewidth 1
    poly \
        -9.5 3.5 -9.5 -3.5 -8.5 -2.5 \
        -8.5 2.5 
    . filled 1
    fcolor 15
    ecolor 15
    poly \
        -9.5 3.5 10.5 3.5 9.5 2.5 \
        -8.5 2.5 
    . filled 1
    fcolor 12
    ecolor 12
    poly \
        10.5 3.5 10.5 -3.5 9.5 -2.5 \
        9.5 2.5 
    . filled 1
    poly \
        -9.5 -3.5 10.5 -3.5 9.5 -2.5 \
        -8.5 -2.5 
    . filled 1
endm
