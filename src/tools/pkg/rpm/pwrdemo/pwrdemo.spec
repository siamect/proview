###############################################################
# PACKAGE INFORMATION
#
# Notes:
#
# - rpm documentation
#   See www.rpm.org.
#


Name:		pwrdemo46
Summary: 	Proview/R demo project
Version:        4.6.0
Release: 	4
License:        GPL
BuildArch:      i386
Packager: 	claes.sjofors@proview.se
Group: 		Development/Tools/Other
Url:            http://www.proview.se

%description
For more information please see www.proview.se.

#%prep
#%setup
#%build

#%install

#%clean

%files
%defattr(-,root,root)
/usr

%post

set -e
# Added by build.sh :
# ver=""
# pwre_target=""
#
aroot="/usr/pwrp/adm"
export pwra_db=$aroot/db

chown -R pwrp:pwrp /usr/pwrp/pwrdemo%{ver}

# Register volume
if [ -e $aroot/db/pwr_volumelist.dat ]; then
  set +e
  ptst=`eval grep "'\bVolPwrDemo\b'" $aroot/db/pwr_volumelist.dat | grep "\bpwrdemo\b"`
  set -e
  if [ "$ptst" = "" ]; then
    echo "	 VolPwrDemo 0.254.254.200 pwrdemo" >> $aroot/db/pwr_volumelist.dat
  fi
fi

# Insert base in projectlist
pwrver=%{ver}
if [ -e $aroot/db/pwr_projectlist.dat ]; then
  set +e
  ptst=`eval grep "'\bpwrdemo%{ver}\b'" $aroot/db/pwr_projectlist.dat`
  set -e
  if [ "$ptst" = "" ]; then
    echo "pwrdemo%{ver} V${pwrver:0:1}.${pwrver:1:1} /usr/pwrp/pwrdemo%{ver} Demos-pwrdemo%{ver} \"\" " >> $aroot/db/pwr_projectlist.dat
  fi
fi

# Insert nodename in DirectoryVolume and create bootfile
nodename=`eval uname -n`
# nodename=${nodename%%.*}
initsh=/tmp/pwrdemo_init.sh
initpwrc=/tmp/pwrdemo_init.pwr_com

# Create a shellscript that attaches to projects and runs wb_cmd
cat > $initsh <<EOF
#!/bin/bash

source $aroot/db/pwr_setup.sh
source \$pwr_exe/pwrp_env.sh set project pwrdemo%{ver}

if [ ! -e "\$pwrp_db/directory.db" ]; then
  wb_cmd create volume/directory
  wb_cmd wb load /load=\"\$pwrp_db/directory.wb_dmp\"
fi
if [ ! -e "\$pwrp_db/volpwrdemo.db" ]; then
  wb_cmd wb load /load=\"\$pwrp_db/volpwrdemo.wb_dmp\"
fi

wb_cmd @$initpwrc
EOF

chmod a+x $initsh

# Create a wb_cmd script that inserts nodename and creates bootfile
cat > $initpwrc <<EOF
  string netname = "$nodename";
  string name;
  name = element( 1, ".", netname);
  set attr/name=Bus999-DemoNode/attr=NodeName/value="'name'"/noconf
  save
  create boot/node=DemoNode
  exit
EOF

# Execute shellscript
sudo -u pwrp $initsh

rm $initsh
rm $initpwrc

%preun

set -e
#echo "Here in prerm..."
 
if [ -e /usr/pwrp/pwrdemo%{ver} ]; then
 rm -r /usr/pwrp/pwrdemo%{ver}
fi


#%postun

%changelog
* Mon Nov 10 2008 Claes Sjofors <claes.sjofors@proview.se> 4.6.0-1
- Base release
 
