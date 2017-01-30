export ZSH=$HOME/.oh-my-zsh

DEFAULT_USER=$USER
CASE_SENSITIVE="true"
ENABLE_CORRECTION="false"
ZSH_THEME="dracula"
plugins=(git)

source $ZSH/oh-my-zsh.sh
unsetopt auto_cd

source $ZSH/custom/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh

# Extra (custom) config scripts
for script in ~/.zsh/*.zsh; do source $script; done

# FZF
[ -f ~/.fzf.zsh ] && source ~/.fzf.zsh

# Homeshick
source "$HOME/.homesick/repos/homeshick/homeshick.sh"
fpath=($HOME/.homesick/repos/homeshick/completions $fpath)

# ssh
export SSH_KEY_PATH="~/.ssh/rsa_id"
if [[ -n $SSH_CONNECTION ]]; then
  export EDITOR='vim'
else
  export EDITOR='nvim'
fi

# z
source `brew --prefix`/etc/profile.d/z.sh

# Hooks

# `cd` hook can disrupt shell scripts, so define this after all `source` commands!
function chpwd () {
  gls -F --color=auto
}
