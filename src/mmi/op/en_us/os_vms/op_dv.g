// 
//  Proview   $Id: op_dv.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
//
mtran0
vis 1
detect 1
op_dv: model
    fcolor 13
    fstyle 0
    finter 1
    fdir 0
    fpercent 100
    ecolor 13
    estyle 0
    ewidth 1
    bg: frect -26.2712 101.359 121.775 -9.04373 
    set_dig: inst op_gis_button1 0 0
    . move 63.005 35.48
    renamedvars  \
        button_label :: "1" \
        callback :: me_SetActval \
        reason :: 1
    reset_dig: inst op_gis_button1 0 0
    . move 63.005 25.48
    renamedvars  \
        button_label :: "0" \
        callback :: me_ResetActval \
        reason :: 1
    more: inst op_gis_button1 0 0
    . move 44.5 2.48001
    renamedvars  \
        button_label :: "More..." \
        callback :: me_More \
        reason :: 1
    inst op_sub_stsline 0 0
    . move 36 70.6454
    cancel: inst op_gis_button1 0 0
    . move 81.8395 2.48001
    renamedvars  \
        button_label :: "Cancel" \
        callback :: pwr_Quit \
        reason :: 1
    fcolor 12
    fstyle 1
    ecolor 12
    poly \
        4.5 53.2756 67.5 53.2756 67 53.7756 \
        5 53.7756 
    . filled 1
    . move 0 6.82497
    fcolor 15
    ecolor 15
    poly \
        4.5 59.2756 5 58.7756 67 58.7756 \
        67.5 59.2756 
    . filled 1
    . move 0 6.82497
    fcolor 14
    ecolor 14
    poly \
        4.5 59.2756 5 58.7756 5 53.7756 \
        4.5 53.2756 
    . filled 1
    . move 0 6.82497
    fcolor 13
    ecolor 13
    tcolor 7
    height 3
    path 1
    font 1
    prec 2
    align 2 3
    size 0 0
    ftrect 67 53.7756 5 58.7702  " "
    . dynprop  \
          (*  \
            (stext StsText "%s"))
    . move 0 6.82497
    fcolor 14
    ecolor 14
    poly \
        67 58.7756 67 53.7756 67.5 53.2756 \
        67.5 59.2756 
    . filled 1
    . move 0 6.82497
    group
    . dynprop   \
          (ActualValue  \
            (= *   \
              (batcherase)  \
              (redraw)))
    . batcherase 1
    . move 15.646 24.0505
    . scale 15.646 24.0505 1.14474 1.14474
        fcolor 4
        finter 2
        ecolor 7
        frect 18 51 46.5 25.5 
        . dynprop   \
              (ActualValue   \
                (= 0  \
                  (fcolor 12) ))
        . move -12.5431 -20.356
        . scale -12.5431 -20.356 0.853222 0.853222
        . refpoint -0.901337 -6.53783
        fcolor 15
        finter 1
        ecolor 15
        poly \
            17.5 51.5 18 51 46.5 51 \
            47 51.5 
        . filled 1
        . move -12.5431 -20.356
        . scale -12.5431 -20.356 0.853222 0.853222
        fcolor 14
        ecolor 14
        poly \
            17.5 51.5 17.5 25 18 25.5 \
            18 51 
        . filled 1
        . move -12.5431 -20.356
        . scale -12.5431 -20.356 0.853222 0.853222
        fcolor 12
        ecolor 12
        poly \
            17.5 25 47 25 46.5 25.5 \
            18 25.5 
        . filled 1
        . move -12.5431 -20.356
        . scale -12.5431 -20.356 0.853222 0.853222
        fcolor 14
        ecolor 14
        poly \
            47 51.5 47 25 46.5 25.5 \
            46.5 51 
        . filled 1
        . move -12.5431 -20.356
        . scale -12.5431 -20.356 0.853222 0.853222
    endg
endm
