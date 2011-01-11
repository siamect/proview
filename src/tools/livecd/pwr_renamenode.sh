broot=/usr/pwrp/pwrdemo48/bld
sed -i 's/red-ant/ubuntu/g' $broot/common/load/ld_boot_red-ant_0999.dat
sed -i 's/red-ant/ubuntu/g' $broot/common/load/ld_node_red-ant_0999.dat
mv $broot/common/load/ld_boot_red-ant_0999.dat $broot/common/load/ld_boot_ubuntu_0999.dat
mv $broot/common/load/ld_node_red-ant_0999.dat $broot/common/load/ld_node_ubuntu_0999.dat
mv $broot/common/load/ld_appl_red-ant_999.txt $broot/common/load/ld_appl_ubuntu_999.txt
mv $broot/x86_linux/exe/plc_red-ant_0999_00005 $broot/x86_linux/exe/plc_ubuntu_0999_00005

