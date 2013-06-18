#!/bin/bash
#
appname=PwrXtt
# 

echo "Executing build.sh..." $PWRE_CONF_ANDROID
if [ ! "$PWRE_CONF_ANDROID" = "1" ]; then
  echo "Android not installed"
  exit
fi

if [ $1 = "copy" ]; then
  if [ ! -e $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname ]; then
    android create project \
      --target 1 \
      --name $appname \
      --path $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname \
      --activity MainActivity \
      --package jpwr.pwrxtt

    rm $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname/src/jpwr/pwrxtt/MainActivity.java
    rm $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname/AndroidManifest.xml
    rm -r $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname/res
  fi
fi

if [ $1 = "copy" ]; then
  source $pwre_bin/pwre_function
  pwre build aapp manifest src
  pwre build aapp pwrxtt src
  pwre build aapp rt src
  pwre build aapp jopg src
  pwre build aapp app src
  pwre build aapp res src
  pwre build aapp assets src
fi

if [ $1 = "exe" ]; then
  android update project \
    --name $appname \
    --target 1 \
    --path $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname

  cp /usr/local/android-sdk-linux/extras/android/support/v4/android-support-v4.jar \
      $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname/libs/.

  cd $pwre_broot/$pwre_os/$pwre_hw/bld/aapp/$appname
  if [ -z "$pwre_keystore" ]; then
    echo "pwre_keystore not defined, build debug application"
    ant release
  else
    echo "Keystore used: $pwre_keystore"
    ant release -Dkey.store=$pwre_keystore -Dkey.alias=pwrkey
  fi
fi
