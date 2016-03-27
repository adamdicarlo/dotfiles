#!/bin/bash

# todo: Install homebrew automatically?
if ! which brew &>/dev/null; then
  echo "Please install homebrew first."
  exit 1
fi

brew install \
  the_silver_searcher \
  cmake \
  coreutils \
  cowsay \
  findutils \
  node \
  git \
  git-extras \
  htop-osx \
  meld \
  most \
  mtr \
  rbenv \
  sl \
  ssh-copy-id \
  watch \
  wget \
  z

brew install macvim --override-system-vim

# get .npmrc in place before installing node packages.
cd ~
if [ ! -d ./dotfiles ]; then
  git clone git@github.com:adamdicarlo/dotfiles.git
fi
cd ./dotfiles
./deploy.sh

# Install NeoBundle and vim plugins
if [ ! -d ./.vim/bundle/neobundle.vim ]; then
  curl https://raw.githubusercontent.com/Shougo/neobundle.vim/master/bin/install.sh | sh
fi
