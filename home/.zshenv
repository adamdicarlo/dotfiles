export XDG_CONFIG_HOME=$HOME/.config
export ZPLUG_HOME=${XDG_CONFIG_HOME}/zplug
export EDITOR=${EDITOR:-nvim}
path=(~/bin $path ~/.yarn/bin ./node_modules/.bin)

if [ -n "$DESKTOP_SESSION" ]; then
  eval $(gnome-keyring-daemon --start)
  export SSH_AUTH_SOCK
fi

if [ ! -z "$GOPATH" ]; then
  path=($path $GOPATH/bin)
fi

export MANPATH="/usr/local/man:$MANPATH"

# nvm
export NODEJS_ORG_MIRROR=https://nodejs.org/dist
export NVM_DIR="${XDG_CONFIG_HOME}/nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
