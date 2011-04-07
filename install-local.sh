#!/bin/bash

# Install packages
cd opt || { echo "Error!" || exit 2; }
for PACKAGE in *; do
  echo "Installing $PACKAGE"
  TARGET=$HOME/opt/$PACKAGE
  rm -rf $TARGET 2>/dev/null
  cp -au $PACKAGE $TARGET
done
cd ..
rm -f $HOME/bin/drush
ln -s $HOME/opt/drush/drush $HOME/bin/drush

