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
  if [ ! -e $pwre_sroot/tools/pkg/deb/pwr/control ]; then
    echo "Controlfile not found"
    exit 1
  fi
  datfile=$pwre_sroot/tools/pkg/deb/pwr/control

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
        echo "Copyright © 2003-${d:0:4} SSAB Oxelösund AB"
	echo "All Rights Reserved."
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
} > $pwr_exe/wtt_version_help.dat

if [ "$1" == "-v" ]; then
  exit
fi

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwr$ver
pkgsrc=$pwre_sroot/tools/pkg/deb/pwr

echo "-- Building pwr$ver"

# Create directories
mkdir -p $pkgroot/DEBIAN
mkdir -p $pkgroot/usr/share/doc/pwr$ver
mkdir -p $pkgroot/usr/pwrp
mkdir -p $pkgroot/etc

find $pkgroot -type d | xargs chmod 755

# control
cp $pkgsrc/control $pkgroot/DEBIAN
echo "#!/bin/sh" > $pkgroot/DEBIAN/postinst
echo "ver=\"$ver\"" >> $pkgroot/DEBIAN/postinst
echo "pwre_target=\"$pwre_target\"" >> $pkgroot/DEBIAN/postinst
cat $pkgsrc/postinst >> $pkgroot/DEBIAN/postinst
chmod a+x $pkgroot/DEBIAN/postinst
cp $pkgsrc/prerm $pkgroot/DEBIAN

# copyright
cp $pkgsrc/copyright $pkgroot/usr/share/doc/pwr$ver

# changelog
cp $pkgsrc/changelog $pkgroot/usr/share/doc/pwr$ver
gzip -fq --best $pkgroot/usr/share/doc/pwr$ver/changelog

# changelog.Debian
cp $pkgsrc/changelog.Debian $pkgroot/usr/share/doc/pwr$ver
gzip -fq --best $pkgroot/usr/share/doc/pwr$ver/changelog.Debian

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

# Copy user to cnf
mkdir $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bashrc $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.bash_profile $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.mwmrc $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.rtt_start $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xtt_start $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xsession $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user

# Create package
echo "-- Building package"
if which fakeroot > /dev/null; then
  fakeroot dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
else
  dpkg -b $pkgroot $pwre_broot/$pwre_target/bld/pkg
fi

rm -r $pkgroot












