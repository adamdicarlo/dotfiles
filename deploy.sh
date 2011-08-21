#!/bin/bash

#
# Deploys configuration for one file/directory by creating a symlink.
# Serves as a wrapper to ln.
#
deploy_config () {
  SOURCE="$(pwd)/$1"
  TARGET="$2"
  if [[ -f "$TARGET" && ! -h "$TARGET" ]]; then
    # Clobber .gitk if it already exists. gitk always updates it.
    if [[ "$1" == "gitk" ]]; then
      echo "Removing ~/.gitk"
    else
      echo "$TARGET exists and is not a symbolic link! Skipping."
      continue
    fi
  fi
  rm "$TARGET" 2>/dev/null
  ln -s "$SOURCE" "$TARGET"
}

#
# Performs all deployments.
#
main () {
  cd home
    for FILE in *; do
      echo "Deploying .$FILE"
      deploy_config "$FILE" ~/."$FILE"
    done
  cd ..
  cd scripts
    for FILE in *; do
      echo "Deploying bin/$FILE"
      deploy_config "$FILE" ~/bin/"$FILE"
    done
  cd ..
}

main
