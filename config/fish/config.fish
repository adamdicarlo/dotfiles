# Path to your oh-my-fish.
set fish_path $HOME/bin/tools/oh-my-fish
set fish_custom $HOME/dotfiles/config/fish/oh-my-fish-custom

# Theme
set fish_theme robbyrussell

# Which plugins would you like to load? (plugins can be found in ~/.oh-my-fish/plugins/*)
# Custom plugins may be added to ~/.oh-my-fish/custom/plugins/
# Example format: set fish_plugins autojump bundler

set -gx Z_SCRIPT_PATH $HOME/bin/tools/z/z.sh

set fish_plugins brew gi node rake vi-mode z

# Load oh-my-fish configuration.
. $fish_path/oh-my-fish.fish

. $HOME/.config/fish/private.fish

set -gx GIT_EDITOR vim
set -gx EDITOR mvim
set -gx VISUAL mvim

set -gx DOCKER_HOST "127.0.0.1:32000"
set -gx NPM_PACKAGES "$HOME/.npm-packages"
set -gx GOPATH "$HOME/go"

set -gx VIM_APP_DIR /usr/local/Cellar/macvim/7.4-73_1

set -gx PATH /usr/local/opt/coreutils/libexec/gnubin \
  $NPM_PACKAGES/bin \
  $HOME/bin \
  /usr/local/bin \
  /usr/local/sbin \
  /bin /sbin /usr/bin /usr/sbin \
  /usr/local/opt/go/libexec/bin

# set -gx RBENV_ROOT /usr/local/var/rbenv
status --is-interactive; and . (rbenv init -|psub)

ulimit -n 2560

# Aliases
alias be 'bundle exec'
alias ... ../..
alias .... ../../..
alias ..... ../../../..
