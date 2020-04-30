#!/bin/bash

PATH=/usr/local/sbin:$PATH

green=$(tput setaf 2)
bold=$(tput bold)
normal=$(tput sgr0)

function green {
  echo "${bold}${green}${*}${normal}"
}

if ! which brew &>/dev/null; then
  green "Installing homebrew"
  /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

#
# Install a package if not already installed.
#
# Avoids delays and noisy brew warnings.
#
function maybe_install {
  local package=$1
  local bin=${2:-${package}}
  if ! command -v "$bin" &>/dev/null; then
    green "Installing $package"
    brew install "$package"
  fi
}

#
# Install a package that overrides a system default (a dupe), if not already installed.
#
function maybe_install_dupe {
  local package=$1
  local bin=${2:-${package}}
  shift 2
  if [[ "$(which "$bin")" != "/usr/local/bin/$bin" ]]; then
    green "Installing $package"
    brew install "$package" "$@"
  fi
}

#
# Install a cask if not already installed.
#
function maybe_install_cask {
  local cask=$1
  local name=$2
  if [[ ! -d "$HOME/Applications/$name" && ! -d "/Applications/$name" ]]; then
    green "Installing $name"
    brew cask install "$cask"
  fi
}

#
# Xcode build tools
#
which BuildStrings &>/dev/null || xcode-select --install

#
# Install-via-Homebrew tools
#
maybe_install ripgrep rg
maybe_install bat
maybe_install cmake
maybe_install coreutils gln
maybe_install diff-so-fancy
maybe_install exa
maybe_install htop-osx htop
maybe_install imagemagick convert
maybe_install most
maybe_install mtr
maybe_install python3
maybe_install shellcheck
maybe_install ssh-copy-id
maybe_install watch
maybe_install wget

maybe_install_dupe git
maybe_install_dupe findutils gfind

[ -d ~/.nvm ] || maybe_install nvm
[ -f "$(brew --prefix)/etc/profile.d/z.sh" ] || maybe_install z

if ! git info &>/dev/null; then
  green "Installing git-extras"
  brew install git-extras
fi

[ -f /usr/local/share/zsh/site-functions/_docker ] || brew install docker-completion
[ -f /usr/local/share/zsh/site-functions/_docker-compose ] || brew install docker-compose-completion
[ -f /usr/local/share/zsh/site-functions/_docker-machine ] || brew install docker-machine-completion

#
# Apps
#
maybe_install_cask alfred 'Alfred 4.app'
maybe_install_cask karabiner-elements Karabiner-Elements.app

[ -d "$HOME/Library/PreferencePanes/AppTrap.prefPane" ] || maybe_install_cask apptrap

#
# Neovim!
#
maybe_install_cask vimr VimR.app

if ! pip3 show neovim &>/dev/null; then
  green "Installing Python neovim integration"
  pip3 install --upgrade pip
  pip3 install neovim
fi

if [ ! -f ~/opt/dracula-theme/iterm/Dracula.itermcolors ]; then
  green "Installing iTerm2 Dracula theme"
  git clone -q https://github.com/dracula/iterm.git ~/opt/dracula-theme/iterm
fi

# Fonts
if [ ! -f ~/Library/Fonts/FiraCode-Regular.otf ]; then
  green "Installing FiraCode font"
  brew tap caskroom/fonts
  brew cask install font-fira-code
fi

green 'Success!'
echo "Invoking common script..."
./bootstrap-common.sh
