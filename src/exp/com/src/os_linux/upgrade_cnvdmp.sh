#! /bin/bash


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

