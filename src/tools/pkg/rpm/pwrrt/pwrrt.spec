###############################################################
# PACKAGE INFORMATION
#
# Notes:
#
# - rpm documentation
#   See www.rpm.org.
#


Name: pwrrt
Summary: Proview/R runtime environment
Version: 4.7.1
Release: 1
License: GPL
BuildArch: i386
Packager: claes.sjofors@proview.se
Requires: xorg-x11-fonts-ISO8859-1-100dpi, xorg-x11-fonts-ISO8859-1-75dpi, xorg-x11-fonts-Type1
Group: Applications/System
Url: http://www.proview.se

%description
For more information please see www.proview.se.

#%prep
#%setup
#%build

%install


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
} > %{buildroot}/usr/pwrrt/exe/xtt_version_help.dat

# Convert to html
co_convert -t -d %{buildroot}/usr/pwrrt/doc %{buildroot}/usr/pwrrt/exe/xtt_version_help.dat

{
  echo "<html><head>"
  echo "<meta http-equiv=\"Refresh\" content=\"5;../xtt_version_help_version.html\">"
  echo "</head></html>"
} > %{buildroot}/usr/pwrrt/doc/en_us/package_version.html

# Print rt version file
echo "Version: %{version}-%{release}" > %{buildroot}/usr/pwrrt/exe/rt_version.dat

#%clean

%files
%defattr(-,root,root)
/usr
/etc

%post

aroot="/usr/pwrp/adm"
proot="/pwrp"

# Create users ...

if getent group pwrp > /dev/null; then
  echo "-- group pwrp already exist"
else
  if groupadd pwrp; then
    echo "-- group pwrp added"
  fi
fi

if getent group b55 > /dev/null; then
  echo "-- group b55 already exist"
else
  if groupadd b55; then
    echo "-- group b55 added"
  fi
fi

if getent group skiftel > /dev/null; then
  echo "-- group skiftel already exist"
else
  if groupadd skiftel; then
    echo "-- group skiftel added"
  fi
fi

new_user=0
if getent passwd pwrp > /dev/null; then
  echo "-- user pwrp already exist"
else
  new_user=1

  if useradd -s /bin/bash -p aaupl/kQs1p3U -g pwrp -G b55,b66,skiftel -d /home/pwrp pwrp; then
    echo "-- user pwrp added"
  fi
  if [ ! -e /home/pwrp ]; then
    mkdir /home/pwrp
    /bin/cp /usr/pwrrt/cnf/user/.bashrc /home/pwrp
    /bin/cp /usr/pwrrt/cnf/user/.bash_profile /home/pwrp
    /bin/cp /usr/pwrrt/cnf/user/.rtt_start /home/pwrp
    chmod a+x /home/pwrp/.rtt_start
    /bin/cp /usr/pwrrt/cnf/user/.xtt_start /home/pwrp
    chmod a+x /home/pwrp/.xtt_start

    chown -R pwrp /home/pwrp
    chgrp -R pwrp /home/pwrp
  fi
fi

if getent passwd skiftel > /dev/null; then
  echo "-- user skiftel already exist"
else
  new_user=1

  if useradd -s /bin/bash -p aa6NzxS/aBgP6 -g skiftel -G pwrp -d /home/skiftel skiftel; then
    echo "-- user skiftel added"
  fi

  if [ ! -e /home/skiftel ]; then
    mkdir /home/skiftel
    cp /usr/pwrrt/cnf/user/.bashrc /home/skiftel
    cp /usr/pwrrt/cnf/user/.bash_profile /home/skiftel
    cp /usr/pwrrt/cnf/user/.rtt_start /home/skiftel
    chmod a+x /home/skiftel/.rtt_start
    cp /usr/pwrrt/cnf/user/.xtt_start /home/skiftel
    chmod a+x /home/skiftel/.xtt_start

    chown -R pwrp /home/skiftel
    chgrp -R pwrp /home/skiftel
  fi
fi

if getent passwd b55 > /dev/null; then
  new_user=1

  # Check if group audio exist
  if getent group audio > /dev/null; then
    groups="pwrp,audio"
  else
    groups="pwrp"
  fi

  if useradd -s /bin/bash -p aaQPClsglxJP6 -g b55 -G $groups -d /home/b55 b55; then
    echo "-- user b55 added"
  fi
  if [ ! -e /home/b55 ]; then
    mkdir /home/b55
    cp /usr/pwrrt/cnf/op/.bashrc /home/b55
    cp /usr/pwrrt/cnf/op/.bash_profile /home/b55
    cp /usr/pwrrt/cnf/op/.rtt_start /home/b55
    chmod a+x /home/b55/.rtt_start
    cp /usr/pwrrt/cnf/op/.xtt_start /home/b55
    chmod a+x /home/b55/.xtt_start

    chown -R b55 /home/b55
    chgrp -R pwrp /home/b55
    chmod g+rwx /home/b55
  fi
fi


#echo "Change owner of files to pwrp"
chown -R pwrp /usr/pwrrt
chgrp -R pwrp /usr/pwrrt

chmod u+s /usr/pwrrt/exe/rt_ini
chmod u+s /usr/pwrrt/exe/rt_rtt
#chmod u+s /usr/pwrrt/exe/rt_xtt
chmod u+s /usr/pwrrt/exe/rt_bck
chown root /usr/pwrrt/exe/rs_remote_alcm
chmod u+s /usr/pwrrt/exe/rs_remote_alcm
chown root /usr/pwrrt/exe/rt_prio
chmod u+s /usr/pwrrt/exe/rt_prio
chown root /usr/pwrrt/exe/rt_mozilla
chmod u+s /usr/pwrrt/exe/rt_mozilla

# Copy configuration files
new_cnf=0
if [ ! -e /etc/proview.cnf ]; then
  cp /usr/pwrrt/cnf/proview.cnf /etc
  new_cnf=1
fi
cp /usr/pwrrt/cnf/pwr /etc/init.d
chmod a+x /etc/init.d/pwr


# Add pwrp_profile to profile
if ! grep -q "/etc/pwrp_profile\b" /etc/profile; then
  cat >> /etc/profile <<-EOF
	if [ -e /etc/pwrp_profile ]; then
	  source /etc/pwrp_profile
	fi
EOF
fi

# Create startup link

if [ -e /etc/init.d/rc2.d ]; then 
  set +e
  checklink=`eval ls /etc/init.d/rc2.d/S90pwr 2>/dev/null`
  set -e
  if [ "$checklink" != "" ]; then
    rm /etc/init.d/rc2.d/S90pwr
  fi
  ln -s /etc/init.d/pwr /etc/init.d/rc2.d/S90pwr
elif [ -e /etc/rc2.d ]; then
  set +e
  checklink=`eval ls /etc/rc2.d/S90pwr 2>/dev/null`
  set -e
  if [ "$checklink" != "" ]; then
    rm /etc/rc2.d/S90pwr
  fi
  ln -s /etc/init.d/pwr /etc/rc2.d/S90pwr
fi

# Create project
new_project=0
if [ ! -e $proot ]; then
  new_project=1

  mkdir $proot
  mkdir $proot/common
  mkdir $proot/common/inc
  mkdir $proot/common/load
  mkdir $proot/common/log
  mkdir $proot/common/doc
  mkdir $proot/common/db
  mkdir $proot/common/web
  mkdir $proot/x86_linux
  mkdir $proot/x86_linux/exe
  mkdir $proot/x86_linux/lib
  mkdir $proot/x86_linux/obj
  mkdir $proot/x86_linux/lis

  chown -R pwrp $proot
  chgrp -R pwrp $proot
fi

if [ ! -e $aroot/db ]; then
  mkdir -p $aroot/db
  chown -R pwrp $aroot
fi

# Copy jar-files to web directory
if [ -e /etc/proview.cnf ]; then
  set +e
  web_dir=`eval cat /etc/proview.cnf | grep "\\bwebDirectory\\b" | awk '{print $2}'`
  set -e

  if [ -e "$web_dir" ]; then
    cp /usr/pwrrt/lib/pwr_rt_client.jar $web_dir
    chown pwrp $web_dir/pwr_rt_client.jar
    cp /usr/pwrrt/lib/pwr_jop.jar $web_dir
    chown pwrp $web_dir/pwr_jop.jar
    cp /usr/pwrrt/lib/pwr_jopc.jar $web_dir
    chown pwrp $web_dir/pwr_jopc.jar
  fi
fi

#
# Add proview web directories to Apache
#
if [ -e /etc/apache2/apache2.conf ]; then
  if ! egrep -q "Alias[ ]+/pwrp_web/" /etc/apache2/apache2.conf; then
    cat >> /etc/apache2/apache2.conf <<-EOF

	#
	# Proview alias pwrp_web, added by Proview installation
	#
	Alias /pwrp_web/ /pwrp/common/web/

	<Directory /pwrp/common/web>
	    Options Indexes MultiViews
	    AllowOverride None
	    Order allow,deny
	    Allow from all
	</Directory>
EOF
  fi


  if ! egrep -q "Alias[ ]+/pwr_doc/" /etc/apache/httpd.conf; then
    cat >> /etc/apache/httpd.conf <<-EOF

	#
	# Proview alias pwr_doc, added by Proview installation
	#
	Alias /pwr_doc/ /usr/pwrrt/doc/

	<Directory /usr/pwrrt/doc>
	    Options Indexes MultiViews
	    AllowOverride None
	    Order allow,deny
	    Allow from all
	</Directory>
EOF
  fi
fi

changes=0
if [ $new_user -eq 1 ]; then
  changes=1
elif [ $new_cnf -eq 1 ]; then
  changes=1
elif [ $new_project -eq 1 ]; then
  changes=1
fi

if [ $changes -ne 0 ]; then
  echo ""
  echo ""
  echo "***********************************************************"
  echo "  Don't forget to do this :"
  echo ""
fi

if [ $new_cnf -eq 1 ]; then
  echo "-- Enter QcomBusId in /etc/proview.cnf"
fi

#if [ $new_user -eq 1 ]; then
#  echo "-- Enter OpPlace object as argument to rt_xtt in /home/b55/.xtt_start"
#fi

if [ $new_project -eq 1 ]; then
  nodename=`eval uname -n`
  echo "-- Create file /home/pwrp/.rhosts"
  echo "-- Distribute project to $nodename"
fi

if [ $changes -ne 0 ]; then
  echo ""
  echo "***********************************************************"
  echo ""
fi


%preun

set -e

proot="/pwrp"
aroot="/usr/pwrp/adm"

# rpm doesn't support interactiveness
#echo ""
#echo -n "Do you wan't to remove project and users (y/n) [n] "
#read remove_all

remove_all="n"
if [ "$remove_all" = "y" ]; then
  
  # Check that any user that is to be removed isn't logged in
  set +e
  user_found=0
  currentusers=`eval users`
  userstr=""
  checkuser=`eval echo $currentusers | grep "\bb55\b"`
  if [ "$checkuser" != "" ]; then
    user_found=1
    userstr=$userstr" b55"
  fi
  checkuser=`eval echo $currentusers | grep "\bpwrp\b"`
  if [ "$checkuser" != "" ]; then
    user_found=1
    userstr=$userstr" pwrp"
  fi
  checkuser=`eval echo $currentusers | grep "\bskiftel\b"`
  if [ "$checkuser" != "" ]; then
    user_found=1
    userstr=$userstr" skiftel"
  fi

  if [ $user_found -eq 1 ]; then
    echo "-- Current users: $currentusers"
    echo "** Remove user will fail. Logout user $userstr first."
    exit -1;
  fi
  set -e

  echo "-- Remove user b55"
  if getent passwd b55 > /dev/null; then
    userdel b55
  fi
  if [ -e /home/b55 ]; then
    rm -r /home/b55
  fi

  echo "-- Remove user pwrp"
  if getent passwd pwrp > /dev/null; then
    userdel pwrp
  fi
  if [ -e /home/pwrp ]; then
    rm -r /home/pwrp
  fi

  echo "-- Remove user skiftel"
  if getent skiftel pwrp > /dev/null; then
    userdel skiftel
  fi
  if [ -e /home/skiftel ]; then
    rm -r /home/skiftel
  fi

  if getent group b55 > /dev/null; then
    groupdel b55
  fi

  if getent group pwrp > /dev/null; then
    groupdel pwrp
  fi

  if getent group skiftel > /dev/null; then
    groupdel skiftel
  fi

  echo "-- Remove project $proot"
  if [ -e $proot ]; then
    rm -r $proot
  fi

  if [ -e $aroot ]; then
    rm -r $aroot
  fi
fi

# Remove startup

if [ -e /etc/init.d/rc2.d ]; then
  checklink=`eval ls /etc/init.d/rc2.d/S90pwr 2>/dev/null`
  if [ "$checklink" != "" ]; then
    rm /etc/init.d/rc2.d/S90pwr
  fi
elif [ -e /etc/rc2.d ]; then
  checklink=`eval ls /etc/rc2.d/S90pwr 2>/dev/null`
  if [ "$checklink" != "" ]; then
    rm /etc/rc2.d/S90pwr
  fi
fi

# Remove jar-files on web directory
if [ -e /etc/proview.cnf ]; then
  web_dir=`eval cat /etc/proview.cnf | grep "\\bwebDirectory\\b" | awk '{print $2}'`

  if [ -e $web_dir/pwr_rt_client.jar ]; then
    rm $web_dir/pwr_rt_client.jar
  fi
  if [ -e $web_dir/pwr_jop.jar ]; then
    rm $web_dir/pwr_jop.jar
  fi
fi

#%postun

%changelog
* Fri Aug 27 2010 Claes Sjofors <claes.sjofors@proview.se> 4.7.1-1
  - Base release.
