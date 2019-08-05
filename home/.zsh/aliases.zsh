alias ag='ag --path-to-ignore ~/.ignore'
alias cat='bat --theme=OneHalfDark'
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

function dce {
  COMMAND=${@}
  SERVICE=${PWD##*/}
  echo "docker-compose exec ${SERVICE} ${COMMAND}"
  docker-compose exec ${SERVICE} ${COMMAND}
}
