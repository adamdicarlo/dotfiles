
alias cbt='rdesktop -g1396x906 -u test -p ie6.die.die.die 10.0.0.155 &'

alias css-aggregation-on='drush vset --yes preprocess_css 1 && drush cc all'
alias css-aggregation-off='drush vset --yes preprocess_css 0 && drush cc all'
#alias cbt-ie6='rdesktop -g1396x900 -u Administrator win2003.opensourcery.com &'
#alias cbt-ie7='rdesktop -g1396x900 -u Administrator 10.0.0.198 &'
#alias cbt-host='vncviewer milliways.opensourcery.com &'
alias tun='pkill -f tunnels;autossh -fN oolon-tunnels'
alias ddis='drush --yes pm-disable'
alias den='drush --yes pm-enable'
alias gs='git status'
alias gd='git diff --color'
alias gpull='git pull origin master'
alias gpush='git push origin master'
alias make-patch='git diff --no-prefix --relative'
alias cd='cool-cd'
alias la='ls -lah'
alias v='vim'
alias 'cd..'='cd ..'
alias s='cd ..'
alias ss='cd ../..'
alias sss='cd ../../..'
alias ssss='cd ../../../..'
alias sssss='cd ../../../../..'
alias ssssss='cd ../../../../../..'

cool-cd () {
  builtin cd "$*" && ls
}
