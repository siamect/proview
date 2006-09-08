###############################################################
# PACKAGE INFORMATION
#
# Notes:
#
# - rpm documentation
#   See www.rpm.org.
#


Name: pwr42
Summary: Proview/R development and runtime environment
Version: 4.2.0
Release: 7
Copyright: (c) 2006 SSAB Oxelösund AB, Sweden
#License: GPL
BuildArch: i386
Packager: robert.karlsson@ssabox.com
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

#%clean

%files
%defattr(-,root,root)
/usr
/etc

%post

aroot="/usr/pwrp/adm"

# Create group pwrp
if ! grep -q "\bpwrp:" /etc/group; then
  echo "-- Add group pwrp"
  groupadd pwrp
fi

if ! grep -q "\bpwrp:" /etc/passwd; then
  echo "-- Add user pwrp"
  useradd -s /bin/bash -p aaupl/kQs1p3U -g pwrp -d /home/pwrp pwrp
  if [ ! -e /home/pwrp ]; then
    mkdir /home/pwrp
  fi
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.bashrc /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.bash_profile /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.mwmrc /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.rtt_start /home/pwrp
  chmod a+x /home/pwrp/.rtt_start
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.xtt_start /home/pwrp
  chmod a+x /home/pwrp/.xtt_start
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/.xsession /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/wtt_init.pwr_com /home/pwrp
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/user/wtt_init1.pwr_com /home/pwrp

  chown -R pwrp /home/pwrp
  chgrp -R pwrp /home/pwrp
fi

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
if [ ! -e $aroot/db/pwr_volumelist.dat ]; then
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/pwr_volumelist.dat $aroot/db
fi
if [ ! -e $aroot/db/pwr_user.dat ]; then
  cp /usr/pwr%{ver}/%{pwre_target}/exp/cnf/pwr_user.dat $aroot/db
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
* Tue Sep 06 2006 Claes Sjofors <claes.sjofors@ssabox.com> 4.2.0-7
- Xtt: System-Device table works for attribute object signals and channels.
- Profibus: Bugfix in area offset.
- Ge: Adjustment property added for text in value and table objects.
- Ge: Format for Time and DeltaTime in table.
- Plc: bugfix for StoIp and CStoIp, supplied value was not fetched for not connected objects.
- User b66 added to operator station.
- BaseSensorFo: inputs Error and Warning added.
- Local mq replaced.
- ABB_ACS800_Slave: IORack bit was missing.
- Bugfix in classcache for large classes.
- Volume volref version check for not yet loaded volume removed.
- Problems with mq quota fixed.
- Profibus: problems with user PRM data for bigendian slaves fixed.
- Base release 
