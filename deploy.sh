#!/bin/bash
cd home
for FILE in *; do
  SOURCE=$(pwd)/$FILE
  TARGET=~/.$FILE
  if [[ -f $TARGET && ! -h $TARGET ]]; then
    echo "$TARGET exists and is not a symbolic link! Aborting."
    exit 1
  fi
  rm $TARGET 2>/dev/null
  ln -s $SOURCE $TARGET
done
cd ..
