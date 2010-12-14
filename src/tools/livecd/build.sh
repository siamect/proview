#!/bin/bash

#pwrpkg_dir=$pwre_broot/os_linux/hw_x86/bld/pkg
pwrpkg_dir=~
customize_dir=~/tmp/remaster-root/tmp
apt_cache=/var/cache/apt/archives

libn1="libmysql++"
lib1="libmysql++3_3.0.9-1ubuntu2_i386.deb"
libn2="libjs-mootools"
lib2="libjs-mootools_1.2.4.0~debian1-1_all.deb"
libn3="midori"
lib3="midori_0.2.4-3_i386.deb"
libn4="thttpd"
lib4="thttpd_2.25b-11_i386.deb"
libn5="pv"
lib5="pv_1.1.4-1_i386.deb"

if [ "$1" == "" ]; then
  # Check packages
  let notfound=0
  if [ ! -e $apt_cache/$lib1 ]; then
    echo "-- Downloading $apt_cache/$lib1"    
    sudo apt-get install  --reinstall -d $libn1
    notfound=1
  fi
  if [ ! -e $apt_cache/$lib2 ]; then
    echo "-- Downloading $apt_cache/$lib2"    
    sudo apt-get install --reinstall -d $libn2
    notfound=1
  fi
  if [ ! -e $apt_cache/$lib3 ]; then
    echo "-- Downloading $apt_cache/$lib3"    
    sudo apt-get install  --reinstall -d $libn3
    notfound=1
  fi
  if [ ! -e $apt_cache/$lib4 ]; then
    echo "-- Downloading $apt_cache/$lib4"    
    sudo apt-get install  --reinstall -d $libn4
    notfound=1
  fi
  if [ ! -e $apt_cache/$lib5 ]; then
    echo "-- Downloading $apt_cache/$lib5"    
    sudo apt-get install  --reinstall -d $libn5
    notfound=1
  fi

  echo ""
  echo "   Start UCK with"
  echo ""
  echo "   > uck-gui"
  echo ""
  echo "   Questions: - Language: en"
  echo "              - Customize the CD: yes"
  echo "              - Name: Proview LiveCD"
  echo "              - Delete files: no"
  echo "              - Choose actions: Run console application"
  echo ""
  echo "   Then start build.sh with argument \"copy\" when the" 
  echo "   UCK customization console is opened"
  echo ""

elif [ "$1" == "copy" ]; then
#
  # Copy packages to customize dir
  cp $pwrpkg_dir/pwr48_4.8.0-1_i386.deb $customize_dir/
  cp $pwrpkg_dir/pwrdemo48_4.8.0-1_i386.deb $customize_dir/
  #
  # Copy installation-scripts and setup
  cp ./pwr_customize.sh $customize_dir/
  cp ./pwr_install.sh $customize_dir/
  cp ./pwr_renamenode.sh $customize_dir/
  cp ./pwr_remove.sh $customize_dir/
  cp ./casper.conf $customize_dir/
  cp ./custom.conf $customize_dir/
  #
  # Scripts, icons and settings for user pwrp
  cp ./backup-script.sh $customize_dir/
  cp ./restore-script.sh $customize_dir/
  cp ./demoweb-script.sh $customize_dir/
  cp ./backup-script.desktop $customize_dir/
  cp ./restore-script.desktop $customize_dir/
  cp ./pwrdemoweb.desktop $customize_dir/
  cp ./welcome.html $customize_dir/
  cp ./midori.desktop $customize_dir/
  cp ./pwr_gconf_config_local.tar.gz $customize_dir/
  #
  # Debian packages
  cp $apt_cache/$lib1 $customize_dir/
  cp $apt_cache/$lib2 $customize_dir/
  cp $apt_cache/$lib3 $customize_dir/
  cp $apt_cache/$lib4 $customize_dir/
  cp $apt_cache/$lib5 $customize_dir/

  echo ""
  echo "   Execute "
  echo ""
  echo "   > cd /tmp"
  echo "   > ./pwr_customize.sh"
  echo ""
  echo "   in the UCK customization console"
  echo ""
  
fi
