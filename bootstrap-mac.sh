#!/bin/bash

if ! which brew &>/dev/null; then
  echo "Installing homebrew"
  /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

#
# Install a package if not already installed.
#
# Avoids delays and noisy brew warnings.
#
function maybe_install {
  local package=$1
  local bin=${2:-${package}}
  if ! command -v $bin &>/dev/null; then
    echo "Installing $package"
    brew install $package
  fi
}

#
# Install a package that overrides a system default (a dupe), if not already installed.
#
function maybe_install_dupe {
  local package=$1
  local bin=${2:-${package}}
  shift 2
  if [[ "$(which $bin)" != "/usr/local/bin/$bin" ]]; then
    echo "Installing $package"
    brew install $package $@
  fi
}

#
# Install a cask if not already installed.
#
function maybe_install_cask {
  local cask=$1
  local name=$2
  if [[ ! -d "$HOME/Applications/$name" && ! -d "/Applications/$name" ]]; then
    echo "Installing $name"
    brew cask install $cask
  fi
}

#
# Xcode build tools
#
which BuildStrings &>/dev/null || xcode-select --install

#
# Install-via-Homebrew tools
#
maybe_install the_silver_searcher ag
maybe_install cmake
maybe_install coreutils gln
maybe_install diff-so-fancy
maybe_install htop-osx htop
maybe_install homebrew/gui/meld meld
maybe_install imagemagick convert
maybe_install most
maybe_install mtr
maybe_install python3
maybe_install ssh-copy-id
maybe_install watch
maybe_install wget

maybe_install_dupe git
maybe_install_dupe findutils find --with-default-names

[ -d ~/.nvm ] || maybe_install nvm
[ -f `brew --prefix`/etc/profile.d/z.sh ] || maybe_install z

if ! git info &>/dev/null; then
  echo "Installing git-extras..."
  brew install git-extras
fi

#
# Apps
#
maybe_install_cask alfred 'Alfred 3.app'
maybe_install_cask amphetamine Amphetamine.app
maybe_install_cask dropbox Dropbox.app
maybe_install_cask evernote Evernote.app
maybe_install_cask flux Flux.app
maybe_install_cask iterm2 iTerm.app
maybe_install_cask karabiner Karabiner.app
maybe_install_cask seil Seil.app
maybe_install_cask vlc VLC.app

[ -d "$HOME/Library/PreferencePanes/AppTrap.prefPane" ] || maybe_install_cask apptrap

#
# Neovim!
#
maybe_install_cask vimr VimR.app

echo "Installing/updating Plug.vim"
curl -SsfLo ~/.config/nvim/autoload/plug.vim --create-dirs \
    https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim

if ! pip3 show neovim &>/dev/null; then
  echo "Installing Python neovim integration"
  pip3 install --upgrade pip
  pip3 install neovim
fi

if [ ! -d ~/.oh-my-zsh ]; then
  echo "Installing oh-my-zsh..."
  sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"
fi

#
# Syntax highlighting (fish-like) for zsh
#
if [ ! -d ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting ]; then
  echo "Installing zsh-syntax-highlighting..."
  git clone -q https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting
fi

if [ ! -f ~/.oh-my-zsh/themes/dracula.zsh-theme ]; then
  echo "Installing zsh Dracula theme..."
  [ -d ~/opt/dracula-theme ] || git clone -q https://github.com/dracula/zsh.git ~/opt/dracula-theme/zsh
  ln -s ~/opt/dracula-theme/zsh/dracula.zsh-theme ~/.oh-my-zsh/themes/dracula.zsh-theme
fi

if [ ! -f ~/opt/dracula-theme/iterm/Dracula.itermcolors ]; then
  echo "Installing iTerm2 Dracula theme"
  git clone -q https://github.com/dracula/iterm.git ~/opt/dracula-theme/iterm
fi

if [ ! -d ~/.homesick/repos/homeshick ]; then
  echo "Installing homeshick..."
  git clone -q git://github.com/andsens/homeshick.git $HOME/.homesick/repos/homeshick
  [ -d ~/.homesick/repos/dotfiles ] || git clone git@github.com:adamdicarlo/dotfiles.git $HOME/.homesick/repos/dotfiles
  [ -d ~/.homesick/repos/neovim ]   || git clone git@github.com:adamdicarlo/castle-neovim.git $HOME/.homesick/repos/neovim
  $HOME/.homesick/repos/homeshick/bin/homeshick link dotfiles
  $HOME/.homesick/repos/homeshick/bin/homeshick link neovim
fi
