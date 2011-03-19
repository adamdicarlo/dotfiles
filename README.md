My Linux and OS X configuration files.

This repository serves to back them up and help keep them in sync across machines.

The general idea is to store all my configuration files in this repository and create symlinks where the OS expects to find those files. This works for most home directory dotfiles, but there is of course a blurry line between configuration/development environment and what should be stored in a repository for such.

So, for drush (the Drupal command line interface), I'm currently using git submodules. This removes the need to store unpacked tarballs of drush (and drush_make) in this repository.

INSTALLATION

1. Clone the repository (to, say, ~/dotfiles).
2. Run $(./deploy.sh) in order to 'deploy' symlinks to the configuration files in this repository. If the repository is moved after deploying, the deploy script must be run again in order to update the symlinks.
3. Run $(git submodule init && git submodule update) in order to pull down tools stored in external git repositories (e.g. drush).
4. Run $(sudo ./install.sh) in order to install these tools into /opt/ with symlinks in /usr/local/bin/ that quickly get the tools into $PATH.

