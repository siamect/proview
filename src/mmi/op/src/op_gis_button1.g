// 
//  Proview   $Id: op_gis_button1.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_gis_button1: model
    . dynprop  \
          (#  \
            (call gms_flash())  \
            (call gms_call1(callback, reason)))
    group
    . dynprop   \
          ((__button_hilite || button_state)  \
            (= *   \
              (batcherase)))
    . scale 0 0 1.1 1.25
    . refpoint 0 0
        state_1: group
        . dynprop   \
              ((__button_hilite || button_state)   \
                (= 0  \
                  (vis 0) )   \
                (= 1  \
                  (vis 1)  \
                  (redraw) ))
            fcolor 14
            fstyle 1
            finter 1
            fdir 0
            fpercent 100
            ecolor 7
            estyle 0
            ewidth 3
            tcolor 7
            height 1.5
            path 1
            font 1
            prec 0
            align 2 3
            size 0 0
            plate_1: ftrect 0 0 10 4  "m_call1"
            . dynprop   \
                  (button_label  \
                    (= *   \
                      (stext button_label "%s")))
            fcolor 7
            ecolor 15
            estyle 1
            upper_1: line 0 0 0 4 10 4 
            ecolor 12
            lower_1: line 0 0 10 0 10 4 
        endg
        state_0: group
        . dynprop   \
              ((__button_hilite || button_state)   \
                (= 0  \
                  (vis 1)  \
                  (redraw) )   \
                (= 1  \
                  (vis 0) ))
            fcolor 13
            ecolor 7
            estyle 0
            font 7
            plate_0: ftrect 0 0 10 4  "Button1"
            . dynprop   \
                  (button_label  \
                    (= *   \
                      (stext button_label "%s")))
            fcolor 7
            ecolor 15
            estyle 1
            lower_0: line 0 0 10 0 10 4 
            ecolor 12
            upper_0: line 0 0 0 4 10 4 
        endg
    endg
endm
