// op_sub_fill_v.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_sub_fill_v: model
    . dynprop  \
          (*  \
            (call gms_hilite_fpercent1(&var, min, max)))
    fcolor 0
    fstyle 1
    finter 1
    fdir 0
    fpercent 100
    ecolor 0
    estyle 0
    ewidth 0
    rect 20 4 0 0 
    . tran -4.371139e-08 -1 0 1 -4.371139e-08 0
    . move 4 8.742212e-08
    fcolor 25
    fdir 1
    fpercent 66
    ecolor 25
    frect -9.646606e-02 3.89917 20.0786 0 
    . dynprop   \
          (fill_color  \
            (= *   \
              (fcolor fill_color)  \
              (ecolor fill_color)))
    . tran -4.356403e-08 -0.971568 0 0.98352 -4.191001e-08 0
    . move 3.89006 0.187503
    . refpoint 3.89006 9.262085e-02
    fcolor 15
    fdir 0
    fpercent 100
    ecolor 15
    estyle 1
    ewidth 2
    line \
        4 20 0 20 0 20 \
        0 20 0 20 0 20 \
        0 20 0 20 0 20 \
        0 20 0 20 0 0 
    fcolor 12
    ecolor 12
    line 4 20 4 0 0 0 
endm
