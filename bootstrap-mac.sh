#!/bin/bash

# todo: Install homebrew automatically?
if [ ! which brew 2>/dev/null ]; then
  echo "Please install homebrew first."
  exit 1
fi

brew tap phinze/cask
brew install brew-cask

brew cask install \
  f-lux \
  google-chrome \
  lastpass-universal \
  silverlight \
  tinkertool \
  xquartz # for meld

brew install \
  ack \
  cmake \
  coreutils \
  cowsay \
  findutils \
  fish \
  nodejs \
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
  wget

brew install macvim --override-system-vim

# get .npmrc in place before installing node packages.
cd ~
if [ ! -d ./dotfiles ]; then
  git clone git@github.com:adamdicarlo/dotfiles.git
fi
cd ./dotfiles
./deploy.sh

npm install -g \
  bower \
  csslint \
  jshint \
  jsonlint \
  js-yaml \
  grunt-cli \
  node-inspector \
  yo

sudo gem install haml-lint ruby-lint scss-lint

# now we have bower, so we can do the full install.
# todo: untested. is adding the path necessary?
PATH=$HOME/.npm-packages/bin:$PATH ./install.sh
