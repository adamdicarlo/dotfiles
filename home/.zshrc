export ZSH=$HOME/.oh-my-zsh

DEFAULT_USER=$USER
CASE_SENSITIVE="true"
ENABLE_CORRECTION="false"
ZSH_THEME="dracula"
plugins=(git)

source $ZSH/oh-my-zsh.sh
unsetopt auto_cd

# ssh
export SSH_KEY_PATH="~/.ssh/rsa_id"
if [[ -n $SSH_CONNECTION ]]; then
  export EDITOR='vim'
else
  export EDITOR='nvim'
fi

export LESS=-RFX
export PAGER=bat
export TERM=${TERM:-xterm-256color}

# Is this an interactive shell?
if [[ $- == *i* ]]; then
  source $ZSH/custom/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh

  # Extra (custom) config scripts
  for script in ~/.zsh/*.zsh; do source $script; done

  # FZF
  [ -f ~/.fzf.zsh ] && source ~/.fzf.zsh

  # Homeshick
  source "$HOME/.homesick/repos/homeshick/homeshick.sh"
  fpath=($HOME/.homesick/repos/homeshick/completions $fpath)

  if [ ! -d /home ]; then
    # z
    source `brew --prefix`/etc/profile.d/z.sh

    # Fix ^H for neovim when TERM=xterm-256color (iTerm2, and Terminal, as of macOS Sierra)
    # See https://github.com/neovim/neovim/issues/2048#issuecomment-78045837
    # No clue whether this is a set-and-forget or needs to be run here (every shell)
    infocmp $TERM | sed 's/kbs=^[hH]/kbs=\\177/' > /tmp/$TERM.ti
    tic /tmp/$TERM.ti
  fi

  # Hooks

  # `cd` hook can disrupt shell scripts, so define this after all `source` commands!
  function chpwd () {
    (which exa 1>/dev/null) && exa || gls -F --color=auto
  }
fi
