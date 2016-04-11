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
  if which $bin &>/dev/null; then
    echo "$package already installed"
  else
    echo "Installing $package"
    brew install $package
  fi
}

#
# Xcode build tools
#
xcode-select --install

#
# Install-via-Homebrew tools
#
echo "Running brew update..."
brew update
echo "Installing packages from homebrew..."
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
maybe_install z _z

if [[ "$(which git)" == "/usr/local/bin/git" ]]; then
  echo "Non-system git already installed"
else
  echo "Installing git..."
  brew install git
fi

if git info &>/dev/null; then
  echo "git-extras already installed"
else
  echo "Installing git-extras..."
  brew install git-extras
fi

#
# Neovim!
#
if [ -d /Applications/Neovim.app ]; then
  echo "Neovim.app already installed"
else
  echo "Installing Neovim.app..."
  brew tap neovim/neovim
  brew tap rogual/neovim-dot-app
  brew install neovim-dot-app
  brew linkapps neovim-dot-app
fi

#
# nvm - node version manager
#
if [ -d ~/.nvm ]; then
  echo "nvm already installed"
else
  echo "Installing nvm..."
  git clone -q https://github.com/creationix/nvm.git ~/.nvm && cd ~/.nvm && git checkout `git describe --abbrev=0 --tags`
fi

#
# Syntax highlighting (fish-like) for zsh
#
if [ -d ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting ]; then
  echo "zsh-syntax-highlighting already installed"
else
  echo "Installing zsh-syntax-highlighting..."
  git clone -q https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting
fi

if [ -f ~/.oh-my-zsh/themes/dracula.zsh-theme ]; then
  echo "zsh dracula-theme already installed"
else
  echo "Installing zsh dracula theme..."
  git clone -q https://github.com/zenorocha/dracula-theme.git ~/opt/dracula-theme
  ln -s ~/opt/dracula-theme/zsh/dracula.zsh-theme ~/.oh-my-zsh/themes/dracula.zsh-theme
fi
