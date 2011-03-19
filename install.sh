#!/bin/bash

# Must be run as root.
if [[ $UID != 0 ]]; then
  echo "Please run as root."
  exit 1
fi

# Install packages
cd opt || { echo "Error!" || exit 2; }
for PACKAGE in *; do
  echo "Installing $PACKAGE"
  TARGET=/opt/$PACKAGE
  rm -rf $TARGET 2>/dev/null
  cp -au $PACKAGE $TARGET
  sudo chown -R root.root $TARGET
done
cd ..
rm -f /usr/local/bin/drush
ln -s "/opt/drush/drush" /usr/local/bin/drush

# Run drush once as root so it can download its PEAR Console_Table include
# file into its includes directory
drush >/dev/null

