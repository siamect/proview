#!/bin/bash

aroot="/usr/pwrp/adm"

# Get version
if [ -e $pwr_inc/pwr_version.h ]; then
  ver=`eval cat $pwr_inc/pwr_version.h | grep "\bpwrv_cWbdbVersionShortStr\b" | awk '{print $3}'`

  if [ -z $ver ]; then
    echo "Unable to get pwr version"
    ver="V00"
  fi
  ver=${ver:2:2}
fi

# Generate version help file
{
  if [ ! -e $pwre_sroot/tools/pkg/suse/pwr/pwr.spec ]; then
    echo "Controlfile not found"
    exit 1
  fi
  datfile=$pwre_sroot/tools/pkg/suse/pwr/pwr.spec

  echo "<topic> version"
  d=`eval date +\"%F %X\"`

  {
    let printout=0
    while read line; do
      if [ "${line:0:6}" = "Name: " ]; then 
        package=${line#Name: }
      fi
      if [ "${line:0:9}" = "Version: " ]; then
        version=${line#Version: }
      fi
      if [ "${line:0:9}" = "Release: " ]; then
        release=${line#Release: }
      fi
      if [ "${line:0:11}" = "BuildArch: " ]; then
        arch=${line#BuildArch: }
      fi
      if [ "${line:0:9}" = "Summary: " ]; then
        summary=${line#Summary: }
      fi

      if [ "${line:0:10}" = "%changelog" ]; then
        echo ""
        echo "<image> pwr_logga.gif"
	echo ""
	echo ""
	echo ""
        echo "<b>Proview V${version:0:3}"
	echo "Version V$version"
        echo ""
        echo "Copyright © 2005-${d:0:4} SSAB AB"
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
        echo "E-mail postmaster@proview.se <weblink> mailto:postmaster@proview.se"
        echo "Internet www.proview.se <weblink> http://www.proview.se"
	echo ""
	echo ""
	echo "<b>Package"
	echo "Package $package""-""$version""-""$release"".""$arch"
	echo "Build date $d"
	echo "Package description:"
	echo "$summary"
	echo ""

        printout=1
      else
        if [ $printout -eq 1 ]; then
	  if [ "${line:0:1}" = "-" ]; then
            echo $line
	  else
            echo ${line:2}
          fi
        fi
      fi
    done
  } < $datfile
  echo "</topic>"
} > $pwr_eexe/wtt_version_help.dat

# Convert to html
co_convert -t -d $pwr_doc $pwr_eexe/wtt_version_help.dat

if [ "$1" == "-v" ]; then
  exit
fi

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwr$ver
pkgsrc=$pwre_sroot/tools/pkg/suse/pwr

echo "-- Building pwr$ver"

# Create directories
mkdir -p $pkgroot/rpm/BUILD
mkdir -p $pkgroot/rpm/RPMS
mkdir -p $pkgroot/rpm/SPECS
mkdir -p $pkgroot/rpm/SOURCES
mkdir -p $pkgroot/rpm/SRPMS
mkdir -p $pkgroot/usr/share/doc/pwr$ver
mkdir -p $pkgroot/usr/pwrp
mkdir -p $pkgroot/etc

find $pkgroot -type d | xargs chmod 755

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwr$ver

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwr$ver
gzip -fq --best $pkgroot/usr/share/doc/pwr$ver/changelog

# Man pages
mkdir -p $pkgroot/usr/share/man/man1
cp $pkgsrc/pwr.1 $pkgroot/usr/share/man/man1/pwr.1
gzip -fq --best $pkgroot/usr/share/man/man1/pwr.1

# Copy proview
mkdir $pkgroot/usr/pwr$ver
currentdir="`eval pwd`"
tarfile=$pwre_broot/$pwre_target/bld/pkg/pwrtmp.tar
cd $pwre_broot
echo "-- copy release to package tree"
tar -cf $tarfile $pwre_target/exp
cd $pkgroot/usr/pwr$ver
tar -xf $tarfile
rm $tarfile
cd $currentdir

# Remove not needed libraries
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_flow*.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_ge*.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_glow*.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_tlog*.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_wb*.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_xtt*.a

# Copy configuration files to cnf
cp $pkgsrc/proview.cnf $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf
cp $pkgsrc/pwrp_profile $pkgroot/etc
chmod a+x $pkgroot/etc/pwrp_profile

# Copy adm files to cnf
cp $pwre_sroot/tools/pkg/deb/adm/pwr_setup.sh $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf
echo "pwrp set base V${ver:0:1}.${ver:1:1}" >> $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/pwr_setup.sh
chmod a+x $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/pwr_setup.sh
cp $pwre_sroot/tools/pkg/deb/adm/pwra_env.sh $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf
chmod a+x $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/pwra_env.sh
cp $pwre_sroot/tools/pkg/deb/adm/pwr_volumelist.dat $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf
cp $pwre_sroot/tools/pkg/deb/adm/pwr_user2.dat $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf
cp $pwre_sroot/tools/pkg/deb/adm/proview_icon.png $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf

# Copy user to cnf
mkdir $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bashrc $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
echo "source $aroot/db/pwr_setup.sh" >> $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user/.bashrc
cp $pwre_sroot/tools/pkg/deb/user/.bash_profile $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
#cp $pwre_sroot/tools/pkg/deb/user/.mwmrc $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.rtt_start $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xtt_start $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
#cp $pwre_sroot/tools/pkg/deb/user/.xsession $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/wtt_init.pwr_com $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/wtt_init1.pwr_com $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user

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
} > $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user/proview$ver.desktop

# Create package
echo "-- Building package"

  rpmbuild -bb  --quiet \
                --define "_topdir $pkgroot/rpm" \
                --define "ver $ver" \
                --define "pwre_target $pwre_target" \
                --buildroot $pkgroot $pkgsrc/pwr.spec > /dev/null 2>&1

mv $pkgroot/rpm/RPMS/i386/*.rpm $pwre_broot/$pwre_target/bld/pkg/.
rm -r $pkgroot












