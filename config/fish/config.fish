# Path to your oh-my-fish.
set fish_path $HOME/bin/tools/oh-my-fish
set fish_custom $HOME/dotfiles/config/fish/oh-my-fish-custom

# Load oh-my-fish configuration.
. $fish_path/oh-my-fish.fish

# Theme "technopagan"
Plugin "balias"
Plugin "brew"
Plugin "gi"
Plugin "ssh"
Plugin "theme"
Plugin "vi-mode"
Plugin "z"
# Theme "bobthefish"
# Theme "dangerous"
# Theme "scorphish"
# Theme "zish"
# Theme "yimmy"
# Theme "bira"
# Theme "red-snapper"
# Theme "eclm"
# Theme "agnoster"
# Theme "will"
# Theme "trout"
# Theme "uggedal"
# Theme "toaster"
# Theme "taktoa"
# Theme "simplivi"
# Theme "ocean"
# Theme "syl20bnr"
# Theme "perryh"
# Theme "l"
# Theme "nai"
# Theme "numist"
# Theme "integral"
# Theme "gitstatus"
# Theme "fox"
# Theme "fisk"
# Theme "robbyrussell"

. $HOME/.config/fish/private.fish

set -x GIT_EDITOR vim
set -x EDITOR vim
set -x VISUAL mvim

set -x GOPATH "$HOME/go"

eval (boot2docker shellinit | tr \n \;)

set -x VIM_APP_DIR /usr/local/Cellar/macvim/7.4-73_1

set -x PATH /usr/local/opt/coreutils/libexec/gnubin \
  $NPM_PACKAGES/bin \
  $HOME/bin \
  /usr/local/bin \
  /usr/local/sbin \
  /bin /sbin /usr/bin /usr/sbin \
  /usr/local/opt/go/libexec/bin

# set -x RBENV_ROOT /usr/local/var/rbenv
status --is-interactive; and . (rbenv init -|psub)

ulimit -n 2560

# Aliases
alias ... ../..
alias .... ../../..
alias ..... ../../../..
balias be 'bundle exec'

balias ga 'git add'
balias gap 'git add -p'
balias gco 'git co'
balias gcop 'git co -p'
balias gd 'git diff'
balias gds 'git diff --staged'
balias gl 'git log --graph --pretty'
balias gs 'git status'
balias la 'ls -lah'

# node version manager
set -x NVM_DIR "$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ]; and bass source "$NVM_DIR/nvm.sh"

function nvm
  bass source $NVM_DIR/nvm.sh ';' nvm $argv
end
