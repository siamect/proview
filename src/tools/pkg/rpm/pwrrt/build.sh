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
  if [ ! -e $pwre_sroot/tools/pkg/rpm/pwrrt/pwrrt.spec ]; then
    echo "Controlfile not found"
    exit 1
  fi
  datfile=$pwre_sroot/tools/pkg/rpm/pwrrt/pwrrt.spec

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
} > $pwr_eexe/xtt_version_help.dat

# Convert to html
co_convert -t -d $pwr_doc $pwr_eexe/xtt_version_help.dat

# Print rt version file
echo "Version: $version" > $pwr_eexe/rt_version.dat

if [ "$1" == "-v" ]; then
  exit
fi

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwrrt
pkgsrc=$pwre_sroot/tools/pkg/rpm/pwrrt


# Create directories
echo "-- Create package tree"
mkdir -p $pkgroot/rpm/BUILD
mkdir -p $pkgroot/rpm/RPMS
mkdir -p $pkgroot/rpm/SPECS
mkdir -p $pkgroot/rpm/SOURCES
mkdir -p $pkgroot/rpm/SRPMS
mkdir -p $pkgroot/usr/share/doc/pwrrt
mkdir -p $pkgroot/etc
# init.d is a symlink on fedora that is owned by with chkconfig and causes conflict
#mkdir -p $pkgroot/etc/init.d

find $pkgroot -type d | xargs chmod 755

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwrrt

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwrrt
gzip -fq --best $pkgroot/usr/share/doc/pwrrt/changelog

# Startup files
cp $pkgsrc/pwrp_profile $pkgroot/etc
chmod a+x $pkgroot/etc/pwrp_profile
#cp $pkgsrc/pwr $pkgroot/etc/init.d
#chmod a+x $pkgroot/etc/init.d/pwr

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
currentdir="`eval pwd`"
tarfile=$pwre_broot/$pwre_target/bld/pkg/pwrtmp.tar
cd $pwre_broot/$pwre_target/exp
echo "-- Copy release to package tree"
tar -cf $tarfile *
cd $pkgroot/usr/pwrrt
tar -xf $tarfile
rm $tarfile
rm -r $pkgroot/usr/pwrrt/lib/*.a
rm -r $pkgroot/usr/pwrrt/exe/wb*
cp $pwr_eexe/wb_distr_keepboot.sh $pkgroot/usr/pwrrt/exe
cd $currentdir

# Copy configuration files to cnf
cp $pkgsrc/proview.cnf $pkgroot/usr/pwrrt/cnf
cp $pkgsrc/pwr $pkgroot/usr/pwrrt/cnf

# Copy op to cnf
mkdir $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.bashrc $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.bash_profile $pkgroot/usr/pwrrt/cnf/op
#cp $pwre_sroot/tools/pkg/deb/op/.mwmrc $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.rtt_start $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.xtt_start $pkgroot/usr/pwrrt/cnf/op
#cp $pwre_sroot/tools/pkg/deb/op/.xsession $pkgroot/usr/pwrrt/cnf/op

# Copy user to cnf
mkdir $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bashrc $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bash_profile $pkgroot/usr/pwrrt/cnf/user
#cp $pwre_sroot/tools/pkg/deb/user/.mwmrc $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.rtt_start $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xtt_start $pkgroot/usr/pwrrt/cnf/user
#cp $pwre_sroot/tools/pkg/deb/user/.xsession $pkgroot/usr/pwrrt/cnf/user

# Create package
echo "-- Building package"

 rpmbuild -bb --quiet \
               --define "_topdir $pkgroot/rpm" \
               --buildroot $pkgroot $pkgsrc/pwrrt.spec > /dev/null 2>&1

mv $pkgroot/rpm/RPMS/i386/*.rpm $pwre_broot/$pwre_target/bld/pkg/.
rm -r $pkgroot












