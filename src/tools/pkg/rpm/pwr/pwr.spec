###############################################################
# PACKAGE INFORMATION
#
# Notes:
#
# - rpm documentation
#   See www.rpm.org.
#


Name: pwr48
Summary: Proview/R development and runtime environment
Version: 4.8.4
Release: 1
License: GPL
BuildArch: i386
Packager: claes.sjofors@proview.se
Requires: gcc, gcc-c++, xorg-x11-fonts-ISO8859-1-100dpi, xorg-x11-fonts-ISO8859-1-75dpi, xorg-x11-fonts-Type1, xterm
Group: Development/Tools/Other
Url: http://www.proview.se

%description
For more information please see www.proview.se.

#%prep
#%setup
#%build

%install

echo "$ver"

# Generate version help file
{
  echo "<topic> version"
  d=`eval date +\"%F %X\"`

  echo ""
  echo "<image> pwr_logga.gif"
  echo ""
  echo ""
  echo ""
  echo "<b>Proview V%{version}"
  echo "Version V%{version}-%{release}"
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
  echo "E-mail postmaster@proview.se"
  echo "Internet www.proview.se <link> http://www.proview.se/index.html"
  echo ""
  echo ""
  echo "<b>Package"
  echo "Package %{name}-%{version}-%{release}.i586.rpm"
  echo "Build date $d"
  echo "Package description:"
  echo ""
  echo %{summary}
  echo "</topic>"
} > %{buildroot}/usr/pwr%{ver}/%{pwre_target}/exp/exe/wtt_version_help.dat

# Convert to html
co_convert -t -d %{buildroot}/usr/pwr%{ver}/%{pwre_target}/exp/doc %{buildroot}/usr/pwr%{ver}/%{pwre_target}/exp/exe/wtt_version_help.dat

{
  echo "<html><head>"
  echo "<meta http-equiv=\"Refresh\" content=\"5;../wtt_version_help_version.html\">"
  echo "</head></html>"
} > %{buildroot}/usr/pwr%{ver}/%{pwre_target}/exp/doc/en_us/package_version.html

%clean

%files
%defattr(-,root,root)
/usr
/etc

%post

aroot="/usr/pwrp/adm"

# Create group pwrp
if getent group pwrp > /dev/null; then
  echo "-- group pwrp already exist"
else
  if groupadd pwrp; then
    echo "-- group pwrp added"
  fi
fi

if getent passwd pwrp > /dev/null; then
  echo "-- user pwrp already exist"
else
  new_user=1
  if useradd -s /bin/bash -p aaupl/kQs1p3U -g pwrp -d /home/pwrp pwrp; then
    echo "-- user pwrp added"
  fi
  if [ ! -e /home/pwrp ]; then
    mkdir /home/pwrp
  fi
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.bashrc /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.bash_profile /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.rtt_start /home/pwrp
  chmod a+x /home/pwrp/.rtt_start
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.xtt_start /home/pwrp
  chmod a+x /home/pwrp/.xtt_start
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/wtt_init.pwr_com /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/wtt_init1.pwr_com /home/pwrp

  chown -R pwrp /home/pwrp
  chgrp -R pwrp /home/pwrp
fi
if [ ! -e /home/pwrp/Desktop ]; then
  mkdir /home/pwrp/Desktop
  chown pwrp:pwrp /home/pwrp/Desktop	
fi
cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/proview%{ver}.desktop /home/pwrp/Desktop
chown pwrp:pwrp /home/pwrp/Desktop/proview%{ver}.desktop
chmod a+x /home/pwrp/Desktop/proview%{ver}.desktop

echo "Change owner of files to pwrp"
chown -R pwrp /usr/pwr%{ver}
chgrp -R pwrp /usr/pwr%{ver}

chmod u+s /usr/pwr%{ver}/%{pwre_target}/exp/exe/rt_ini
chmod u+s /usr/pwr%{ver}/%{pwre_target}/exp/exe/rt_rtt
chmod u+s /usr/pwr%{ver}/%{pwre_target}/exp/exe/rt_xtt
chmod u+s /usr/pwr%{ver}/%{pwre_target}/exp/exe/rt_bck

# Copy configuration files
new_cnf=0
if [ ! -e /etc/proview.cnf ]; then
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/proview.cnf /etc
  new_cnf=1
fi

# Add pwrp_profile to profile
if ! grep -q "/etc/pwrp_profile\b" /etc/profile; then
  cat >> /etc/profile <<-EOF
	if [ -e /etc/pwrp_profile ]; then
	  source /etc/pwrp_profile
	fi
EOF
fi

# Create and copy adm
if [ ! -e $aroot/db ]; then
  mkdir -p $aroot/db
fi

cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/pwra_env.sh $aroot/db
cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/pwr_setup.sh $aroot/db
cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/proview_icon.png $aroot/db
if [ ! -e $aroot/db/pwr_volumelist.dat ]; then
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/pwr_volumelist.dat $aroot/db
fi
if [ ! -e $aroot/db/pwr_user2.dat ]; then
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/pwr_user2.dat $aroot/db
fi

# Insert base in projectlist
pwrver=%{ver}
if [ ! -e $aroot/db/pwr_projectlist.dat ]; then
  echo "%base V${pwrver:0:1}.${pwrver:1:1} /usr/pwr$pwrver" > $aroot/db/pwr_projectlist.dat
else
  set +e
  ptst=`eval grep "^%base" $aroot/db/pwr_projectlist.dat | grep "\bV${pwrver:0:1}.${pwrver:1:1}\b"`
  set -e
  if [ "$ptst" = "" ]; then
    echo "%base V${pwrver:0:1}.${pwrver:1:1} /usr/pwr$pwrver" >> $aroot/db/pwr_projectlist.dat
  fi
fi

# Change group to pwrp
chgrp pwrp $aroot
chmod g+w $aroot
chgrp -R pwrp $aroot/*
chmod -R g+w $aroot/*
chmod a+w /usr/pwr%{ver}/%{pwre_target}/exp/load/*.dbs

# Create project root
if [ ! -e /usr/local/pwrp ]; then
  mkdir /usr/local/pwrp
  chgrp pwrp /usr/local/pwrp
  chmod g+w /usr/local/pwrp
fi

#%preun

#%postun

%changelog
* Thu Mar 01 2012 Claes Sjofors <claes.sjofors@proview.se> 4.8.4-1
   - Buffer input and output channels and signals.
* Thu Dec 20 2012 Claes Sjofors <claes.sjofors@proview.se> 4.8.3-1
   - Post server for sending emails and sms.
   - Report server.
* Sun Jun 12 2011 Claes Sjofors <claes.sjofors@proview.se> 4.8.2-1
  - Support for Hilscher cifX boards.
  - Support for Hilscher cifX Profinet Controller.
  - UDP communication configured with IO objects.
  - Support for USB joystick.
* Thu Feb 24 2011 Claes Sjofors <claes.sjofors@proview.se> 4.8.1-2
   - Profinet configurator, ordernumber showed in module enum list. 
   - Curve window, second mark and export dialog added.
   - Profibus/Profinet configurator, default module class setting added, and Appl button added.
   - Configurator fix, Delete, Create and Move object menu items were not insensive in view mode.
   - Plc link, problem with uppercase nodename for opt-file fixed.
   - Xtt, problems with OpPlace and XttGraph objects in SharedVolume fixed.
   - Plc compile, problem with compile of component subwindows fixed.
   - Wb object editor, Ctrl DoubleClick MB1 didn't work on attribute references.
   - Flow alloc color problem fixed.
   - wb locale fix for ubuntu.
* Mon Jan 24 2011 Claes Sjofors <claes.sjofors@proview.se> 4.8.1-1
  - I/O support for Arduino USB board.
  - Remote support for Websphere Message Queue.
  - Operator event logging and replay.
* Sun Nov 28 2010 Claes Sjofors <claes.sjofors@proview.se> 4.8.0-1
  - Base release.
