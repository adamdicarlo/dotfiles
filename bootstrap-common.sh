#!/bin/bash

green=$(tput setaf 2)
red=$(tput setaf 3)
bold=$(tput bold)
normal=$(tput sgr0)

function green {
  echo "${bold}${green}${*}${normal}"
}

function red {
  echo "${bold}${red}${*}${normal}"
}

function install_homeshick {
  if [ ! -d ~/.homesick/repos/homeshick ]; then
    green "Installing homeshick: Dotfiles manager"
    git clone -q git://github.com/andsens/homeshick.git ~/.homesick/repos/homeshick
  fi

  PATH=$PATH:$HOME/.homesick/repos/homeshick/bin
  [ -d ~/.homesick/repos/dotfiles ]      || homeshick clone git@github.com:adamdicarlo/dotfiles
  [ -d ~/.homesick/repos/castle-neovim ] || homeshick clone git@github.com:adamdicarlo/castle-neovim
  homeshick link dotfiles
  homeshick link castle-neovim
}

function install_fnm {
  if [ ! -d "$HOME/.config/fnm" ]; then
    green "Installing fnm: Fast Node version manager"
    curl -fsSL https://fnm.vercel.app/install | bash -s -- \
      --install-dir "$HOME/.config/fnm" \
      --skip-shell
  fi
}

function install_shell {
  if [[ "$SHELL" != "/usr/bin/zsh" && "$SHELL" != "/bin/zsh" ]]; then
    if which zsh 2>/dev/null; then
      green "Updating login shell to zsh"
      chsh -s $(which zsh)
    else
      red "Unable to determine path of zsh"
    fi
  fi
}

function install_zplug {
  ZPLUG_HOME=$HOME/.config/zplug
  if [ ! -d $ZPLUG_HOME ]; then
    green "Installing zplug: Zsh plugin manager"
    curl -sL --proto-redir -all,https \
      https://raw.githubusercontent.com/zplug/installer/master/installer.zsh \
      | ZPLUG_HOME=$ZPLUG_HOME zsh
  fi
}


install_shell
install_homeshick
install_fnm
install_zplug
