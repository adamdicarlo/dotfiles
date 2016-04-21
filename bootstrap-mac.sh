#!/bin/bash

# todo: Install homebrew automatically?
if ! which brew &>/dev/null; then
  echo "Please install homebrew first."
  exit 1
fi

#
# Check whether package is installed before installing to avoid noisy brew warnings & errors.
#
function maybe_install {
  local package=$1
  local bin=${2:-${package}}
  if ! which $bin &>/dev/null; then
    echo "Installing $package"
    brew install $package
  fi
}

#
# Check whether cask is installed before installing.
#
function maybe_install_cask {
  local cask=$1
  local name=$2
  if [ ! -d ~/Applications/$name ]; then
    echo "Installing $name"
    brew install Caskroom/cask/$cask
  fi
}
#
# Xcode build tools
#
which BuildStrings &>/dev/null || xcode-select --install

#
# Install-via-Homebrew tools
#
echo "Running brew update..."
brew update
maybe_install the_silver_searcher ag
maybe_install cmake
maybe_install coreutils gls
maybe_install findutils gfind
maybe_install htop-osx htop
maybe_install homebrew/gui/meld meld
maybe_install most
maybe_install mtr
maybe_install rbenv
maybe_install ssh-copy-id
maybe_install watch
maybe_install wget
[ -f `brew --prefix`/etc/profile.d/z.sh ] || maybe_install z

if [[ "$(which git)" != "/usr/local/bin/git" ]]; then
  echo "Installing git..."
  brew install git
fi

if ! git info &>/dev/null; then
  echo "Installing git-extras..."
  brew install git-extras
fi

#
# Apps
#
maybe_install_cask flux Flux.app

#
# Neovim!
#
if ! which nvim &>/dev/null; then
  echo "Installing neovim..."
  brew tap neovim/homebrew-neovim
  brew install --HEAD neovim
fi

if [ -d /Applications/Neovim.app ]; then
  echo "Neovim.app already installed"
else
  echo "Installing Neovim.app..."
  brew tap neovim/neovim
  brew tap rogual/neovim-dot-app
  brew install --HEAD neovim-dot-app && brew linkapps neovim-dot-app
fi

echo "Installing/updating Plug.vim..."
curl -SsfLo ~/.config/nvim/autoload/plug.vim --create-dirs \
    https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim

#
# nvm - node version manager
#
if [ ! -d ~/.nvm ]; then
  echo "Installing nvm..."
  git clone -q https://github.com/creationix/nvm.git ~/.nvm && cd ~/.nvm && git checkout `git describe --abbrev=0 --tags`
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
  echo "Installing zsh dracula theme..."
  [ -d ~/opt/dracula-theme ] || git clone -q https://github.com/zenorocha/dracula-theme.git ~/opt/dracula-theme
  ln -s ~/opt/dracula-theme/zsh/dracula.zsh-theme ~/.oh-my-zsh/themes/dracula.zsh-theme
fi

if [ ! -d ~/.homesick/repos/homeshick ]; then
  echo "Installing homeshick..."
  git clone -q git://github.com/andsens/homeshick.git $HOME/.homesick/repos/homeshick
  [ -d ~/.homesick/repos/dotfiles ] || git clone git@github.com:adamdicarlo/dotfiles.git $HOME/.homesick/repos/dotfiles
  [ -d ~/.homesick/repos/neovim ]   || git clone git@github.com:adamdicarlo/castle-neovim.git $HOME/.homesick/repos/neovim
  $HOME/.homesick/repos/homeshick/bin/homeshick link dotfiles
  $HOME/.homesick/repos/homeshick/bin/homeshick link neovim
fi
