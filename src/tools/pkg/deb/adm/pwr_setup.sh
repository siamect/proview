if [ "$pwra_db" == "" ]; then
  source /etc/pwrp_profile
fi

pwrp()
{
	source $pwra_db/pwra_env.sh $@
}

alias sdf="source $pwra_db/pwra_env.sh set project"
alias pwrc="wb_cmd"
alias pwrs="wb_start.sh pwrp pwrp"
alias pwra="wb -p pwrp pwrp"
