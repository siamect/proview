// 
//  Proview   $Id: op_cont.g,v 1.2 2005-09-06 10:39:34 claes Exp $
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
op_cont: model
    . dynprop  \
          (*  \
            (call pwr_SetSelectedTextObject(__selected_object)))
    detect 0
    fcolor 13
    fstyle 0
    finter 1
    fdir 0
    fpercent 100
    ecolor 13
    estyle 1
    ewidth 1
    bg: frect 1 33.8949 179.547 76.1051 
    . refpoint 0.488098 3.55171
    detect 1
    graphobject: inst op_gis_button1 0 0
    . move 2.67534 62.3172
    . scale 2.67534 62.3172 0.984707 0.873039
    . refpoint 2.70895 66.3271
    renamedvars  \
        button_label :: "Grafik" \
        callback :: cont_Graph_CB \
        reason :: 1
    trendobject: inst op_gis_button1 0 0
    . move 14.2059 62.3172
    . scale 14.2059 62.3172 0.984707 0.873039
    . refpoint 19.6218 64.4998
    renamedvars  \
        button_label :: "Trend" \
        callback :: cont_Trend_CB \
        reason :: 3
    object: inst op_gis_button1 0 0
    . move 38.802 62.3172
    . scale 38.802 62.3172 1.09106 0.873039
    . refpoint 44.8028 64.4998
    renamedvars  \
        button_label :: "Objekt" \
        callback :: cont_Object_CB \
        reason :: 4
    group
    . refpoint 2 75
        ecolor 15
        ewidth 2
        line 2 61.2562 2 74.9998 90.2621 74.9998 
        ecolor 12
        line 2 61.2562 90.2621 61.2562 90.2621 74.9998 
    endg
    group
    . refpoint 2 54.0001
        ecolor 15
        line 2 54.0001 2 60.8705 37.4727 60.8705 
        ecolor 12
        line 2 54.0001 37.4727 54.0001 37.4727 60.8705 
    endg
    group
    . refpoint 104.849 75
        ecolor 15
        line 104.849 53.9998 104.849 74.9997 174.98 74.9997 
        ecolor 12
        line 104.849 53.9998 174.98 53.9998 174.98 74.9997 
    endg
    group
    . refpoint 38.1115 54
        ecolor 15
        line 38.1115 54.0001 38.1115 60.8705 90.2621 60.8705 
        ecolor 12
        line 38.1115 54.0001 90.2621 54.0001 90.2621 60.8705 
    endg
    fastavail1: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt0Vis))
    . move 105.704 69.3447
    . scale 105.704 69.3447 1.16828 0.905879
    . refpoint 105.704 69.3447
    renamedvars  \
        button_label :: ApplLab0 \
        callback :: cont_ApplButt_CB \
        reason :: 0
    fastavail2: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt1Vis))
    . move 119.596 69.3447
    . scale 119.596 69.3447 1.16828 0.905879
    . refpoint 119.596 69.3447
    renamedvars  \
        button_label :: ApplLab1 \
        callback :: cont_ApplButt_CB \
        reason :: 1
    fastavail3: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt2Vis))
    . move 133.489 69.3447
    . scale 133.489 69.3447 1.16828 0.905879
    . refpoint 133.489 69.3447
    renamedvars  \
        button_label :: ApplLab2 \
        callback :: cont_ApplButt_CB \
        reason :: 2
    fastavail4: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt3Vis))
    . move 147.381 69.3447
    . scale 147.381 69.3447 1.16828 0.905879
    . refpoint 147.381 69.3447
    renamedvars  \
        button_label :: ApplLab3 \
        callback :: cont_ApplButt_CB \
        reason :: 3
    fastavail5: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt4Vis))
    . move 161.273 69.3447
    . scale 161.273 69.3447 1.16828 0.905879
    . refpoint 161.273 69.3447
    renamedvars  \
        button_label :: ApplLab4 \
        callback :: cont_ApplButt_CB \
        reason :: 4
    fastavail6: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt5Vis))
    . move 105.704 62.235
    . scale 105.704 62.235 1.16828 0.905879
    . refpoint 105.704 62.235
    renamedvars  \
        button_label :: ApplLab5 \
        callback :: cont_ApplButt_CB \
        reason :: 5
    fastavail7: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt6Vis))
    . move 119.596 62.235
    . scale 119.596 62.235 1.16828 0.905879
    . refpoint 119.596 62.235
    renamedvars  \
        button_label :: ApplLab6 \
        callback :: cont_ApplButt_CB \
        reason :: 6
    fastavail8: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt7Vis))
    . move 133.489 62.235
    . scale 133.489 62.235 1.16828 0.905879
    . refpoint 133.489 62.235
    renamedvars  \
        button_label :: ApplLab7 \
        callback :: cont_ApplButt_CB \
        reason :: 7
    fastavail9: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt8Vis))
    . move 147.381 62.235
    . scale 147.381 62.235 1.16828 0.905879
    . refpoint 147.381 62.235
    renamedvars  \
        button_label :: ApplLab8 \
        callback :: cont_ApplButt_CB \
        reason :: 8
    fastavail10: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt9Vis))
    . move 161.273 62.235
    . scale 161.273 62.235 1.16828 0.905879
    . refpoint 161.273 62.235
    renamedvars  \
        button_label :: ApplLab9 \
        callback :: cont_ApplButt_CB \
        reason :: 9
    fastavail11: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt10Vis))
    . move 105.704 55.2353
    . scale 105.704 55.2353 1.16828 0.905879
    . refpoint 105.704 55.2353
    renamedvars  \
        button_label :: ApplLab10 \
        callback :: cont_ApplButt_CB \
        reason :: 10
    fastavail12: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt11Vis))
    . move 119.596 55.2353
    . scale 119.596 55.2353 1.16828 0.905879
    . refpoint 119.596 55.2353
    renamedvars  \
        button_label :: ApplLab11 \
        callback :: cont_ApplButt_CB \
        reason :: 11
    fastavail13: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt12Vis))
    . move 133.489 55.2353
    . scale 133.489 55.2353 1.16828 0.905879
    . refpoint 133.489 55.2353
    renamedvars  \
        button_label :: ApplLab12 \
        callback :: cont_ApplButt_CB \
        reason :: 12
    fastavail14: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt13Vis))
    . move 147.381 55.2353
    . scale 147.381 55.2353 1.16828 0.905879
    . refpoint 147.381 55.2353
    renamedvars  \
        button_label :: ApplLab13 \
        callback :: cont_ApplButt_CB \
        reason :: 13
    fastavail15: inst op_gis_button1 0 0
    . dynprop  \
          (*  \
            (vis ApplButt14Vis))
    . move 161.273 55.2353
    . scale 161.273 55.2353 1.16828 0.905879
    . refpoint 161.273 55.2353
    renamedvars  \
        button_label :: ApplLab14 \
        callback :: cont_ApplButt_CB \
        reason :: 14
    graphlist: inst op_gis_button1 0 0
    . move 2.69496 55.3174
    . scale 2.69496 55.3174 0.984707 0.873039
    . refpoint 2.69496 55.3174
    renamedvars  \
        button_label :: "Grafik" \
        callback :: cont_GraphList_CB \
        reason :: 1
    trendlist: inst op_gis_button1 0 0
    . move 14.2256 55.3174
    . scale 14.2256 55.3174 0.984707 0.873039
    . refpoint 14.2256 55.3174
    renamedvars  \
        button_label :: "Trend" \
        callback :: cont_TrendList_CB \
        reason :: 3
    fastlist: inst op_gis_button1 0 0
    . move 25.7563 55.3174
    . scale 25.7563 55.3174 0.984707 0.873039
    . refpoint 25.7563 55.3174
    renamedvars  \
        button_label :: "Snabb" \
        callback :: cont_FastList_CB
    histlist: inst op_gis_button1 0 0
    . move 64.5607 55.2527
    . scale 64.5607 55.2527 1.09106 0.873039
    . refpoint 64.5607 55.2527
    renamedvars  \
        button_label :: "Histlista" \
        callback :: cont_HistLis_CB \
        reason :: 5
    blocklist: inst op_gis_button1 0 0
    . move 77.4322 55.2527
    . scale 77.4322 55.2527 1.09106 0.873039
    . refpoint 77.4322 55.2527
    renamedvars  \
        button_label :: "Blocklista" \
        callback :: cont_BlockLis_CB \
        reason :: 5
    group
    . refpoint 90.7854 75
        ecolor 15
        line 90.7853 53.9998 90.7853 74.9997 104.302 74.9997 
        ecolor 12
        line 90.7853 53.9998 104.302 53.9998 104.302 74.9997 
    endg
    navigator: inst op_gis_button1 0 0
    . move 91.5428 55.3174
    . scale 91.5428 55.3174 1.09106 0.873039
    . refpoint 97.5436 57.5
    renamedvars  \
        button_label :: "Navigatör" \
        callback :: cont_Navigator_CB \
        reason :: 4
    htrend: inst op_gis_button1 0 0
    . move 91.5428 62.3171
    . scale 91.5428 62.3171 1.09106 0.873039
    . refpoint 97.5436 64.4997
    renamedvars  \
        button_label :: "HTrend" \
        callback :: cont_HTrend_CB \
        reason :: 4
    alarmlist: inst op_gis_button1 0 0
    . move 38.802 55.2527
    . scale 38.802 55.2527 1.09106 0.873039
    . refpoint 38.802 55.2527
    renamedvars  \
        button_label :: "Larmlista" \
        callback :: cont_AlarmLis_CB \
        reason :: 5
    eventlist: inst op_gis_button1 0 0
    . move 51.6735 55.2527
    . scale 51.6735 55.2527 1.09106 0.873039
    . refpoint 51.6735 55.2527
    renamedvars  \
        button_label :: "Händlista" \
        callback :: cont_EventLis_CB \
        reason :: 5
    fastobject: inst op_gis_button1 0 0
    . move 25.7563 62.3172
    . scale 25.7563 62.3172 0.984707 0.873039
    . refpoint 31.1722 64.4998
    renamedvars  \
        button_label :: "Snabb" \
        callback :: cont_Fast_CB \
        reason :: 3
    alarmlimit: inst op_gis_button1 0 0
    . move 64.5452 62.3172
    . scale 64.5452 62.3172 1.09106 0.873039
    . refpoint 64.5452 62.3172
    renamedvars  \
        button_label :: "Larmgräns" \
        callback :: cont_AlarmLim_CB \
        reason :: 5
    alarmblock: inst op_gis_button1 0 0
    . move 77.3176 62.3172
    . scale 77.3176 62.3172 1.09106 0.873039
    . refpoint 77.3176 62.3172
    renamedvars  \
        button_label :: "Larmblock" \
        callback :: cont_AlarmBlock_CB \
        reason :: 4
endm
