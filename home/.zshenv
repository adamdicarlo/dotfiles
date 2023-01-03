# Nothing in here is zsh specific, so rename this to .profile to work with other shells?

if [ -n "$DESKTOP_SESSION" ]; then
  export AMD_VULKAN_ICD=RADV
  export BROWSER=firefox-developer-edition
  export GTK2_RC_FILES="$HOME/.gtkrc-2.0"
  export MAIL=thunderbird
  export XDG_SCREENSHOTS_DIR=$HOME/Sync/Screenshots

  export TERM=${TERM:-xterm-256color}

  if [ -f /usr/lib/ssh/gnome-ssh-askpass3 ]; then
    export SUDO_ASKPASS=/usr/lib/ssh/gnome-ssh-askpass3
  else
    echo "WARNING: Can't find gnome-ssh-askpass3"
  fi

  if ! pgrep -f $(which gnome-keyring-daemon) >/dev/null; then
    eval $(gnome-keyring-daemon --start)
    export SSH_AUTH_SOCK
  fi
else
  export TERM=xterm-256color
fi

if which nvim >/dev/null; then
  export EDITOR='nvim'
elif which vim >/dev/null; then
  export EDITOR='vim'
elif which vi >/dev/null; then
  export EDITOR='vi'
else
  echo "WARNING: Can't find nvim, vim, or even vi\! EDITOR=${EDITOR}"
fi

export BINARY_SSH=/usr/bin/ssh
export GPG_TTY=$(tty)
export MAKEFLAGS="-j$(expr $(nproc) \+ 1)"
export MANPAGER="sh -c 'col -bx | bat -l man -p'"
export MANPATH="/usr/local/man:$MANPATH"
export PAGER="bat"
export SSH_KEY_PATH="~/.ssh/ed25519.pub"
export VISUAL=${VISUAL:-nvim}
export XDG_CONFIG_HOME=$HOME/.config
export ZPLUG_HOME=${XDG_CONFIG_HOME}/zplug
path=(~/bin ~/.local/bin $path)


# Golang
if [ ! -z "$GOPATH" ]; then
  path=($path $GOPATH/bin)
fi

# Fast Node.js version manager
which fnm >/dev/null && source <(fnm env)

# Ruby
which rbenv >/dev/null && eval "$(rbenv init - zsh)"
