# Linux?
if [ -d /home ]; then
  export XDG_CONFIG_HOME=$HOME/.config

  # LIVE THE DREAM: Bind Caps-Lock to be Esc when tapped, Ctrl when held.
  setxkbmap -option 'caps:ctrl_modifier'
  xcape -e 'Caps_Lock=Escape'
fi

export EDITOR=${EDITOR:-nvim}
export PATH="$HOME/bin:/usr/local/bin:/usr/local/sbin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:$HOME/.yarn/bin:./node_modules/.bin"

if [ ! -z "$GOPATH" ]; then
  export PATH="$PATH:$GOPATH/bin"
fi

export MANPATH="/usr/local/man:$MANPATH"

# nvm
export NODEJS_ORG_MIRROR=https://nodejs.org/dist
export NVM_DIR="${XDG_CONFIG_HOME}/nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
