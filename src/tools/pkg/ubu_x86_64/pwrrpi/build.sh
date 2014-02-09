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

if [ "$1" == "-v" ]; then
  exit
fi

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwrrpi$ver
pkgsrc=$pwre_sroot/tools/pkg/ubu_x86_64/pwrrpi
rpi_target=os_linux/hw_arm

echo "-- Building pwrrpi$ver"

# Create directories
mkdir -p $pkgroot/DEBIAN
mkdir -p $pkgroot/usr/share/doc/pwrrpi$ver
mkdir -p $pkgroot/usr/pwrp
mkdir -p $pkgroot/etc

find $pkgroot -type d | xargs chmod 755

# control
cp $pkgsrc/control $pkgroot/DEBIAN
echo "#!/bin/bash" > $pkgroot/DEBIAN/postinst
echo "ver=\"$ver\"" >> $pkgroot/DEBIAN/postinst
echo "pwre_target=\"$pwre_target\"" >> $pkgroot/DEBIAN/postinst
cat $pkgsrc/postinst >> $pkgroot/DEBIAN/postinst
chmod a+x $pkgroot/DEBIAN/postinst
cp $pkgsrc/prerm $pkgroot/DEBIAN

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwrrpi$ver

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwrrpi$ver
gzip -fq --best $pkgroot/usr/share/doc/pwrrpi$ver/changelog

# changelog.Debian
cp $pkgsrc/changelog.Debian $pkgroot/usr/share/doc/pwrrpi$ver
gzip -fq --best $pkgroot/usr/share/doc/pwrrpi$ver/changelog.Debian

# Copy proview
mkdir $pkgroot/usr/pwr$ver
currentdir="`eval pwd`"
tarfile=$pwre_broot/$pwre_target/bld/pkg/pwrtmp.tar
cd $pwre_broot
echo "-- copy release to package tree"
tar -cf $tarfile $rpi_target/exp
cd $pkgroot/usr/pwr$ver
tar -xf $tarfile
rm $tarfile
cd $currentdir


# Copy adm files to cnf
#echo "pwrp set base V${ver:0:1}.${ver:1:1}" >> $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/pwr_setup.sh

# Create package
echo "-- Building package"
if which fakeroot > /dev/null; then
  fakeroot dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
else
  dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
fi

rm -r $pkgroot












