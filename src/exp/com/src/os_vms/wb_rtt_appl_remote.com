$!
$! Remote link or compile
$!
$
$ pgmname = p1
$ action = f$integer( p2)
$ opsys = f$integer( p3)
$ debug = f$integer( p4)
$ p_rttbld = p5
$ p_root = p6
$
$ platform = "VAX_VMS"
$ hardware = f$extract(0,3,f$getsyi("HW_NAME"))
$ if hardware .nes. "VAX" then platform = "AXP_VMS"
$
$ set ver
$ define /job pwrp_rttbld 'p_rttbld'
$ define /job/trans=conc pwrp_root 'p_root'
$ define /job pwrp_obj pwrp_root:['platform'.obj]
$ define /job pwrp_lib pwrp_root:['platform'.lib]
$ define /job pwrp_exe pwrp_root:['platform'.exe]
$
$ @pwr_exe:wb_rtt_appl 'p1' 'p2' 'p3' 'p4'
