# dotfiles

My Linux and OS X configuration files.

I store my dotfiles in this repository, typically checked out to `~/dotfiles`. My deploy script symlinks them to my homedir.

## Installation

1. Fork and clone the repository.
1. If you haven't already, install Node.js.
1. Run `./install.sh` in order to install tools, and "deploy" symlinks to the configuration files, scripts, and tools in this repository. If the repository is moved after deploying, the ./deploy.sh script must be run again in order to update the symlinks.
