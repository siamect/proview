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

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwrrt
pkgsrc=$pwre_sroot/tools/pkg/deb/pwrrt


# Create directories
echo "-- Create package tree"
mkdir -p $pkgroot/DEBIAN
mkdir -p $pkgroot/usr/share/doc/pwrrt
mkdir -p $pkgroot/etc/init.d

find $pkgroot -type d | xargs chmod 755

# control
cp $pkgsrc/control $pkgroot/DEBIAN
cp $pkgsrc/postinst $pkgroot/DEBIAN
cp $pkgsrc/prerm $pkgroot/DEBIAN

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
chmod a+x $pkgroot/etc/pwrp_profile
cp $pkgsrc/pwr $pkgroot/etc/init.d
chmod a+x $pkgroot/etc/init.d/pwr

# Man pages
mkdir -p $pkgroot/usr/share/man/man1
cp $pkgsrc/pwr.1 $pkgroot/usr/share/man/man1/pwr.1
gzip -fq --best $pkgroot/usr/share/man/man1/pwr.1

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
cp $pwr_exe/wb_distr_keepboot.sh $pkgroot/usr/pwrrt/exe
cd $currentdir

# Copy configuration files to cnf
cp $pkgsrc/proview.cnf $pkgroot/usr/pwrrt/cnf

# Copy op to cnf
mkdir $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.bashrc $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.bash_profile $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.mwmrc $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.rtt_start $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.xtt_start $pkgroot/usr/pwrrt/cnf/op
cp $pwre_sroot/tools/pkg/deb/op/.xsession $pkgroot/usr/pwrrt/cnf/op

# Copy user to cnf
mkdir $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bashrc $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bash_profile $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.mwmrc $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.rtt_start $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xtt_start $pkgroot/usr/pwrrt/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xsession $pkgroot/usr/pwrrt/cnf/user

# Create package
echo "-- Building package"
if which fakeroot > /dev/null; then
  fakeroot dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
else
  dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
fi

rm -r $pkgroot












