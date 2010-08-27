set expandtab
set tabstop=2
set shiftwidth=2
set autoindent
set smartindent
colorscheme desert

set incsearch
set mouse=a

let php_sql_query = 1
let php_parent_error_close = 1
" let php_folding = 1

let g:debuggerMaxDepth = 5
let g:debuggerMiniBufExpl = 1

if has("autocmd")
  " Drupal's PHP file extensions
  augroup module
    autocmd BufRead,BufNewFile *.engine set filetype=php
    autocmd BufRead,BufNewFile *.inc set filetype=php
    autocmd BufRead,BufNewFile *.install set filetype=php
    autocmd BufRead,BufNewFile *.module set filetype=php
    autocmd BufRead,BufNewFile *.profile set filetype=php
    autocmd BufRead,BufNewFile *.test set filetype=php
    autocmd BufRead,BufNewFile *.theme set filetype=php
  augroup END
endif
syntax on

" Drupal 6 omni-completion: http://github.com/agileadam/drupal6complete
imap <buffer> <C-d> <C-\><C-O>:call drupal6complete#Map()<CR><C-X><C-O>
let g:drupal6complete_add_paren = 1

" Highlight redundant whitespaces and tabs.
highlight RedundantSpaces ctermbg=red guibg=red
match RedundantSpaces /\s\+$\| \+\ze\t\|\t/

" Highlight chars that go over the 80-column limit
":highlight OverLength ctermbg=red ctermfg=white guibg=red guifg=white
":match OverLength '\%81v.*'
