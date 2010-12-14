
#
# Install 
apt-get -y install libdb4.8
apt-get -y install libdb4.8++
apt-get -y install g++
apt-get -y install mysql-common
apt-get -y install libmysqlclient16
dpkg -i libmysql++3_3.0.9-1ubuntu2_i386.deb
dpkg -i pwr48_4.8.0-1_i386.deb
dpkg -i pwrdemo48_4.8.0-1_i386.deb
apt-get -y install midori
apt-get -y install default-jre-headless
ln -s /usr/lib/jvm/default-java/jre /usr/local/jre
apt-get -y install firefox
apt-get -y install icedtea6-plugin
dpkg -i thttpd_2.25b-11_i386.deb
dpkg -i pv_1.1.4-1_i386.deb

#
# Configure
chmod a+w /etc/casper.conf
cp ./casper.conf /etc/
cp ./custom.conf /etc/gdm/
sed -i 's/no/yes/g' /etc/default/thttpd
sed -i 's/sudo:x:27:/sudo:x:27:pwrp/g' /etc/group
#
# Add pwrp to sudoers
chmod u+s /etc/sudoers
echo "pwrp ALL= NOPASSWD: ALL" >> /etc/sudoers
chmod u-s /etc/sudoers


#
# Rename host nodename to livecd nodename in proview files
/tmp/pwr_renamenode.sh

#
# User pwrp
cp ./backup-script.sh /home/pwrp/
cp ./restore-script.sh /home/pwrp/
cp ./demoweb-script.sh /home/pwrp/
chmod a+x /home/pwrp/*.sh 
cp ./backup-script.desktop /home/pwrp/Desktop/
cp ./restore-script.desktop /home/pwrp/Desktop/
cp  ./pwrdemoweb.desktop /home/pwrp/Desktop
chmod a+x /home/pwrp/Desktop/*.desktop
cp ./welcome.html /home/pwrp/
mkdir -p /home/pwrp/.config/autostart
cp ./midori.desktop /home/pwrp/.config/autostart
chmod a+x /home/pwrp/.config/autostart/midori.desktop
tar --directory /home/pwrp -xzf ./pwr_gconf_config_local.tar.gz
chown -R pwrp:pwrp /home/pwrp/*
chown -R pwrp:pwrp /home/pwrp/.*

#
# Web
cp /usr/pwrp/pwrdemo48/bld/common/web/* /var/www/
cp /usr/pwrp/pwrdemo48/bld/common/load/*.flw /var/www/
cp /usr/pwrp/pwrdemo48/bld/common/load/*rtt*.dat /var/www/
cp /usr/pwr48/os_linux/hw_x86/exp/lib/pwr_jopc.jar /var/www/
cp /usr/pwr48/os_linux/hw_x86/exp/lib/pwr_jop.jar /var/www/
cp /usr/pwr48/os_linux/hw_x86/exp/lib/pwr_rt_client.jar /var/www/
cp /usr/pwr48/os_linux/hw_x86/exp/exe/pwr_logga.gif /var/www/

#
# Midori
dpkg -i libjs-mootools_1.2.4.0~debian1-1_all.deb
dpkg -i midori_0.2.4-3_i386.deb
#
# Clean
#rm /tmp/*.deb
rm -r /usr/pwr48/os_linux/hw_x86/exp/doc/*
rm /usr/pwr48/os_linux/hw_x86/exp/exe/co_help_gtk
rm /usr/pwr48/os_linux/hw_x86/exp/exe/sev_xtt_gtk
rm /usr/pwr48/os_linux/hw_x86/exp/exe/wb_cmd_gtk
rm /usr/pwr48/os_linux/hw_x86/exp/exe/wb_ge_gtk
rm /usr/pwr48/os_linux/hw_x86/exp/exe/xtt_ge_jprint
rm /usr/pwr48/os_linux/hw_x86/exp/exe/wb_upgrade
rm /usr/pwr48/os_linux/hw_x86/exp/exe/wb_rtt
rm /usr/pwr48/os_linux/hw_x86/exp/exe/wb_ldlist

chown -R pwrp:pwrp /usr/pwrp
chown -R pwrp:pwrp /usr/local/pwrp

