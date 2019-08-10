#!/bin/bash

green=$(tput setaf 2)
bold=$(tput bold)
normal=$(tput sgr0)

function green {
  echo "${bold}${green}${*}${normal}"
}

function command_exists () {
  type $1 &>/dev/null
}

sudo apt install -y \
  libnotify-bin \
  meld \
  ripgrep \
  xcape \
  zsh

if ! command_exists bat; then
  green "Installing bat"
  deb=bat_0.11.0_amd64.deb
  wget -q https://github.com/sharkdp/bat/releases/download/v0.11.0/$deb
  sudo dpkg -i ./$deb
  rm ./$deb
fi

# In tandem with xcape (a daemon), make CapsLock into a hybrid Ctrl+Esc key so that when tapped, it
# acts as Esc, and when held it acts as Ctrl.
gsettings set org.gnome.desktop.input-sources xkb-options "['caps:ctrl_modifier']"

./bootstrap-common.sh
