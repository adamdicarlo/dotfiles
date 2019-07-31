export ZPLUG_HOME=~/.config/zplug
source $ZPLUG_HOME/init.zsh

#zplug "djui/alias-tips"
#zplug "zsh-users/zsh-autosuggestions"
#zplug "zsh-users/zsh-completions"
zplug "zsh-users/zsh-syntax-highlighting", defer:2 # Should be loaded 2nd last.
#zplug "zsh-users/zsh-history-substring-search", defer:3 # Should be loaded last.
zplug "mafredri/zsh-async", defer:0, from:github
zplug "sindresorhus/pure", as:theme, from:github, use:pure.zsh
zplug "chriskempson/base16-shell", use:"scripts/base16-dracula.sh", defer:0

#zplug "unixorn/tumult.plugin.zsh"
#zplug "junegunn/fzf", use:"shell/*.zsh"

# Check for uninstalled plugins.
if ! zplug check --verbose; then
  printf "Install? [y/N]: "
  if read -q; then
    echo; zplug install
  fi
fi

# Source plugins.
zplug load
