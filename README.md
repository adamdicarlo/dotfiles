# dotfiles

My Linux and OS X configuration files.

I store my dotfiles in this repository, typically checked out to `~/dotfiles`. My deploy script symlinks them to my homedir.

## Installation

1. Fork and clone the repository.
1. If you haven't already, install Node.js.
1. Run `bootstrap-mac.sh` or `bootstrap-ubuntu.sh` to install software and set up basics
1. Run `deploy.sh` to symlinks configuration files and scripts into your home dir.

If the repository is moved after deploying, `deploy.sh` must be run again in order to update the symlinks.
