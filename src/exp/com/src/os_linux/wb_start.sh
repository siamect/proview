#
# Start the navigator
#

username=$1
password=$2
database=$3
volume=$4 

if [ -z "$database" ]; then
  source pwrp_env.sh setdb
else
  source pwrp_env.sh setdb $database
fi

wb $username $password $volume
