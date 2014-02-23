 = dotfiles

My Linux and OS X configuration files.

I store my dotfiles in this repository, typically checked out to ~/dotfiles. My deploy script symlinks them to my homedir.

== Installation

1. Clone the repository.
1. Run `./deploy.sh` in order to 'deploy' symlinks to the configuration files in this repository. If the repository is moved after deploying, the deploy script must be run again in order to update the symlinks.
