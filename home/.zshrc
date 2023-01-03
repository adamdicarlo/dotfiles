CASE_SENSITIVE="true"
ENABLE_CORRECTION="false"
setopt interactivecomments
unsetopt auto_cd

# fancy completion menus
zstyle ':completion:*' menu select

# Extra (custom) config scripts
for script in ~/.zsh/*.zsh; do source $script; done

[ -f ~/.fzf.zsh ] && source ~/.fzf.zsh

# Homeshick
source "$HOME/.homesick/repos/homeshick/homeshick.sh"
fpath=($HOME/.homesick/repos/homeshick/completions $fpath)

# macOS?
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
  ls
}
