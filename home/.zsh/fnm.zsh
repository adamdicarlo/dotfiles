if which fnm >/dev/null; then
  export FNM_DIR=~/.config/fnm
  eval "$(fnm env)"
fi
