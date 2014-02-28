#!/bin/bash
brew tap phinze/cask
brew install brew-cask

brew cask install xquartz
brew install meld

brew cask install google-chrome lastpass-universal macvim silverlight

brew install \
  ack \
  coreutils \
  cowsay \
  nodejs \
  git \
  git-extras \
  htop-osx \
  rbenv \
  sl \
  ssh-copy-id

# get .npmrc in place before installing node packages.
git clone git@github.com:adamdicarlo/dotfiles.git
cd ~/dotfiles
./deploy.sh

npm install -g \
  bower \
  grunt-cli \
  node-inspector \
  yo

# now we have bower, so we can do the full install.
./install.sh
