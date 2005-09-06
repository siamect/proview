// 
//  Proview   $Id: op_sub_stsline.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_sub_stsline: model
    group
    . dynprop   \
          (sts  \
            (= *   \
              (batcherase)  \
              (redraw)))
    . batcherase 1
        fcolor 13
        fstyle 1
        finter 1
        fdir 0
        fpercent 100
        ecolor 13
        estyle 0
        ewidth 1
        frect 31.5 -3 -31.5 3 
        group
        . dynprop  \
              (*  \
                (vis (sts > 0)))
            fcolor 6
            ecolor 7
            tcolor 7
            height 3
            path 1
            font 7
            prec 2
            align 2 3
            size 0 0
            ftrect -31 2.5 31 -2.5  "Meddelande"
            . dynprop   \
                  (sts   \
                    (= 0  \
                      (fcolor 13)  \
                      (stext "" "%s") )   \
                    (= 1  \
                      (fcolor 1)  \
                      (tcolor 0)  \
                      (stext "A-larm" "%s") )   \
                    (= 2  \
                      (fcolor 3)  \
                      (tcolor 7)  \
                      (stext "B-larm" "%s") )   \
                    (= 3  \
                      (fcolor 6)  \
                      (tcolor 7)  \
                      (stext "C-larm" "%s") )   \
                    (= 4  \
                      (fcolor 5)  \
                      (fstyle 1)  \
                      (finter 2)  \
                      (tcolor 7)  \
                      (stext "D-larm" "%s") )   \
                    (= 5  \
                      (fcolor 6)  \
                      (tcolor 7)  \
                      (stext "Ingen kontakt f.n." "%s") ))
            fcolor 14
            ecolor 14
            poly \
                -31.5 3 -31 2.5 -31 -2.5 \
                -31.5 -3 
            . filled 1
            fcolor 12
            ecolor 12
            poly \
                -31.5 -3 31.5 -3 31 -2.5 \
                -31 -2.5 
            . filled 1
            fcolor 15
            ecolor 15
            poly \
                -31.5 3 -31 2.5 31 2.5 \
                31.5 3 
            . filled 1
            fcolor 12
            ecolor 12
            poly \
                31 2.5 31 -2.5 31.5 -3 \
                31.5 3 
            . filled 1
        endg
    endg
endm
