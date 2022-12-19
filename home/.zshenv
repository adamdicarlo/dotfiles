if [ -n "$DESKTOP_SESSION" ]; then
  export TERM="${TERM:-xterm-256color}"
  if [ -f /usr/lib/ssh/gnome-ssh-askpass3 ]; then
    export SUDO_ASKPASS=/usr/lib/ssh/gnome-ssh-askpass3
  fi
else
  export TERM=xterm-256color
fi

# ssh
export SSH_KEY_PATH="~/.ssh/rsa_id"

if which nvim >/dev/null; then
  export EDITOR='nvim'
elif which vim >/dev/null; then
  export EDITOR='vim'
elif which vi >/dev/null; then
  export EDITOR='vi'
else
  echo "WARNING: Can't find any Vims\! EDITOR=${EDITOR}"
fi

# Make sure ssh-ident (which I have symlinked as ssh, rsync, scp in ~/bin) won't invoke itself when
# trying to delegate to actual ssh.
export BINARY_SSH=/usr/bin/ssh

export GPG_TTY=$(tty)
export LESS=-RFX
export PAGER=less

export XDG_CONFIG_HOME=$HOME/.config
export ZPLUG_HOME=${XDG_CONFIG_HOME}/zplug
export EDITOR=${EDITOR:-nvim}
path=(~/.local/bin $path)

export MAKEFLAGS="-j$(expr $(nproc) \+ 1)"

if [ ! -z "$GOPATH" ]; then
  path=($path $GOPATH/bin)
fi

if which man >/dev/null; then
  export MANPATH="/usr/local/man:$MANPATH"
fi
