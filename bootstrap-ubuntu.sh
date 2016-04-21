#!/bin/bash

echo "WARNING:"
echo "This is very out of date"
echo "Ideally, the non-platform specific stuff in bootstrap-mac.sh should be factored out and used here..."

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
