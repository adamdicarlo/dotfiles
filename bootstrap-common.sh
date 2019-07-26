#!/bin/bash

green=$(tput setaf 2)
bold=$(tput bold)
normal=$(tput sgr0)

function green {
  echo "${bold}${green}${*}${normal}"
}

function homeshick {
  if [ ! -d ~/.homesick/repos/homeshick ]; then
    green "Installing homeshick"
    git clone -q git://github.com/andsens/homeshick.git ~/.homesick/repos/homeshick
    PATH=$PATH:$HOME/.homesick/repos/homeshick/bin
    [ -d ~/.homesick/repos/dotfiles ] || homeshick clone git@github.com:adamdicarlo/dotfiles
    [ -d ~/.homesick/repos/neovim ]   || homeshick clone git@github.com:adamdicarlo/castle-neovim
    homeshick link dotfiles
    homeshick link neovim
  fi
}

function oh_my_zsh {
  # Oh My Zsh
  if [ ! -d ~/.oh-my-zsh ]; then
    green "Installing oh-my-zsh"
    sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"
  fi
}

function omz_syntax_highlighting {
  # Syntax highlighting (fish-like) for zsh
  if [ ! -d ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting ]; then
    green "Installing zsh-syntax-highlighting"
    git clone -q https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting
  fi
}

function omz_dracula_theme {
  # Dracula theme
  if [ ! -f ~/.oh-my-zsh/themes/dracula.zsh-theme ]; then
    green "Installing zsh Dracula theme"
    [ -d ~/opt/dracula-theme ] || git clone -q https://github.com/dracula/zsh.git ~/opt/dracula-theme/zsh
    mkdir -p ~/.oh-my-zsh/themes
    ln -s ~/opt/dracula-theme/zsh/dracula.zsh-theme ~/.oh-my-zsh/themes/dracula.zsh-theme
  fi
}

function vim_plug {
  green "Installing/updating Plug.vim"
  curl -SsfLo ~/.config/nvim/autoload/plug.vim --create-dirs \
      https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
}


homeshick
oh_my_zsh
omz_dracula_theme
omz_syntax_highlighting
vim_plug
