#!/bin/bash

sudo apt-get install -y \
  build-essential \
  cmake \
  python-dev \
  curl \
  git \
  libnotify-bin \
  meld \
  most \
  silversearcher-ag \
  vim-gtk \
  yakuake

# get .npmrc in place before installing node packages.
if [ ! -d $HOME/dotfiles ]; then
  echo "Fetching dotfiles"
  git clone https://github.com/adamdicarlo/dotfiles.git $HOME/dotfiles >/dev/null
else
  echo "dotfiles present"
fi

cd $HOME/dotfiles
./deploy.sh

# Install latest node 0.10
if ! which node &>/dev/null; then
  LATEST=`curl https://semver.io/node/resolve/0.10.x`
  echo "Installing node-$LATEST"
  wget https://nodejs.org/dist/v$LATEST/node-v$LATEST-linux-x64.tar.gz -O /tmp/nodejs.tar.gz
  sudo tar --xform 's/^node[^\/]*//' -C /usr/local -xf /tmp/nodejs.tar.gz
else
  echo "node present (`node --version`)"
fi

# Install NeoBundle and vim plugins
if [ ! -d $HOME/.vim/bundle/neobundle.vim ]; then
  echo "Installing NeoBundle"
  curl https://raw.githubusercontent.com/Shougo/neobundle.vim/master/bin/install.sh | sh
else
  echo "NeoBundle present"
fi

# Build and install xcape, needed for mapping CapsLock to be both Ctrl and Esc (see .bash_aliases)
if ! which xcape &>/dev/null; then
  echo "Installing xcape"
  sudo apt-get install pkg-config libx11-dev libxtst-dev libxi-dev
  rm -rf /tmp/xcape 2>/dev/null
  mkdir /tmp/xcape
  cd /tmp/xcape
  git clone https://github.com/alols/xcape.git .
  make
  sudo make install
  # no idea why it installs everything but bin into /usr/local...
  sudo mv /usr/bin/xcape /usr/local/bin
else
  echo "xcape present"
fi

