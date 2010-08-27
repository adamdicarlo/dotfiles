# ~/.profile: executed by the command interpreter for login shells.
# This file is not read by bash(1), if ~/.bash_profile or ~/.bash_login
# exists.
# see /usr/share/doc/bash/examples/startup-files for examples.
# the files are located in the bash-doc package.

# the default umask is set in /etc/profile; for setting the umask
# for ssh logins, install and configure the libpam-umask package.
#umask 022

# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
	. "$HOME/.bashrc"
    fi
fi

PATH="$HOME/bin:$HOME/bin/drush:/var/lib/gems/1.8/bin:$PATH"

export EDITOR=vim
export CVSROOT=:pserver:bikko@cvs.drupal.org:/cvs/drupal-contrib

# make CAPSLOCK a repeating key since I use it as backspace
# xset r 66
