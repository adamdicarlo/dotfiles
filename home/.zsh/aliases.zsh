alias ..='cd ..'
alias ...='cd ../..'
alias ....='cd ../../..'
alias .....='cd ../../../..'
alias icat='kitty +kitten icat'
alias nvimconfig="$EDITOR ~/.config/nvim/init.vim"
alias zshconfig="$EDITOR ~/.zshrc"
alias v="$EDITOR"

if which exa &>/dev/null; then
  # Replace ls with exa
  alias ls='exa -al --color=always --group-directories-first --icons' # preferred listing
  alias la='exa -a --color=always --group-directories-first --icons'  # all files and dirs
  alias ll='exa -l --color=always --group-directories-first --icons'  # long format
  alias lt='exa -aT --color=always --group-directories-first --icons' # tree listing
  alias l.="exa -a | grep -E '^\.'"                                   # show only dotfiles
else
  echo "Don't forget to install 'exa'"
fi

# Replace some more things with better alternatives
if which bat &>/dev/null; then
  alias cat='bat --style header --style snip --style changes --style header'
fi

#
# Git stuff
#
alias gs='git status' # just can't get used to `gst` for this!
alias gds='git diff --staged'
alias gud='git checkout $(git_develop_branch); git pull; git checkout -'
alias gum='git checkout $(git_main_branch); git pull; git checkout -'
alias grbd='git fetch && git rebase origin/$(git_develop_branch)'
alias grbm='git fetch && git rebase origin/$(git_main_branch)'
alias gcom='git checkout $(git_main_branch); git pull'

alias git-show-merged-branches='git branch --merged $(git_main_branch) | grep -v $(git_main_branch)'
alias git-clean-up-old-branches='for i in $(git-show-merged-branches); do git branch -d $i; done'

# https://github.com/not-an-aardvark/git-delete-squashed
alias git-delete-squashed='git checkout -q $(git_main_branch) && git for-each-ref refs/heads/ "--format=%(refname:short)" | while read branch; do mergeBase=$(git merge-base $(git_main_branch) $branch) && [[ $(git cherry $(git_main_branch) $(git commit-tree $(git rev-parse $branch\^{tree}) -p $mergeBase -m _)) == "-"* ]] && git branch -D $branch; done'

#
# Docker stuff
#
alias dc='docker-compose'
alias dcup='docker-compose up -d'
alias dcre='docker-compose restart'
alias dcst='docker-compose stop'
alias dck='docker-compose kill'
alias dcps='docker-compose ps'
alias dcpl='docker-compose pull'
alias dclg='docker-compose logs --tail=50 --follow'
alias dockerclean='docker images -qf dangling=true | xargs docker rmi'

function dce {
  COMMAND=${@}
  SERVICE=${PWD##*/}
  echo "docker-compose exec ${SERVICE} ${COMMAND}"
  docker-compose exec ${SERVICE} ${COMMAND}
}

#
# General Linux stuff
#
alias grep='grep --color=auto'
alias hw='hwinfo --short'                                   # Hardware Info
alias ip="ip -color"

# Get error messages from the journal
alias jctl="journalctl -p 3 -xb"

alias psmem='ps auxf | sort -nr -k 4'
alias psmem10='ps auxf | sort -nr -k 4 | head -10'

#
# Arch Linux stuff
#
[ ! -x /usr/bin/yay ] && [ -x /usr/bin/paru ] && alias yay='paru'

# Cleanup orphaned packages
alias rmpkg="sudo pacman -Rdd"
alias cleanup='sudo pacman -Rns (pacman -Qtdq)'

# Get fastest mirrors
alias mirror="sudo reflector -f 30 -l 30 --number 10 --verbose --save /etc/pacman.d/mirrorlist"
alias mirrord="sudo reflector --latest 50 --number 20 --sort delay --save /etc/pacman.d/mirrorlist"
alias mirrors="sudo reflector --latest 50 --number 20 --sort score --save /etc/pacman.d/mirrorlist"
alias mirrora="sudo reflector --latest 50 --number 20 --sort age --save /etc/pacman.d/mirrorlist"

#
# Sway stuff
#
alias sway-appids="swaymsg -t get_tree | jq -r '.nodes[].nodes[].nodes[].app_id' | uniq"
