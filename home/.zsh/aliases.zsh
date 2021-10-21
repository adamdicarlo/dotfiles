alias ag='ag --path-to-ignore ~/.ignore'
alias icat='kitty +kitten icat'
alias nvimconfig="$EDITOR ~/.config/nvim/init.vim"
alias zshconfig="$EDITOR ~/.zshrc"
alias v="$EDITOR"

if which exa &>/dev/null; then
  alias ls='exa -B'
  alias la='exa -lB'
else
  echo "Don't forget to install 'exa'"
fi

# git
alias gs='git status' # just can't get used to `gst` for this!
alias gds='git diff --staged'
alias gum='git checkout master; git pull; git checkout -'
alias gud='git checkout develop; git pull; git checkout -'
alias grbd='git rebase develop'

alias git-clean-up-old-branches='for i in $(git-show-merged-branches); do git branch -d $i; done'

# https://github.com/not-an-aardvark/git-delete-squashed
alias git-delete-squashed='git checkout -q master && git for-each-ref refs/heads/ "--format=%(refname:short)" | while read branch; do mergeBase=$(git merge-base master $branch) && [[ $(git cherry master $(git commit-tree $(git rev-parse $branch\^{tree}) -p $mergeBase -m _)) == "-"* ]] && git branch -D $branch; done'

# docker
alias dc='docker-compose'
alias dcup='docker-compose up -d'
alias dcre='docker-compose restart'
alias dcst='docker-compose stop'
alias dck='docker-compose kill'
alias dcps='docker-compose ps'
alias dcpl='docker-compose pull'
alias dclg='docker-compose logs --tail=50 --follow'
alias dockerclean='docker images -qf dangling=true | xargs docker rmi'

# sway/wayland
alias sway-appids="swaymsg -t get_tree | jq -r '.nodes[].nodes[].nodes[].app_id' | uniq"

function dce {
  COMMAND=${@}
  SERVICE=${PWD##*/}
  echo "docker-compose exec ${SERVICE} ${COMMAND}"
  docker-compose exec ${SERVICE} ${COMMAND}
}
