#! /bin/bash
#
# Proview   $Id: ge_javac.sh,v 1.4 2005-09-01 14:57:49 claes Exp $
# Copyright (C) 2005 SSAB Oxelösund AB.
#
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License as 
# published by the Free Software Foundation, either version 2 of 
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with the program, if not, write to the Free Software 
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#
#
function=$1
src_file=$2
systemname=$3


if [ $function == "java_web" ]; then
  jarfile="$pwrp_web/pwrp_"$systemname"_web.jar"

  if javac -d $pwrp_web -classpath $pwr_lib/pwr_rt.jar:$pwr_lib/pwr_jop.jar:$jarfile $pwrp_pop/$src_file
  then
    echo "-- $src_file compiled"
    if [ ! -e $jarfile ]; then
      cd $pwrp_web
      jar -cf $jarfile ${src_file%.java}.class
      rm ${src_file%.java}.class
    else
      cd $pwrp_web
      jar -uf $jarfile ${src_file%.java}.class
      rm ${src_file%.java}.class
    fi
    files=`ls $pwrp_web/${src_file%.java}\\$*.class 2>&-`
    if [ ! -z "$files" ]; then
      # echo "Insert in archive $jarfile"
      jar -uf $jarfile ${src_file%.java}\$*.class
      rm ${src_file%.java}\$*.class
    fi
    files=`ls $pwrp_pop/*.gif 2>&-`
    if [ ! -z "$files" ]; then
      cd $pwrp_pop
      jar -uf $jarfile *.gif
    fi
    files=`ls $pwrp_pop/*.jpg 2>&-`
    if [ ! -z "$files" ]; then
      cd $pwrp_pop
      jar -uf $jarfile *.jpg
    fi

  fi
fi

if [ $function == "java" ]; then
  jarfile="$pwrp_lib/pwrp_"$systemname".jar"
  jarfileweb="$pwrp_web/pwrp_"$systemname"_web.jar"

  if javac -d $pwrp_web -classpath $pwr_lib/pwr_rt.jar:$pwr_lib/pwr_jop.jar:$jarfile $pwrp_pop/$src_file
  then
    echo "-- $src_file compiled"
    if [ ! -e $jarfile ]; then
      cd $pwrp_web
      jar -cf $jarfile ${src_file%.java}.class
    else
      cd $pwrp_web
      jar -uf $jarfile ${src_file%.java}.class
    fi
    if [ ! -e $jarfileweb ]; then
      cd $pwrp_web
      jar -cf $jarfileweb ${src_file%.java}.class
    else
      cd $pwrp_web
      jar -uf $jarfileweb ${src_file%.java}.class
    fi
    rm ${src_file%.java}.class
    files=`ls $pwrp_web/${src_file%.java}\\$*.class 2>&-`
    if [ ! -z "$files" ]; then
      # echo "Insert in archive $jarfile"
      jar -uf $jarfile ${src_file%.java}\$*.class
      jar -uf $jarfileweb ${src_file%.java}\$*.class
      rm ${src_file%.java}\$*.class
    fi
    files=`ls $pwrp_pop/*.gif 2>&-`
    if [ ! -z "$files" ]; then
      cd $pwrp_pop
      jar -uf $jarfile *.gif
      jar -uf $jarfileweb *.gif
    fi
    files=`ls $pwrp_pop/*.jpg 2>&-`
    if [ ! -z "$files" ]; then
      cd $pwrp_pop
      jar -uf $jarfile *.jpg
      jar -uf $jarfileweb *.jpg
    fi

  fi
fi

if [ $function == "html" ]; then
  cp $pwrp_pop/$src_file $pwrp_web/
fi








