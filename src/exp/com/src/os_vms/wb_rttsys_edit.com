$!
$! Commandfile to start the rtt editor for system picture library
$!
$ define/job pwrp_rtt pwr_root:[src.lib.dtt.src]
$ define/job pwrp_rttbld pwrb_root:[os_'pwre_os'.hw_'pwre_hw'.bld.lib.dtt]
$ mc pwr_exe:wb_rtt rttsys
$ deassign/job pwrp_rtt
$ deassign/job pwrp_rttbld
