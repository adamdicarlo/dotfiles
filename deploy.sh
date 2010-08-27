#!/bin/bash

#
# Deploys configuration for one file/directory by creating a symlink.
# Serves as a wrapper to ln.
#
deploy_config () {
  SOURCE=$(pwd)/$1
  TARGET=$2
  if [[ -f $TARGET && ! -h $TARGET ]]; then
    echo "$TARGET exists and is not a symbolic link! Aborting."
    exit 1
  fi
  rm $TARGET 2>/dev/null
  ln -s $SOURCE $TARGET
}

#
# Performs all deployments.
#
main () {
  cd home
  for FILE in *; do
    deploy_config $FILE ~/.$FILE
  done
  cd ..

  deploy_config drush ~/.drush
}

main
