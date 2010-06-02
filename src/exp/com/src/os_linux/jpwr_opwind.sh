export CLASSPATH=$pwr_lib/pwr_rt.jar:$pwr_lib/pwr_jop.jar:$pwr_lib/pwr_jopc.jar:$pwr_lib/pwr_bcomp.jar:$pwr_lib/pwr_bcompfc.jar:$pwr_lib/pwr_abb.jar:$pwrp_lib/pwrp_$pwrp_projectname.jar
export LD_LIBRARY_PATH=$pwr_exe
$jdk/bin/java jpwr.jop.JopOp
