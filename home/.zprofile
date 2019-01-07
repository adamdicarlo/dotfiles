export EDITOR=${EDITOR:-nvim}
export PATH="$HOME/bin:/usr/local/bin:/usr/local/sbin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:$HOME/.yarn/bin:./node_modules/.bin:$GOPATH/bin"
export MANPATH="/usr/local/man:$MANPATH"

# nvm
export NODEJS_ORG_MIRROR=https://nodejs.org/dist
export NVM_DIR="$HOME/.nvm"
source "/usr/local/opt/nvm/nvm.sh"

# brew --prefix reports a bad path - different version than that installed. waaatttt
# source "$(brew --prefix nvm)/nvm.sh"
