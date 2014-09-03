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
  if [ ! -e $pwre_sroot/tools/pkg/cygwin/pwr/control ]; then
    echo "Controlfile not found"
    exit 1
  fi
  datfile=$pwre_sroot/tools/pkg/cygwin/pwr/control

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
} > $pwr_eexe/wtt_version_help.dat

# Convert to html
co_convert -t -d $pwr_doc $pwr_eexe/wtt_version_help.dat

{
  echo "<html><head>"
  echo "<meta http-equiv=\"Refresh\" content=\"5;../wtt_version_help_version.html\">"
  echo "</head></html>"
} > $pwr_doc/en_us/package_version.html

if [ "$1" == "-v" ]; then
  exit
fi

pkgroot=$pwre_broot/$pwre_target/bld/pkg/pwr$ver
pkgsrc=$pwre_sroot/tools/pkg/cygwin/pwr
packagename=pwr$ver-$version.tar.gz

echo "-- Building pwr$ver"

# Create directories
mkdir -p $pkgroot/etc/pwr$ver
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
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_flow.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_ge.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_glow.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_tlog.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_wb.a
rm $pkgroot/usr/pwr$ver/$pwre_target/exp/lib/libpwr_xtt.a

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
cp $pwre_sroot/tools/pkg/cygwin/user/.bashrc $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
echo "source $aroot/db/pwr_setup.sh" >> $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user/.bashrc
cp $pwre_sroot/tools/pkg/deb/user/.bash_profile $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.rtt_start $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/.xtt_start $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/wtt_init.pwr_com $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/deb/user/wtt_init1.pwr_com $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/cygwin/user/Proview.lnk $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user
cp $pwre_sroot/tools/pkg/cygwin/user/proview_icon.ico $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user

# Generate startscript
{
  echo "#!/bin/bash"
  echo "export PATH=/usr/local/bin:/usr/bin"
  echo "export pwra_db=$aroot/db"
  echo "source \$pwra_db/pwr_setup.sh"
  echo "source \$pwra_db/pwra_env.sh set base V""${version:0:3}"""
  echo "source \$pwra_db/pwra_env.sh set bus"
  echo "export DISPLAY=127.0.0.1:0"
  echo "wb_gtk -p pwrp pwrp"
} > $pkgroot/usr/pwr$ver/$pwre_target/exp/cnf/user/pwrstart

# Create package
echo "-- Building package"

mkdir -p $pkgroot/etc/pwr$ver
cd $pkgroot
find * -type f -exec echo "rm -f /{}" \; > $pkgroot/etc/pwr$ver/rmfiles.sh
echo "rm -r /usr/pwr$ver" >> $pkgroot/etc/pwr$ver/rmfiles.sh

# control
cp $pkgsrc/control $pkgroot/etc/pwr$ver
cp $pkgsrc/prerm $pkgroot/etc/pwr$ver
cp $pkgsrc/postinst $pkgroot/etc/pwr$ver

tar -czf ../$packagename *

rm -r $pkgroot












