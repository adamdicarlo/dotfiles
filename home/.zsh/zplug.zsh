export ZPLUG_HOME=~/.config/zplug
source $ZPLUG_HOME/init.zsh

#zplug "djui/alias-tips"
#zplug "zsh-users/zsh-autosuggestions"
#zplug "zsh-users/zsh-completions"

zplug "chriskempson/base16-shell", use:"scripts/base16-dracula.sh", defer:0
zplug "mafredri/zsh-async", defer:0, from:github

# clipcopy, clippaste commands that work the same on macOS & Linux
zplug "lib/clipboard", from:oh-my-zsh

zplug "plugins/git", from:oh-my-zsh

zplug "sindresorhus/pure", as:theme, from:github, use:pure.zsh

zplug "zsh-users/zsh-completions"
zplug "tarruda/zsh-autosuggestions",            defer:3
zplug "zsh-users/zsh-syntax-highlighting",      defer:3
zplug "zsh-users/zsh-history-substring-search", defer:3

#zplug "unixorn/tumult.plugin.zsh"
#zplug "junegunn/fzf", use:"shell/*.zsh"

zplug 'zplug/zplug', hook-build:'zplug --self-manage'

# Check for uninstalled plugins.
if ! zplug check --verbose; then
  printf "Install? [y/N]: "
  if read -q; then
    echo; zplug install
  fi
fi

# Source plugins.
zplug load
