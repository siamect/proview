# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files for examples
source /etc/pwrp_profile

# If running interactively, then:
if [ "$PS1" ]; then

    # enable color support of ls and also add handy aliases

    eval `dircolors -b`
    #alias ls='ls --color=auto'
    #alias ll='ls -l'
    #alias la='ls -A'
    #alias l='ls -CF'
    #alias dir='ls --color=auto --format=vertical'
    #alias vdir='ls --color=auto --format=long'

    # set a fancy prompt

    PS1='\u@\h:\w\$ '

    # If this is an xterm set the title to user@host:dir
    #case $TERM in
    #xterm*)
    #    PROMPT_COMMAND='echo -ne "\033]0;${USER}@${HOSTNAME}: ${PWD}\007"'
    #    ;;
    #*)
    #    ;;
    #esac

fi
