/* 
 * Proview   $Id: xtt_methodtoolbar.cpp,v 1.1 2007-01-17 06:19:57 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* xtt_methodtoolbar.cpp -- Display method toolbar */

#include <stdio.h>
#include <stdlib.h>
#include "xtt_methodtoolbar.h"

xtt_sMethodButton XttMethodToolbar::m_data[XttMethodToolbar::m_size] =
  {{"Help", 		"$Object-Help",		"$Object-HelpFilter", 		"$pwr_exe/xtt_meth_help.png", 		"Help"},
   {"Note",		"$Object-Note",		"$Object-NoteFilter", 		"$pwr_exe/xtt_meth_note.png", 		"Note"},
   {"Trend",		"$Object-OpenTrend",	"$Object-OpenTrendFilter", 	"$pwr_exe/xtt_meth_trend.png", 		"Trend"},
   {"Fast",		"$Object-OpenFast",	"$Object-OpenFastFilter", 	"$pwr_exe/xtt_meth_fast.png", 		"Fast"},
   {"Photo",		"$Object-Photo",	"$Object-PhotoFilter", 		"$pwr_exe/xtt_meth_photo.png", "Photo"},
   {"Graph",		"$Object-OpenGraph",	"$Object-OpenGraphFilter", 	"$pwr_exe/xtt_meth_graph.png", 		"Graph"},
   {"Object Graph", 	"$Object-OpenObjectGraph","$Object-OpenObjectGraphFilter", "$pwr_exe/xtt_meth_classgraph.png", 	"Object Graph"},
   {"Open Plc",		"$Object-OpenTrace",	"$Object-OpenTraceFilter", 	"$pwr_exe/xtt_meth_trace.png", 	        "Open Plc"},
   {"DataSheet",	"$Object-DataSheet",	"$Object-DataSheetFilter", 	"$pwr_exe/xtt_meth_datasheet.png", 	"DataSheet"},
   {"Hist Event...",	"$Object-HistEvent",	"$Object-HistEventFilter", 	"$pwr_exe/xtt_meth_histevent.png", 	"Hist Event..."},
   {"Block Events...",	"$Object-BlockEvents",	"$Object-BlockEventsFilter", 	"$pwr_exe/xtt_meth_block.png", 		"Block Events..."},
   {"Open Object", 	"$Object-OpenObject",	"$Object-OpenObjectFilter", 	"$pwr_exe/xtt_meth_openobject.png", 	"Open Object"},
   {"Crossreferences", 	"$Object-OpenCrossref",	"$Object-OpenCrossrefFilter", 	"$pwr_exe/xtt_meth_crossref.png", 	"Crossreferences"},
   {"CircuitDiagram",	"$Object-CircuitDiagram",	"$Object-CircuitDiagramFilter", "$pwr_exe/xtt_meth_circuitdiagram.png", "CircuitDiagram"},
   {"Help Class", 	"$Object-HelpClass",	"$Object-HelpClassFilter", 	"$pwr_exe/xtt_meth_classhelp.png", 	"Help Class"}};


