#!/bin/bash

set -o xtrace

aroot="/usr/pwrp/adm"

pkgsrc=$pwre_sroot/tools/pkg/openbsd/pwr


# Get version
if [ -e $pwr_inc/pwr_version.h ]; then
  ver=`eval cat $pwr_inc/pwr_version.h | grep "[[:<:]]pwrv_cWbdbVersionShortStr[[:>:]]" | awk '{print $3}'`

  if [ -z $ver ]; then
    echo "Unable to get pwr version"
    ver="V00"
  fi
  ver=${ver:2:2}
fi

# Generate version help file
{
  if [ ! -e $pkgsrc/control ]; then
    echo "Controlfile not found"
    exit 1
  fi
  datfile=$pkgsrc/control

  echo "<topic> version"
  d=`eval date +\"%F %X\"`

  {
    let printout=0
    while read line; do
      if [ "${line:0:9}" = "Package: " ]; then 
        package=${line#Package: }
      fi
      if [ "${line:0:9}" = "Version: " ]; then
        version=${line#Version: }
      fi
      if [ "${line:0:14}" = "Architecture: " ]; then
        arch=${line#Architecture: }
      fi

      if [ "${line:0:12}" = "Description:" ]; then
        echo ""
        echo "<image> pwr_logga.gif"
	echo ""
	echo ""
	echo ""
        echo "<b>Proview V${version:0:3}"
	echo "Version V$version"
        echo ""
        echo "Copyright © 2004-${d:0:4} SSAB Oxelösund AB"
        echo ""
        echo "This program is free software; you can redistribute it and/or"
        echo "modify it under the terms of the GNU General Public License as" 
        echo "published by the Free Software Foundation, either version 2 of"
        echo "the License, or (at your option) any later version."
        echo ""
        echo "This program is distributed in the hope that it will be useful" 
        echo "but WITHOUT ANY WARRANTY; without even the implied warranty of" 
        echo "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
        echo "For more details, see the"
        echo "GNU General Public License. <weblink> http://www.proview.se/gpllicense.html"
        echo ""
        echo "E-mail postmaster@proview.se"
        echo "Internet www.proview.se <link> http://www.proview.se/index.html"
	echo ""
	echo ""
	echo "<b>Package"
	echo "Package $package""_""$version""_""$arch"
	echo "Build date $d"
	echo "Package description:"
	echo ""

        printout=1
      else
        if [ $printout -eq 1 ]; then
          echo $line
        fi
      fi
    done
  } < $datfile
  echo "</topic>"
} > $pwr_eexe/wtt_version_help.dat

# Convert to html
co_convert -t -d $pwr_doc $pwr_eexe/wtt_version_help.dat

{
  echo "<html><head>"
  echo "<meta http-equiv=\"Refresh\" content=\"5;../wtt_version_help_version.html\">"
  echo "</head></html>"
} > $pwr_doc/en_us/package_version.html

if [ "$1" == "-v" ]; then
  exit
fi

echo "-- Building pwr$ver"
packagename=pwr$ver-$version
pkgroot=$pwre_broot/$pwre_target/bld/pkg/$packagename
pkg_proot=$pwre_broot/$pwre_target/bld/pkg/$packagename/share/pwr$ver
# Create directories
echo "echo pwr$ver dummy" > $pkgroot/bin/pwr$ver
chmod a+x $pkgroot/bin/pwr$ver
mkdir -p $pkgroot/share/pwr$ver

mkdir -p $pkg_proot/usr/pwrp
mkdir -p $pkg_proot/etc
mkdir -p $pkg_proot/bin
echo "Start pwr$ver with command pwra" > $pkg_proot/share/pwr$ver/README
cp $pkgsrc/postinst $pkg_proot/install.site

find $pkg_proot -type d | xargs chmod 755

# copyright
cp $pkgsrc/copyright $pkg_proot/usr/share/doc/pwr$ver

# Man pages
mkdir -p $pkg_proot/usr/share/man/man1
cp $pkgsrc/pwr.1 $pkg_proot/usr/share/man/man1/pwr.1
gzip -fq --best $pkg_proot/usr/share/man/man1/pwr.1

# Copy proview
mkdir $pkg_proot/usr/pwr$ver
currentdir="`eval pwd`"
tarfile=$pwre_broot/$pwre_target/bld/pkg/pwrtmp.tar
cd $pwre_broot
echo "-- copy release to package tree"
tar -cf $tarfile $pwre_target/exp
cd $pkg_proot/usr/pwr$ver
tar -xf $tarfile
rm $tarfile
cd $currentdir

# Remove not needed libraries
rm $pkg_proot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_flow.a
rm $pkg_proot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_ge.a
rm $pkg_proot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_glow.a
rm $pkg_proot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_tlog.a
rm $pkg_proot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_wb.a
rm $pkg_proot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_xtt.a

# Copy configuration files to cnf
cp $pkgsrc/proview.cnf $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf
cp $pkgsrc/pwrp_profile $pkg_proot/etc
chmod a+x $pkg_proot/etc/pwrp_profile

# Copy adm files to cnf
cp $pwre_sroot/tools/pkg/deb/adm/pwr_setup.sh $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf
echo "pwrp set base V${ver:0:1}.${ver:1:1}" >> $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/pwr_setup.sh
chmod a+x $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/pwr_setup.sh
cp $pwre_sroot/tools/pkg/deb/adm/pwra_env.sh $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf
chmod a+x $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/pwra_env.sh
cp $pwre_sroot/tools/pkg/deb/adm/pwr_volumelist.dat $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf
cp $pwre_sroot/tools/pkg/deb/adm/pwr_user2.dat $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf
cp $pwre_sroot/tools/pkg/deb/adm/proview_icon.png $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf

# Copy user to cnf
mkdir $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bashrc $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
echo "source $aroot/db/pwr_setup.sh" >> $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user/.bashrc
cp $pwre_sroot/tools/pkg/deb/user/.bash_profile $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
#cp $pwre_sroot/tools/pkg/deb/user/.mwmrc $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.rtt_start $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xtt_start $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
#cp $pwre_sroot/tools/pkg/deb/user/.xsession $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/wtt_init.pwr_com $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/wtt_init1.pwr_com $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user

# Generate desktop file
{
  echo "[Desktop Entry]"
  echo "Version=1.0"
  echo "Encoding=UTF-8"
  echo "Name=Proview V${version:0:3}"
  echo "Type=Application"
  echo "Comment=Start Proview"
  echo "Exec=xterm -title \"Proview Development Console\" -e 'export pwra_db=$aroot/db;source \$pwra_db/pwra_env.sh set base V""${version:0:3}"";source \$pwra_db/pwra_env.sh set bus;wb -p pwrp pwrp'"
  echo "Icon=$aroot/db/proview_icon.png"
  echo "Categories=GNOME;GTK;Application;"
} > $pkg_proot/usr/pwr$ver/$pwre_target/exp/cnf/user/proview$ver.desktop

# Create package
echo "-- Building package"
cd $pwre_broot/$pwre_target/bld/pkg
tar -czf /usr/ports/distfiles/$packagename.tar.gz $packagename
mkdir -p /usr/ports/mystuff/misc/pwr$ver
cd /usr/ports/mystuff/misc/pwr$ver
cp $pkgsrc/Makefile .
make makesum
mkdir pkg
echo "Proview development and runtime package" > pkg/DESCR
if [ -e /usr/obj/ports/$pkgname ]; then
  rm -r /usr/obj/ports/$pkgname
fi
make fake
make plist
make package
#rm -r $pgkroot












