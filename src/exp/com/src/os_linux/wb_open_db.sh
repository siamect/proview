#
# Open a new terminal and start the navigator
# This procedure is started from the navigator 'Open db' method for
# DbConfig and DbTrace objects.
#
username=$1
password=$2
database=$3
title=$4

set -o xtrace
if [ -z $database ]; then
  echo "Database is missing"
  return
fi

echo "-- Opening volume '$database'"
wb $username $password $database
