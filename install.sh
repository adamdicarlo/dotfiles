#!/bin/bash
if [ ! which npm 2>/dev/null ]; then
  echo "ERROR: node.js (and npm) required"
  exit 1
fi
if [ ! which bower 2>/dev/null ]; then
  npm install -g bower
fi
bower install
cd home/vim
  bower install
  rm autoload/pathogen.vim 2>/dev/null
  ln -s ../bundle/vim-pathogen/autoload/pathogen.vim autoload/pathogen.vim

  # Build YouCompleteMe.
  cd bundle
  if [ ! -d ./YouCompleteMe/.git ]; then
    rm -rf ./YouCompleteMe
    git clone gh:Valloric/YouCompleteMe
  fi
  cd ..

  cd bundle/YouCompleteMe
    git submodule update --init --recursive
    ./install.sh --clang-completer
  cd ../..
cd ../..
./deploy.sh
