$!
$!	Logical names for trend and data storage
$!
$ defg := define/group/nolog
$!
$ defg	op_htre_hd_db			"pwrp_db:rt_hd"
$!
$!
$ defg 	op_htre_realtime_count		30
$ defg	op_htre_trend_timeout		60
$ defg	op_htre_color_print		"no"
$ defg	op_htre_default_directory  	"pwrp_lis:"
$ defg	op_htre_default_extension	"ps"
$ defg	op_htre_default_destination 	"file"
$ defg	op_htre_default_printer		"lps17_ps"
$ defg  op_htre_groups_db		"pwrp_db:op_htre_groups"
$!
$!
$!
$ exit
