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
pkgsrc=$pwre_sroot/tools/pkg/ubu/pwrdemo

echo "-- Building pwrdemo$ver"

# Create directories
mkdir -p $pkgroot/DEBIAN
mkdir -p $pkgroot/usr/share/doc/pwrdemo$ver
mkdir -p $pkgroot/usr/pwrp

find $pkgroot -type d | xargs chmod 755

# control
cp $pkgsrc/control $pkgroot/DEBIAN
echo "#!/bin/bash" > $pkgroot/DEBIAN/postinst
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
mkdir $pkgroot/usr/pwrp/pwrdemo$ver/cnf
currentdir="`eval pwd`"
tarfile=$pwre_broot/$pwre_target/bld/pkg/pwrtmp.tar
cd $proot
cd ..
echo "-- copy project to package tree"
tar -cf $tarfile pwrdemo$ver
cd $pkgroot/usr/pwrp
tar -xf $tarfile
rm $tarfile
rm -r pwrdemo$ver/.git
cd $currentdir

# Generate desktop file
{
  echo "[Desktop Entry]"
  echo "Version=1.0"
  echo "Encoding=UTF-8"
  echo "Name=Proview Demo"
  echo "Type=Application"
  echo "Comment=Start Proview Demo"
  echo "Exec=xterm -title \"Proview Demo Console\" -e 'source /etc/pwrp_profile;export pwra_db=$aroot/db;source \$pwra_db/pwr_setup.sh;source \$pwra_db/pwra_env.sh set project pwrdemo""$ver"";export PWR_BUS_ID=999;pwr_rtmon'"
  echo "Icon=$aroot/db/proview_icon.png"
  echo "Categories=GNOME;GTK;Application;"
} > $pkgroot/usr/pwrp/pwrdemo$ver/cnf/proviewdemo$ver.desktop

# Create package
echo "-- Building package"
if which fakeroot > /dev/null; then
  fakeroot dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
else
  dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
fi

rm -r $pkgroot












