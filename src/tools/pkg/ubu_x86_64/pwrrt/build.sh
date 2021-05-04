#!/bin/bash

hw=ubu_x86_64

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
  if [ ! -e $pwre_sroot/tools/pkg/$hw/pwrrt/control ]; then
    echo "Controlfile not found"
    exit 1
  fi
  datfile=$pwre_sroot/tools/pkg/$hw/pwrrt/control

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
        echo "<b>ProviewR V${version:0:3}"
	echo "Version V$version"
        echo ""
        echo "Copyright � 2005-${d:0:4} SSAB EMEA AB"
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
} > $pwr_eload/xtt_version_help.dat

# Convert to html
co_convert -t -d $pwr_doc $pwr_eload/xtt_version_help.dat

{
  echo "<html><head>"
  echo "<meta http-equiv=\"Refresh\" content=\"5;../xtt_version_help_version.html\">"
  echo "</head></html>"
} > $pwr_doc/en_us/package_version.html

# Print rt version file
echo "Version: $version" > $pwr_eexe/rt_version.dat

if [ "$1" == "-v" ]; then
  exit
fi

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwrrt
pkgsrc=$pwre_sroot/tools/pkg/$hw/pwrrt

# Create directories
echo "-- Create package tree"
mkdir -p $pkgroot/DEBIAN
mkdir -p $pkgroot/usr/share/doc/pwrrt
mkdir -p $pkgroot/etc/init.d
mkdir -p $pkgroot/etc/systemd/system

find $pkgroot -type d | xargs chmod 755

# control
cp $pkgsrc/control $pkgroot/DEBIAN
cp $pkgsrc/postinst $pkgroot/DEBIAN
cp $pkgsrc/prerm $pkgroot/DEBIAN
cp $pkgsrc/postrm $pkgroot/DEBIAN
chmod 755 $pkgroot/DEBIAN/postinst
chmod 755 $pkgroot/DEBIAN/prerm
chmod 755 $pkgroot/DEBIAN/postrm
chmod 644 $pkgroot/DEBIAN/control

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwrrt

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwrrt
gzip -fq --best $pkgroot/usr/share/doc/pwrrt/changelog

# changelog.Debian
cp $pkgsrc/changelog.Debian $pkgroot/usr/share/doc/pwrrt
gzip -fq --best $pkgroot/usr/share/doc/pwrrt/changelog.Debian

# Startup files
cp $pkgsrc/pwrp_profile $pkgroot/etc
cp $pkgsrc/pwr $pkgroot/etc/init.d
chmod a+x $pkgroot/etc/init.d/pwr
cp $pkgsrc/pwr.service $pkgroot/etc/systemd/system/

# Man pages
mkdir -p $pkgroot/usr/share/man/man1
cp $pkgsrc/pwr.1 $pkgroot/usr/share/man/man1/pwr.1
gzip -fq --best $pkgroot/usr/share/man/man1/pwr.1
cp $pkgsrc/rt_ini.1 $pkgroot/usr/share/man/man1/rt_ini.1
gzip -fq --best $pkgroot/usr/share/man/man1/rt_ini.1
cp $pkgsrc/rt_xtt.1 $pkgroot/usr/share/man/man1/rt_xtt.1
gzip -fq --best $pkgroot/usr/share/man/man1/rt_xtt.1
cp $pkgsrc/rt_rtt.1 $pkgroot/usr/share/man/man1/rt_rtt.1
gzip -fq --best $pkgroot/usr/share/man/man1/rt_rtt.1

# Copy proview
mkdir $pkgroot/usr/pwrrt
echo "-- Copy release to package tree"
cp -r $pwre_broot/$pwre_target/exp/* $pkgroot/usr/pwrrt/
rm -r $pkgroot/usr/pwrrt/lib/*.a
rm -r $pkgroot/usr/pwrrt/exe/wb*
cp $pwr_eexe/wb_distr_keepboot.sh $pkgroot/usr/pwrrt/exe
cp $pkgsrc/pwr_mailto $pkgroot/usr/pwrrt/exe/

# Copy configuration files to cnf
cp $pkgsrc/proview.cnf $pkgroot/usr/pwrrt/cnf

# Copy op to cnf
mkdir $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/$hw/op/.rtt_start $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/$hw/op/.xtt_start $pkgroot/usr/pwrrt/cnf/op

# Copy user to cnf
mkdir $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/$hw/user/.rtt_start $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/$hw/user/.xtt_start $pkgroot/usr/pwrrt/cnf/user

# Create package
echo "-- Building package"
if which fakeroot > /dev/null; then
  fakeroot dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
else
  dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
fi

rm -r $pkgroot
