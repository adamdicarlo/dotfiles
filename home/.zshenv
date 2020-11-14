# ssh
export SSH_KEY_PATH="~/.ssh/rsa_id"
if [[ -n $SSH_CONNECTION ]]; then
  export EDITOR='vim'
else
  export EDITOR='nvim'
fi

# Make sure ssh-ident (which I have symlinked as ssh, rsync, scp in ~/bin) won't invoke itself when
# trying to delegate to actual ssh.
export BINARY_SSH=/usr/bin/ssh

export GPG_TTY=$(tty)
export LESS=-RFX
export PAGER=less
export TERM=${TERM:-xterm-256color}

export XDG_CONFIG_HOME=$HOME/.config
export ZPLUG_HOME=${XDG_CONFIG_HOME}/zplug
export EDITOR=${EDITOR:-nvim}
path=(~/bin $path ~/.yarn/bin ./node_modules/.bin)

if [ -n "$DESKTOP_SESSION" ]; then
  eval $(gnome-keyring-daemon --start)
  export SSH_AUTH_SOCK
fi

export MAKEFLAGS="-j$(expr $(nproc) \+ 1)"

if [ ! -z "$GOPATH" ]; then
  path=($path $GOPATH/bin)
fi

export MANPATH="/usr/local/man:$MANPATH"

# nvm
export NODEJS_ORG_MIRROR=https://nodejs.org/dist
export NVM_DIR="${XDG_CONFIG_HOME}/nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
