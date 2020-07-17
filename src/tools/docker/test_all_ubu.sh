#!/bin/bash

if [ "$1" == "" ]; then
  start=1
else
  start=$1
fi
if [ "$2" == "" ]; then
  end=3
else
  end=$2
fi

release="ubuntu:18.04"
build_rpi=1
gitrepo="-b stable http://192.168.0.104/git/x5-7-2/pwr/.git"
install_update="apt-get update"
install_git="apt-get install -y git make"
install_build="apt-get install -y libgtk2.0-dev doxygen gcc g++ make libasound2-dev \
	libdb5.3-dev libdb5.3++-dev openjdk-11-jdk libmariadb-dev \
	libsqlite3-dev libhdf5-openmpi-dev librabbitmq-dev libusb-1.0.0-dev librsvg2-dev \
	libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libpython-dev"
install_rpi="apt-get install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf"
install_pwr="apt-get install -y libgtk2.0-0 libasound2 \
	libdb5.3 libdb5.3++ libsqlite3-0 librsvg2-2 g++  xterm libmariadb3 \
	librabbitmq4 libusb-1.0-0 libhdf5-openmpi-100 librabbitmq4 \
	libgstreamer1.0-0 libgstreamer-plugins-base1.0-0 openjdk-11-jdk \
	xterm xfonts-100dpi sudo procps libpython-dev"
install_pwrrt="apt-get install -y libgtk2.0-0 libasound2 \
	libdb5.3 libdb5.3++ libsqlite3-0 librsvg2-2 g++ xterm libmariadb3 \
	librabbitmq4 libusb-1.0-0 libhdf5-openmpi-100 \
	libgstreamer1.0-0 libgstreamer-plugins-base1.0-0 \
	xterm xfonts-100dpi sudo procps python"
jdk_dir=/usr/lib/jvm/java-11-openjdk-amd64
ver="5.7.2-1"
sver="57"
arch="amd64"
pkg_pwr="pwr"$sver"_"$ver"_"$arch".deb"
pkg_pwrdemo="pwrdemo"$sver"_"$ver"_"$arch".deb"
pkg_pwrrt="pwrrt_"$ver"_"$arch".deb"
pkg_pwrsev="pwrsev_"$ver"_"$arch".deb"
pkg_pwrrpi="pwrrpi"$sver"_"$ver"_"$arch".deb"

# Build pwr container
if [ $start -le 1 ] && [ $end -ge 1 ]; then
  echo "** Building $release"
  docker image build -t pwr:v1 -f build/Dockerfile.build \
    --build-arg RELEASE=$release \
    --build-arg BUILD_RPI=$build_rpi \
    --build-arg GITREPO="$gitrepo" \
    --build-arg INSTALL_UPDATE="$install_update" \
    --build-arg INSTALL_GIT="$install_git" \
    --build-arg INSTALL_BUILD="$install_build" \
    --build-arg INSTALL_RPI="$install_rpi" \
    --build-arg JDK_DIR=$jdk_dir \
    ./
  docker container create --name tmp pwr:v1
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwr ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrdemo ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrrt ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrsev ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrrpi ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest01/bld/common/load/pwrp_pkg_pwrtest01a_0001.tgz ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest01/bld/common/load/pwrp_pkg_pwrtest01b_0001.tgz ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest01/bld/common/load/pwrp_pkg_pwrtest01c_0001.tgz ./
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest02/bld/common/load/pwrtest02.tar.gz ./
  docker container rm tmp
fi

# Demo container
if [ $start -le 2 ] && [ $end -ge 2 ]; then
  docker image build -t pwrdemo:v1 -f pwrdemo/Dockerfile.pwrdemo \
    --build-arg RELEASE=$release \
    --build-arg INSTALL_UPDATE="$install_update" \
    --build-arg INSTALL_PWR="$install_pwr" \
    --build-arg PKG_PWR=$pkg_pwr \
    --build-arg PKG_PWRDEMO=$pkg_pwrdemo \
    ./
  docker run --name pwrdm pwrdemo:v1
  docker container cp pwrdm:/usr/pwrp/pwrdemo$sver/bld/common/tmp/pwrdemo_status.tlog ./log/
fi

# Runtime container pwrtest01c
if [ $start -le 3 ] && [ $end -ge 3 ]; then
  docker image build -t pwrtest01c:v1 -f pwrtest01c/Dockerfile.pwrtest01c \
    --build-arg RELEASE=$release \
    --build-arg INSTALL_UPDATE="$install_update" \
    --build-arg INSTALL_PWRRT="$install_pwrrt" \
    --build-arg PKG_PWRRT=$pkg_pwrrt \
    ./
  docker run --name pwrtc pwrtest01c:v1
  docker container cp pwrtc:/pwrp/common/log/plc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/aproc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/cdh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/errh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/gdh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/mh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/mhappl.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/plc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/qcom.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/qmon.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/qmonc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/time.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/nettime.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/ccm.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/xttscript.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/pwrrt.tlog ./log/
fi

# Pwrtest02 container
if [ $start -le 4 ] && [ $end -ge 4 ]; then
  docker image build -t pwrtest02:v1 -f pwrtest02/Dockerfile.pwrtest02 \
    --build-arg RELEASE=$release \
    --build-arg INSTALL_UPDATE="$install_update" \
    --build-arg INSTALL_PWR="$install_pwr" \
    --build-arg PKG_PWR=$pkg_pwr \
    ./
  docker run --name pwrt2 pwrtest02:v1
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/pwrtest02_classvolume.tlog ./log/
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/pwrtest02_rootvolume.tlog ./log/
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/ldh.tlog ./log/
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/pwrwb.tlog ./log/
fi

