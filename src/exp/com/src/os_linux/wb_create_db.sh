#
#      Create the database directory and copy en empty database to the
#      directory.
#

db=$1

source pwrp_env.sh copy template $db noconfirm

if [ $? != 0 ]; then
  return 1;
fi
