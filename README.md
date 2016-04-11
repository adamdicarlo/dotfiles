# dotfiles

My shell configuration for developing on OS X.

I'm using [Homeshick](https://github.com/andsens/homeshick) to manage these.

My Homeshick _castles_:

* [adamdicarlo/dotfiles.git](https://github.com/adamdicarlo/dotfiles)
* [adamdicarlo/castle-neovim.git](https://github.com/adamdicarlo/castle-neovim)

Flavor (tools I use):

* Neovim and Neovim.app
* zsh with oh-my-zsh

## Installation

1. Install [homebrew](http://brew.sh/)
1. Install [Homeshick](https://github.com/andsens/homeshick)
1. Clone castles (including this one)
1. Run `homeshick link dotfiles`
1. Run `homeshick link castle-neovim`
1. Run `~/.homesick/castles/dotfiles/bootstrap-mac.sh`
   - NB: This will install Neovim.app (TODO: Move Neovim install to castle-neovim!)
