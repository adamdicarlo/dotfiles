#!/bin/bash

green=$(tput setaf 2)
bold=$(tput bold)
normal=$(tput sgr0)

function green {
  echo "${bold}${green}${*}${normal}"
}

function install_homeshick {
  if [ ! -d ~/.homesick/repos/homeshick ]; then
    green "Installing homeshick"
    git clone -q git://github.com/andsens/homeshick.git ~/.homesick/repos/homeshick
  fi

  PATH=$PATH:$HOME/.homesick/repos/homeshick/bin
  [ -d ~/.homesick/repos/dotfiles ]      || homeshick clone git@github.com:adamdicarlo/dotfiles
  [ -d ~/.homesick/repos/castle-neovim ] || homeshick clone git@github.com:adamdicarlo/castle-neovim
  homeshick link dotfiles
  homeshick link castle-neovim
}

function install_nvm {
  if [ ! -d "$HOME/.config/nvm" ]; then
    # PROFILE=/dev/null Tells nvm installer not to append lines to shell profiles (.bashrc, .zshrc)
    # nvm will install into $XDG_CONFIG_HOME/nvm if the var is defined.
    PROFILE=/dev/null
    XDG_CONFIG_HOME=$HOME/.config

    # Reason for fork: https://github.com/nvm-sh/nvm/pull/2013
    curl -o- https://raw.githubusercontent.com/bees/nvm/update_install_dir_output/install.sh | bash
  fi
}

function install_oh_my_zsh {
  # Oh My Zsh
  if [ ! -d ~/.oh-my-zsh ]; then
    green "Installing oh-my-zsh"
    sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"
  fi
}

function install_zplug {
  ZPLUG_HOME=$HOME/.config/zplug
  if [ ! -d $ZPLUG_HOME ]; then
    green "Installing zplug"
    curl -sL --proto-redir -all,https \
      https://raw.githubusercontent.com/zplug/installer/master/installer.zsh \
      | ZPLUG_HOME=$ZPLUG_HOME zsh
  fi
}

function install_omz_syntax_highlighting {
  # Syntax highlighting (fish-like) for zsh
  if [ ! -d ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting ]; then
    green "Installing zsh-syntax-highlighting"
    git clone -q https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting
  fi
}

function install_omz_dracula_theme {
  # Dracula theme
  if [ ! -d ~/.oh-my-zsh/themes/dracula.zsh-theme ]; then
    green "Installing zsh Dracula theme"
    git clone -q https://github.com/dracula/zsh.git ~/.oh-my-zsh/themes/dracula.zsh-theme
  fi
}

function install_shell {
  if [ "$SHELL" != "/usr/bin/zsh" ]; then
    green "Updating login shell to zsh"
    chsh --shell /usr/bin/zsh
  fi
}

function install_vim_plug {
  green "Installing/updating Plug.vim"
  curl -SsfLo ~/.config/nvim/autoload/plug.vim --create-dirs \
      https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
}

install_homeshick
install_nvm
install_oh_my_zsh
install_zplug
install_omz_dracula_theme
install_omz_syntax_highlighting
install_vim_plug
install_shell
