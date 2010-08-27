core = 6.x

; Test that a specific tag can be pulled.
projects[tao][type] = theme
projects[tao][download][type] = git
projects[tao][download][url] = git://github.com/developmentseed/tao.git
projects[tao][download][tag] = drupal-6--1-9

; Test that a branch can be pulled. We use a super-old "stale" branch in the
; Aegir project that we expect not to change.
projects[hostmaster][type] = profile
projects[hostmaster][download][type] = git
projects[hostmaster][download][url] = git://git.aegirproject.org/hostmaster.git
projects[hostmaster][download][branch] = DRUPAL-5

; Test that a specific revision can be pulled. Note that provision is not
; actually a module.
projects[provision][type] = module
projects[provision][download][type] = git
projects[provision][download][url] = git://git.aegirproject.org/provision.git
projects[provision][download][revision] = 017345defebaa6214a8962522e0e9a94889d0020
