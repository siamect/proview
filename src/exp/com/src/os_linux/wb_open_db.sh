#
# Open a new terminal and start the navigator
# This procedure is started from the navigator 'Open db' method for
# DbConfig and DbTrace objects.
#
database=$1
username=$2
password=$3
volume=$4
title=$5

if [ -z $database ]; then
  echo "Database id is missing"
  return
fi

source pwrp_env.sh setdb $database

if [ -z $volume ]; then
  echo "-- Opening database '$database'"
  wb $username $password
else
  echo "-- Opening database '$database' volume '$volume'"
  wb $username $password $volume
fi
