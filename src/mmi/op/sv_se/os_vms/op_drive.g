// 
//  Proview   $Id: op_drive.g,v 1.2 2005-09-06 10:39:34 claes Exp $
//  Copyright (C) 2005 SSAB Oxelösund AB.
//
//  This program is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU General Public License as 
//  published by the Free Software Foundation, either version 2 of 
//  the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License 
//  along with the program, if not, write to the Free Software 
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

mtran0
vis 1
detect 1
op_drive: model
    fcolor 13
    fstyle 1
    finter 2
    fdir 0
    fpercent 100
    ecolor 7
    estyle 0
    ewidth 1
    frect -22.3484 91.1926 117.757 -15.7027 
    inst op_gis_button1 0 0
    . move 87.5243 2.48309
    renamedvars  \
        button_label :: "Mer..." \
        callback :: drive_More \
        reason :: 1
    inst op_gis_button1 0 0
    . move 76.9799 18.8288
    . scale 76.9799 18.8288 1.28371 1.28371
    . refpoint 76.9799 18.8288
    renamedvars  \
        button_label :: "Stopp" \
        callback :: drive_Stopp \
        reason :: 1
    inst op_gis_button1 0 0
    . move 77.1857 31.7974
    . scale 77.1857 31.7974 1.28371 1.28371
    . refpoint 77.1857 31.7974
    renamedvars  \
        button_label :: "Start" \
        callback :: drive_Start \
        reason :: 1
    inst op_gis_button1 0 0
    . move 77.1857 44.7659
    . scale 77.1857 44.7659 1.28371 1.28371
    . refpoint 77.1857 44.7659
    renamedvars  \
        button_label :: "Man" \
        callback :: drive_Man \
        reason :: 1
    inst op_gis_button1 0 0
    . tran 1.28371 2.168404e-19 0 -2.168404e-19 1.28371 0
    . move 51.6603 44.5601
    renamedvars  \
        button_label :: "Auto" \
        callback :: drive_Auto \
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
    . move -28.8599 -14.9748
    . scale -28.8599 -14.9748 1.28371 1.28371
    . refpoint -28.8599 -14.9748
    fcir2 88.1568 53.9114 89.7604 54.0717 
    . dynprop   \
          (ManMod   \
            (= 1  \
              (fcolor 0) )   \
            (= 0  \
              (fcolor 2) )   \
            (= 2  \
              (fcolor 7) ))
    . move -54.5167 -15.1351
    . scale -54.5167 -15.1351 1.28371 1.28371
    . refpoint -54.5167 -15.1351
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
          (Stat   \
            (!= 2  \
              (fcolor 0) )   \
            (!= 3  \
              (fcolor 0) )   \
            (= 2  \
              (fcolor 17) )   \
            (= 3  \
              (fcolor 17) ))
    . tran 1.44319 0 0 0 1.44319 0
    . move -20.0572 -21.9948
    poly \
        42 45 44 45 44 50 \
        45 50 43 52 41 50 \
        42 50 
    . filled 1
    . dynprop   \
          (Stat   \
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
    ftrect 16.1266 43.284 36.3316 34  "LOCAL"
    . dynprop   \
          (NoStart   \
            (= 0  \
              (stext " " "%s") ))  \
          (NoStart   \
            (= 1  \
              (stext "EJ START" "%s") ))  \
          (ProdStop   \
            (= 1  \
              (stext "DRIFT" "%s") ))  \
          (Local   \
            (= 1  \
              (stext "LOKAL" "%s") ))  \
          (SafeStop   \
            (= 1  \
              (stext "SÄK" "%s") ))
    . move -5.98853 -8.98277
    . scale -5.98853 -8.98277 1.23033 1.23033
    . refpoint -5.98853 -8.98277
    ftrect 14 64 38 56  "DRIFT"
    . dynprop   \
          (Stat   \
            (!= 4  \
              (stext " " "%s") ))  \
          (Stat   \
            (= 4  \
              (stext "DRIFT" "%s") ))
    ftrect 14 16 38 24  "STOPP"
    . dynprop   \
          (Stat   \
            (!= 0  \
              (stext " " "%s") ))  \
          (Stat   \
            (= 0  \
              (stext "STOPP" "%s") ))
    inst op_gis_button1 0 0
    . tran 1.28371 2.168404e-19 0 -2.168404e-19 1.28371 0
    . move 51.5946 31.4338
    renamedvars  \
        button_label :: "Auto" \
        callback :: drive_Auto \
        reason :: 1
    inst op_gis_button1 0 0
    . tran 1.28371 2.168404e-19 0 -2.168404e-19 1.28371 0
    . move 51.7549 31.4338
    renamedvars  \
        button_label :: "Auto" \
        callback :: drive_Auto \
        reason :: 1
    inst op_gis_button1 0 0
    . tran 1.28371 2.168404e-19 0 -2.168404e-19 1.28371 0
    . move 51.9153 31.5942
    renamedvars  \
        button_label :: "Auto" \
        callback :: drive_Auto \
        reason :: 1
    inst op_gis_button1 0 0
    . tran 1.28371 2.168404e-19 0 -2.168404e-19 1.28371 0
    . move 51.4343 31.2735
    renamedvars  \
        button_label :: "Auto" \
        callback :: drive_Auto \
        reason :: 1
    inst op_gis_button1 0 0
    . tran 1.28371 2.168404e-19 0 -2.168404e-19 1.28371 0
    . move 51.5946 31.2735
    renamedvars  \
        button_label :: "Restart" \
        callback :: drive_Restart \
        reason :: 1
    fcolor 6
    fstyle 1
    fcir2 88.1568 53.9114 89.7604 54.0717 
    . dynprop   \
          (AutoStart   \
            (= 0  \
              (fcolor 0) )   \
            (= 1  \
              (fcolor 2) ))
    . move -54.6771 -28.4446
    . scale -54.6771 -28.4446 1.28371 1.28371
    . refpoint -54.6771 -28.4446
    bcolor 0
    height 1.5
    text "Förregling :" 25.458 47.1765
    group
    . dynprop   \
          (Alarm  \
            (= *   \
              (batcherase)))
    . batcherase 1
        fcolor 13
        finter 2
        ecolor 7
        estyle 0
        frect 1.05194 11.0324 54.0332 0.513138 
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
                  (stext "Tid kontaktorsvar" "%s") )   \
                (= 2  \
                  (stext "Tappat kontaktorsvar" "%s") )   \
                (= 3  \
                  (stext "Hastighetsvakt" "%s") )  \
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
        frect 1.43678 74.7896 66.2202 66.5794 
        . dynprop   \
              (sts  \
                (= *   \
                  (redraw)))
        inst op_sub_stsline 0 0
        . dynprop   \
              (sts  \
                (= *   \
                  (redraw)))
        . move 32.9947 71.0694
    endg
endm
