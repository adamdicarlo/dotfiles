# Path to your oh-my-fish.
set fish_path $HOME/bin/tools/oh-my-fish

# Theme
set fish_theme robbyrussell

# Which plugins would you like to load? (plugins can be found in ~/.oh-my-fish/plugins/*)
# Custom plugins may be added to ~/.oh-my-fish/custom/plugins/
# Example format: set fish_plugins autojump bundler

set -gx Z_SCRIPT_PATH $HOME/bin/tools/z/z.sh

set fish_plugins brew git node rake rbenv vi-mode z


# Path to your custom folder (default path is $FISH/custom)
#set fish_custom $HOME/dotfiles/oh-my-fish

# Load oh-my-fish configuration.
. $fish_path/oh-my-fish.fish
