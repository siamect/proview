#!/bin/bash

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
  if [ ! -e $pwre_sroot/tools/pkg/suse/pwrsev/pwrsev.spec ]; then
    echo "Controlfile not found"
    exit 1
  fi
  datfile=$pwre_sroot/tools/pkg/suse/pwrsev/pwrsev.spec

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
} > $pwr_eexe/sev_xtt_version_help.dat

# Convert to html
co_convert -t -d $pwr_doc $pwr_eexe/sev_xtt_version_help.dat

{
  echo "<html><head>"
  echo "<meta http-equiv=\"Refresh\" content=\"5;../sev_xtt_version_help_version.html\">"
  echo "</head></html>"
} > $pwr_doc/en_us/package_version.html

if [ "$1" == "-v" ]; then
  exit
fi

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwrsev
pkgsrc=$pwre_sroot/tools/pkg/suse/pwrsev


# Create directories
echo "-- Create package tree"
mkdir -p $pkgroot/rpm/BUILD
mkdir -p $pkgroot/rpm/RPMS
mkdir -p $pkgroot/rpm/SPECS
mkdir -p $pkgroot/rpm/SOURCES
mkdir -p $pkgroot/rpm/SRPMS
mkdir -p $pkgroot/usr/share/doc/pwrsev
mkdir -p $pkgroot/etc
# init.d is a symlink on fedora that is owned by with chkconfig and causes conflict
# mkdir $pkgroot/etc/init.d

find $pkgroot -type d | xargs chmod 755

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwrsev

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwrsev
gzip -fq --best $pkgroot/usr/share/doc/pwrsev/changelog

# Startup files
cp $pkgsrc/pwrp_profile $pkgroot/etc
chmod a+x $pkgroot/etc/pwrp_profile
#cp $pkgsrc/pwrsev $pkgroot/etc/init.d
#chmod a+x $pkgroot/etc/init.d/pwrsev

# Man pages
mkdir -p $pkgroot/usr/share/man/man1
cp $pkgsrc/pwrsev.1 $pkgroot/usr/share/man/man1/pwrsev.1
gzip -fq --best $pkgroot/usr/share/man/man1/pwrsev.1
cp $pkgsrc/sev_ini.1 $pkgroot/usr/share/man/man1/sev_ini.1
gzip -fq --best $pkgroot/usr/share/man/man1/sev_ini.1
cp $pkgsrc/sev_xtt.1 $pkgroot/usr/share/man/man1/sev_xtt.1
gzip -fq --best $pkgroot/usr/share/man/man1/sev_xtt.1

# Copy proview
mkdir $pkgroot/usr/pwrsev
currentdir="`eval pwd`"
tarfile=$pwre_broot/$pwre_target/bld/pkg/pwrtmp.tar
cd $pwre_broot/$pwre_target/exp
echo "-- Copy release to package tree"
tar -cf $tarfile exe/rt_qmon exe/rt_prio exe/sev_ini exe/sev_xtt exe/sev_xtt_gtk exe/sev_server exe/*.gif exe/*.png exe/sev_xtt_version_help.dat exe/pwr_pkg.sh exe/en_us/*.txt doc/*
cd $pkgroot/usr/pwrsev
mkdir cnf
tar -xf $tarfile
rm $tarfile
cp $pwr_eexe/wb_distr_keepboot.sh $pkgroot/usr/pwrsev/exe
cd $currentdir

# Copy configuration files to cnf
cp $pkgsrc/proview.cnf $pkgroot/usr/pwrsev/cnf
cp $pkgsrc/pwrsev $pkgroot/usr/pwrsev/cnf

# Copy op to cnf
mkdir $pkgroot/usr/pwrsev/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.bashrc $pkgroot/usr/pwrsev/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.bash_profile $pkgroot/usr/pwrsev/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.rtt_start $pkgroot/usr/pwrsev/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.xtt_start $pkgroot/usr/pwrsev/cnf/op

# Copy user to cnf
mkdir $pkgroot/usr/pwrsev/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bashrc $pkgroot/usr/pwrsev/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bash_profile $pkgroot/usr/pwrsev/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.rtt_start $pkgroot/usr/pwrsev/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xtt_start $pkgroot/usr/pwrsev/cnf/user

# Create package
echo "-- Building package"

  rpmbuild -bb --quiet \
               --define "_topdir $pkgroot/rpm" \
               --buildroot $pkgroot $pkgsrc/pwrsev.spec > /dev/null 2>&1

mv $pkgroot/rpm/RPMS/i386/*.rpm $pwre_broot/$pwre_target/bld/pkg/.
rm -r $pkgroot












