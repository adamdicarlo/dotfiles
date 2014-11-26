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

set -x GIT_EDITOR vim
set -x EDITOR mvim
set -x VISUAL mvim

set -x DOCKER_HOST "127.0.0.1:32000"
set -x NPM_PACKAGES "$HOME/.npm-packages"
set -x NODE_PATH $NPM_PACKAGES/lib/node_modules $NODE_PATH

function add_path
  if test -d $argv
    set PATH $argv $PATH
  end
end

add_path "$HOME/bin"
add_path "/usr/local/bin"
add_path "/usr/local/sbin"
add_path "$NPM_PACKAGES/bin"
add_path "/usr/local/opt/coreutils/libexec/gnubin"

# set -gx RBENV_ROOT /usr/local/var/rbenv
status --is-interactive; and . (rbenv init -|psub)

# Aliases
alias ... ../..
alias .... ../../..
alias ..... ../../../..
