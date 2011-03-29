<?php

$my_projects = array(
  'cfdn',
  'dr',
  'ip',
  'potp',
  'vr',
  'wd',
);
foreach ($my_projects as $project) {
  $aliases[$project] = array(
    'uri' => 'default',
    'root' => '/home/adam/projects/' . $project . '/app/drupal',
  );
}

