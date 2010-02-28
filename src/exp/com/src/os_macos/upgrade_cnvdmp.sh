#!/bin/bash
#
# Proview   $Id: upgrade_cnvdmp.sh,v 1.4 2006-01-30 09:02:51 claes Exp $
# Copyright (C) 2005 SSAB Oxelösund AB.
#
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License as 
# published by the Free Software Foundation, either version 2 of 
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with the program, if not, write to the Free Software 
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#


sed '
/\"$/{
N
h
s/\"\n.*Attr Parameter = \"/\./
}
s/Attr object_did = /Attr oid = /
s/Attr window_did = /Attr woid = /
s/Attr subwind_objdid/Attr subwind_oid/
s/Attr parent_node_did = /Attr poid = /
s/Attr dest_node_did = /Attr dest_oid = /
s/Attr source_node_did = /Attr source_oid = /
s/Attr reset_objdid = /Attr reset_oid = /
s/pwrb:Class-Ai_AI32uP/SsabOx:Class-Ai_AI32uP/
s/pwrb:Class-Ai_HVAI32/SsabOx:Class-Ai_HVAI32/
s/pwrb:Class-Ao_AO8uP/SsabOx:Class-Ao_AO8uP/
s/pwrb:Class-Ao_HVAO4/SsabOx:Class-Ao_HVAO4/
s/pwrb:Class-Co_CO4uP/SsabOx:Class-Co_CO4uP/
s/pwrb:Class-Co_PI24BO/SsabOx:Class-Co_PI24BO/
s/pwrb:Class-Di_DIX2/SsabOx:Class-Di_DIX2/
s/pwrb:Class-Do_HVDO32/SsabOx:Class-Do_HVDO32/
s/NMps:Class-RemTransSend/Remote:Class-RemTransSend/
s/NMps:Class-RemTransRcv/Remote:Class-RemTransRcv/
/PlcProgram$/{
N
N
h
s/Attr objdid = /Attr oid =/
s/Attr classid = /Attr cid =/
}
/PlcWindow$/{
N
N
h
s/Attr objdid = /Attr oid =/
s/Attr classid = /Attr cid =/
}
/PlcNode$/{
N
N
h
s/Attr classid = /Attr cid =/
}
/PlcConnection$/{
N
N
N
h
s/Attr objdid = /Attr oid =/
s/Attr classid = /Attr cid =/
}
' $1 > $2


