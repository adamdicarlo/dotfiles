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

function install_oh_my_zsh {
  # Oh My Zsh
  if [ ! -d ~/.oh-my-zsh ]; then
    green "Installing oh-my-zsh"
    sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"
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
  if [ ! -f ~/.oh-my-zsh/themes/dracula.zsh-theme ]; then
    green "Installing zsh Dracula theme"
    [ -d ~/opt/dracula-theme ] || git clone -q https://github.com/dracula/zsh.git ~/opt/dracula-theme/zsh
    mkdir -p ~/.oh-my-zsh/themes
    ln -s ~/opt/dracula-theme/zsh/dracula.zsh-theme ~/.oh-my-zsh/themes/dracula.zsh-theme
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
install_oh_my_zsh
install_omz_dracula_theme
install_omz_syntax_highlighting
install_vim_plug
install_shell
