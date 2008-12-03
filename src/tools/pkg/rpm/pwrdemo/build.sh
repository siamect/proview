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

proot=/data0/pwrp/pwrdemo$ver
pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwrdemo$ver
pkgsrc=$pwre_sroot/tools/pkg/rpm/pwrdemo

echo "-- Building pwrdemo$ver"

# Create directories
echo "-- Create package tree"
mkdir -p $pkgroot/rpm/BUILD
mkdir -p $pkgroot/rpm/RPMS
mkdir -p $pkgroot/rpm/SPECS
mkdir -p $pkgroot/rpm/SOURCES
mkdir -p $pkgroot/rpm/SRPMS
mkdir -p $pkgroot/usr/share/doc/pwrdemo$ver
mkdir -p $pkgroot/usr/pwrp

find $pkgroot -type d | xargs chmod 755

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwrdemo$ver

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwrdemo$ver
gzip -fq --best $pkgroot/usr/share/doc/pwrdemo$ver/changelog

# Man pages

# Copy pwrdemo
mkdir $pkgroot/usr/pwrp/pwrdemo$ver
currentdir="`eval pwd`"
tarfile=$pwre_broot/$pwre_target/bld/pkg/pwrtmp.tar
cd $proot
cd ..
echo "-- copy project to package tree"
tar -cf $tarfile pwrdemo$ver
cd $pkgroot/usr/pwrp
tar -xf $tarfile
rm $tarfile
cd $currentdir

# Create package
echo "-- Building package"
  rpmbuild -bb --quiet \
               --define "_topdir $pkgroot/rpm" \
               --define "ver $ver" \
               --buildroot $pkgroot $pkgsrc/pwrdemo.spec > /dev/null 2>&1
                                                                                
mv $pkgroot/rpm/RPMS/i386/*.rpm $pwre_broot/$pwre_target/bld/pkg/.
rm -r $pkgroot
