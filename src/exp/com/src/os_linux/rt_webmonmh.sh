#/bin/bash
#
export LD_LIBRARY_PATH=$pwr_exe
export CLASSPATH=$pwr_lib/pwr_rt.jar:$pwr_lib/pwr_jop.jar
$jdk_home/java jpwr.rt.MhServer
