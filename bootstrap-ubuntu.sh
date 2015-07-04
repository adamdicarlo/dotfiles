#!/bin/bash

sudo apt-get install \
  build-essential \
  cmake \
  python-dev \
  curl \
  git \
  meld \
  vim-gtk \
  yakuake

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

# Build and install xcape, needed for mapping CapsLock to L_Ctrl when held, Esc when tapped
# (see .profile)
sudo apt-get install pkg-config libx11-dev libxtst-dev libxi-dev
rm -rf /tmp/xcape 2>/dev/null
mkdir /tmp/xcape
cd /tmp/xcape
git clone https://github.com/alols/xcape.git .
make
sudo make install
# no idea why it installs everything but bin into /usr/local...
sudo mv /usr/bin/xcape /usr/local/bin
