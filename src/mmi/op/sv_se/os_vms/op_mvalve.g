// op_mvalve.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
mtran0
vis 1
detect 1
op_mvalve: model
    fcolor 13
    fstyle 1
    finter 2
    fdir 0
    fpercent 100
    ecolor 7
    estyle 0
    ewidth 1
    frect -21.9333 89.5321 125.022 -19.0237 
    inst op_gis_button1 0 0
    . move 87.5243 2.48309
    renamedvars  \
        button_label :: "Mer..." \
        callback :: mvalve_More \
        reason :: 1
    inst op_gis_button1 0 0
    . move 77.1857 44.7659
    . scale 77.1857 44.7659 1.28371 1.28371
    . refpoint 77.1857 44.7659
    renamedvars  \
        button_label :: "Man" \
        callback :: mvalve_Man \
        reason :: 1
    inst op_gis_button1 0 0
    . move 51.6603 44.5601
    . scale 51.6603 44.5601 1.28371 1.28371
    . refpoint 51.6603 44.5601
    renamedvars  \
        button_label :: "Auto" \
        callback :: mvalve_Auto \
        reason :: 1
    fcolor 6
    finter 1
    ecolor 4
    estyle 1
    fcir2 88.1568 53.9114 89.7604 54.0717 
    . dynprop   \
          (ManMod   \
            (= 1  \
              (fcolor 2) )   \
            (= 0  \
              (fcolor 0) )   \
            (= 2  \
              (fcolor 7) ))
    . move -29.3953 -9.41385
    . scale -29.3953 -9.41385 1.28371 1.28371
    . refpoint -29.3953 -9.41385
    fcir2 88.1568 53.9114 89.7604 54.0717 
    . dynprop   \
          (ManMod   \
            (= 1  \
              (fcolor 0) )   \
            (= 0  \
              (fcolor 2) )   \
            (= 2  \
              (fcolor 7) ))
    . move -55.3324 -9.41385
    . scale -55.3324 -9.41385 1.28371 1.28371
    . refpoint -55.3324 -9.41385
    line 10 62 42 62 
    . move 0 2
    line 10 62 42 62 
    . tran 1 0 0 0 -1 0
    . move 0 78
    fcolor 17
    fstyle 0
    poly \
        42 45 44 45 44 50 \
        45 50 43 52 41 50 \
        42 50 
    . filled 1
    . dynprop   \
          (OrderOpen   \
            (!= 1  \
              (fcolor 0) )   \
            (= 1  \
              (fcolor 17) ))
    . tran 1.44319 0 0 0 1.44319 0
    . move -20.0572 -21.9948
    poly \
        42 45 44 45 44 50 \
        45 50 43 52 41 50 \
        42 50 
    . filled 1
    . dynprop   \
          (OrderClose   \
            (!= 1  \
              (fcolor 0) )   \
            (= 1  \
              (fcolor 17) ))
    . tran 1.44319 0 0 0 -1.44319 0
    . move -20.0572 101.995
    fcolor 0
    tcolor 7
    height 3
    path 1
    font 1
    prec 2
    align 2 3
    size 0 0
    ftrect 37.3162 42.7482 14.8671 34  "LOCAL"
    . dynprop   \
          (ProdOpen   \
            (= 0  \
              (stext " " "%s") ))  \
          (ProdOpen   \
            (= 1  \
              (stext "DRIFT" "%s") ))  \
          (ProdClose   \
            (= 1  \
              (stext "DRIFT" "%s") ))  \
          (ProdStop   \
            (= 1  \
              (stext "DRIFT" "%s") ))  \
          (Local   \
            (= 1  \
              (stext "LOKAL" "%s") ))  \
          (SafeOpen   \
            (= 1  \
              (stext "SÄK" "%s") ))  \
          (SafeClose   \
            (= 1  \
              (stext "SÄK" "%s") ))  \
          (SafeStop   \
            (= 1  \
              (stext "SÄK" "%s") ))
    ftrect 14 64 38 56  "DRIFT"
    . dynprop   \
          (EndOpen   \
            (!= 1  \
              (stext " " "%s") ))  \
          (EndOpen   \
            (= 1  \
              (stext "ÖPPEN" "%s") ))
    ftrect 14 16 38 24  "STOPP"
    . dynprop   \
          (EndClose   \
            (!= 1  \
              (stext " " "%s") ))  \
          (EndClose   \
            (= 1  \
              (stext "STÄNGD" "%s") ))
    inst op_gis_button1 0 0
    . move 78.6959 32.7445
    renamedvars  \
        button_label :: "Öppna" \
        callback :: mvalve_Open \
        reason :: 1
    inst op_gis_button1 0 0
    . move 78.6959 24.8871
    renamedvars  \
        button_label :: "Stopp" \
        callback :: mvalve_Stop \
        reason :: 1
    inst op_gis_button1 0 0
    . move 78.7047 16.5943
    renamedvars  \
        button_label :: "Stäng" \
        callback :: mvalve_Close \
        reason :: 1
    fcolor 17
    poly \
        42 45 44 45 44 50 \
        45 50 43 52 41 50 \
        42 50 
    . filled 1
    . dynprop   \
          (AutoOpen   \
            (!= 1  \
              (fcolor 0) )   \
            (= 1  \
              (fcolor 17) ))
    . tran 0.870504 0 0 0 0.870504 0
    . move 23.4953 -1.93825
    poly \
        42 45 44 45 44 50 \
        45 50 43 52 41 50 \
        42 50 
    . filled 1
    . dynprop   \
          (AutoClose   \
            (!= 1  \
              (fcolor 0) )   \
            (= 1  \
              (fcolor 17) ))
    . tran 0.870504 0 0 0 -0.870504 0
    . move 17.9821 82.5006
    bcolor 0
    height 1.5
    text "Förregling :" 25.1373 45.5729
    group
    . dynprop   \
          (Alarm  \
            (= *   \
              (batcherase)))
    . batcherase 1
        fcolor 13
        fstyle 1
        finter 2
        ecolor 7
        estyle 0
        frect 2.33476 9.62131 54.4181 0.513138 
        . dynprop   \
              (Alarm  \
                (= *   \
                  (redraw)))
        fcolor 1
        fstyle 0
        finter 1
        estyle 1
        tcolor 0
        height 3
        ftrect 4 8 52 2  "Larmtext"
        . dynprop   \
              (Alarm   \
                (= 1  \
                  (stext "Gångtid Öppna" "%s") )   \
                (= 2  \
                  (stext "Gångtid Stäng" "%s") )   \
                (= 3  \
                  (stext "Tappat öppetläge" "%s") )   \
                (= 4  \
                  (stext "Tappat stängläge" "%s") )   \
                (= 5  \
                  (stext "Kontaktorsvar öppna" "%s") )   \
                (= 6  \
                  (stext "Kontaktorsvar stäng" "%s") )  \
                (= *   \
                  (vis (Alarm > 0))  \
                  (redraw)))
    endg
    group
    . dynprop   \
          (sts  \
            (= *   \
              (batcherase)))
    . batcherase 1
        fcolor 13
        fstyle 1
        finter 2
        estyle 0
        frect 1.3085 74.2765 67.6314 66.9643 
        . dynprop   \
              (sts  \
                (= *   \
                  (redraw)))
        inst op_sub_stsline 0 0
        . dynprop   \
              (sts  \
                (= *   \
                  (redraw)))
        . move 34.1492 71.0694
    endg
endm
