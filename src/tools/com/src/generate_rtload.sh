#!/bin/bash
source $pwre_bin/pwre_function

targetdir=$pwre_broot/$pwre_os/$pwre_hw/exp/rtload
if [ ! -e $targetdir ]; then
  mkdir $targetdir
fi

pwre module
target="$targetdir/pwrs.dbs"
wb_cmd -q -i create snapshot /rtonly /file=\"$pwre_sroot/wbl/pwrs/src/\"/out=\"$target\"

target="$targetdir/pwrb.dbs"
wb_cmd -q -i create snapshot /rtonly /file=\"$pwre_sroot/wbl/pwrb/src/\"/out=\"$target\"

target="$targetdir/rt.dbs"
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/rt/src/rt.wb_load\"/out=\"$target\"
pwre merge

pwre module nmps
target="$targetdir/nmps.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/nmps/src/\"/out=\"$target\"
pwre merge

pwre module remote
target="$targetdir/remote.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/remote/src/\"/out=\"$target\"
pwre merge

pwre module tlog
target="$targetdir/tlog.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/tlog/src/\"/out=\"$target\"
pwre merge

pwre module profibus
target="$targetdir/profibus.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/profibus.wb_load\"/out=\"$target\"
pwre merge

pwre module bcomp
target="$targetdir/basecomponent.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/bcomp/src/basecomponent.wb_load\"/out=\"$target\"
pwre merge

pwre module opc
target="$targetdir/opc.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/opc.wb_load\"/out=\"$target\"
pwre merge

pwre module ssabox
target="$targetdir/ssabox.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/ssabox.wb_load\"/out=\"$target\"
pwre merge

pwre module misc
target="$targetdir/miscellaneous.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/misc/src/miscellaneous.wb_load\"/out=\"$target\"
pwre merge

pwre module siemens
target="$targetdir/siemens.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/siemens.wb_load\"/out=\"$target\"
pwre merge

pwre module otherio
target="$targetdir/otherio.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/otherio.wb_load\"/out=\"$target\"
pwre merge

pwre module othermanu
target="$targetdir/othermanufacturer.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/othermanufacturer.wb_load\"/out=\"$target\"
pwre merge

pwre module abb
target="$targetdir/abb.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/abb.wb_load\"/out=\"$target\"
pwre merge

pwre module inor
target="$targetdir/inor.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/inor.wb_load\"/out=\"$target\"
pwre merge

pwre module klocknermoeller
target="$targetdir/klocknermoeller.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/klocknermoeller.wb_load\"/out=\"$target\"
pwre merge

pwre module telemecanique
target="$targetdir/opc.dbs"
export pwr_load=$pwr_eload
wb_cmd -i -q create snapshot /rtonly /file=\"$pwre_sroot/wbl/mcomp/src/telemecanique.wb_load\"/out=\"$target\"
pwre merge


