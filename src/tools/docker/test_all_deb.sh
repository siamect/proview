#!/bin/bash

release="debian:10"
release_name="deb"
build_rpi=1
gitrepo="-b stable http://10.255.164.96/git/x5-7-2/pwr"
install_update="apt-get update"
install_git="apt-get install -y git make"
install_build="apt-get install -y libgtk2.0-dev doxygen gcc g++ make libasound2-dev \
	libdb5.3-dev libdb5.3++-dev openjdk-11-jdk libmariadb-dev \
	libsqlite3-dev libhdf5-openmpi-dev librabbitmq-dev libusb-1.0.0-dev librsvg2-dev \
	libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev"
install_rpi="apt-get install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf"
install_pwr="apt-get install -y libgtk2.0-0 libasound2 \
	libdb5.3 libdb5.3++ libsqlite3-0 librsvg2-2 g++  xterm libmariadb3 \
	librabbitmq4 libusb-1.0-0 libhdf5-openmpi-103 librabbitmq4 \
	libgstreamer1.0-0 libgstreamer-plugins-base1.0-0 openjdk-11-jdk \
	xterm xfonts-100dpi sudo procps"
install_pwrrt="apt-get install -y libgtk2.0-0 libasound2 \
	libdb5.3 libdb5.3++ libsqlite3-0 librsvg2-2 g++ xterm libmariadb3 \
	librabbitmq4 libusb-1.0-0 libhdf5-openmpi-103 \
	libgstreamer1.0-0 libgstreamer-plugins-base1.0-0 \
	xterm xfonts-100dpi sudo procps"
jdk_dir=/usr/lib/jvm/java-11-openjdk-amd64
ver="5.7.2-1"
sver="57"
arch="amd64"
pkg_pwr="pwr"$sver"_"$ver"_"$arch".deb"
pkg_pwrdemo="pwrdemo"$sver"_"$ver"_"$arch".deb"
pkg_pwrrt="pwrrt_"$ver"_"$arch".deb"
pkg_pwrsev="pwrsev_"$ver"_"$arch".deb"
pkg_pwrrpi="pwrrpi"$sver"_"$ver"_"$arch".deb"

docker image build -t pwr:v1 -f Dockerfile.pwr \
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
docker container rm tmp
docker image build -t pwrdemo:v1 -f Dockerfile.pwrdemo \
  --build-arg RELEASE=$release \
  --build-arg INSTALL_UPDATE="$install_update" \
  --build-arg INSTALL_PWR="$install_pwr" \
  --build-arg PKG_PWR=$pkg_pwr \
  --build-arg PKG_PWRDEMO=$pkg_pwrdemo \
  ./
docker run pwrdemo:v1
docker image build -t pwrtest01:v1 -f Dockerfile.pwrtest01 \
  --build-arg RELEASE=$release \
  --build-arg INSTALL_UPDATE="$install_update" \
  --build-arg INSTALL_PWRRT="$install_pwr" \
  --build-arg PKG_PWRRT=$pkg_pwrrt \
  ./
docker run pwrtest01:v1
