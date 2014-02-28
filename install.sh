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
  rm autoload/pathogen.vim
  ln -s ../bundle/vim-pathogen/autoload/pathogen.vim autoload/pathogen.vim
cd ../..
./deploy.sh
