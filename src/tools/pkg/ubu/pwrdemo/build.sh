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
pkgsrc=$pwre_sroot/tools/pkg/deb/pwrdemo

echo "-- Building pwrdemo$ver"

# Create directories
mkdir -p $pkgroot/DEBIAN
mkdir -p $pkgroot/usr/share/doc/pwrdemo$ver
mkdir -p $pkgroot/usr/pwrp

find $pkgroot -type d | xargs chmod 755

# control
cp $pkgsrc/control $pkgroot/DEBIAN
echo "#!/bin/sh" > $pkgroot/DEBIAN/postinst
echo "ver=\"$ver\"" >> $pkgroot/DEBIAN/postinst
echo "pwre_target=\"$pwre_target\"" >> $pkgroot/DEBIAN/postinst
cat $pkgsrc/postinst >> $pkgroot/DEBIAN/postinst
chmod a+x $pkgroot/DEBIAN/postinst

echo "#!/bin/sh" > $pkgroot/DEBIAN/prerm
echo "ver=\"$ver\"" >> $pkgroot/DEBIAN/prerm
cat $pkgsrc/prerm >> $pkgroot/DEBIAN/prerm
chmod a+x $pkgroot/DEBIAN/prerm

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwrdemo$ver

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwrdemo$ver
gzip -fq --best $pkgroot/usr/share/doc/pwrdemo$ver/changelog

# changelog.Debian
cp $pkgsrc/changelog.Debian $pkgroot/usr/share/doc/pwrdemo$ver
gzip -fq --best $pkgroot/usr/share/doc/pwrdemo$ver/changelog.Debian

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
if which fakeroot > /dev/null; then
  fakeroot dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
else
  dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
fi

rm -r $pkgroot












